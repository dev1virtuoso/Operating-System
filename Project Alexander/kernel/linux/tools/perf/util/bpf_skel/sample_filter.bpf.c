// SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause)
// Copyright (c) 2023 Google
#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#include "sample-filter.h"

/* BPF map that will be filled by user space */
struct filters {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, int);
	__type(value, struct perf_bpf_filter_entry[MAX_FILTERS]);
	__uint(max_entries, 1);
} filters SEC(".maps");

/*
 * An evsel has multiple instances for each CPU or task but we need a single
 * id to be used as a key for the idx_hash.  This hashmap would translate the
 * instance's ID to a representative ID.
 */
struct event_hash {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, __u64);
	__type(value, __u64);
	__uint(max_entries, 1);
} event_hash SEC(".maps");

/* tgid/evtid to filter index */
struct idx_hash {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, struct idx_hash_key);
	__type(value, int);
	__uint(max_entries, 1);
} idx_hash SEC(".maps");

/* tgid to filter index */
struct lost_count {
	__uint(type, BPF_MAP_TYPE_ARRAY);
	__type(key, int);
	__type(value, int);
	__uint(max_entries, 1);
} dropped SEC(".maps");

volatile const int use_idx_hash;

void *bpf_cast_to_kern_ctx(void *) __ksym;

/* new kernel perf_sample_data definition */
struct perf_sample_data___new {
	__u64 sample_flags;
} __attribute__((preserve_access_index));

/* new kernel perf_mem_data_src definition */
union perf_mem_data_src___new {
	__u64 val;
	struct {
		__u64   mem_op:5,	/* type of opcode */
			mem_lvl:14,	/* memory hierarchy level */
			mem_snoop:5,	/* snoop mode */
			mem_lock:2,	/* lock instr */
			mem_dtlb:7,	/* tlb access */
			mem_lvl_num:4,	/* memory hierarchy level number */
			mem_remote:1,   /* remote */
			mem_snoopx:2,	/* snoop mode, ext */
			mem_blk:3,	/* access blocked */
			mem_hops:3,	/* hop level */
			mem_rsvd:18;
	};
};

/* helper function to return the given perf sample data */
static inline __u64 perf_get_sample(struct bpf_perf_event_data_kern *kctx,
				    struct perf_bpf_filter_entry *entry)
{
	struct perf_sample_data___new *data = (void *)kctx->data;

	if (!bpf_core_field_exists(data->sample_flags))
		return 0;

#define BUILD_CHECK_SAMPLE(x)					\
	_Static_assert((1 << (PBF_TERM_##x - PBF_TERM_SAMPLE_START)) == PERF_SAMPLE_##x, \
		"Mismatched PBF term to sample bit " #x)
	BUILD_CHECK_SAMPLE(IP);
	BUILD_CHECK_SAMPLE(TID);
	BUILD_CHECK_SAMPLE(TIME);
	BUILD_CHECK_SAMPLE(ADDR);
	BUILD_CHECK_SAMPLE(ID);
	BUILD_CHECK_SAMPLE(CPU);
	BUILD_CHECK_SAMPLE(PERIOD);
	BUILD_CHECK_SAMPLE(WEIGHT);
	BUILD_CHECK_SAMPLE(DATA_SRC);
	BUILD_CHECK_SAMPLE(TRANSACTION);
	BUILD_CHECK_SAMPLE(PHYS_ADDR);
	BUILD_CHECK_SAMPLE(CGROUP);
	BUILD_CHECK_SAMPLE(DATA_PAGE_SIZE);
	BUILD_CHECK_SAMPLE(CODE_PAGE_SIZE);
	BUILD_CHECK_SAMPLE(WEIGHT_STRUCT);
#undef BUILD_CHECK_SAMPLE

	/* For sample terms check the sample bit is set. */
	if (entry->term >= PBF_TERM_SAMPLE_START && entry->term <= PBF_TERM_SAMPLE_END &&
	    (data->sample_flags & (1 << (entry->term - PBF_TERM_SAMPLE_START))) == 0)
		return 0;

	switch (entry->term) {
	case PBF_TERM_IP:
		return kctx->data->ip;
	case PBF_TERM_ID:
		return kctx->data->id;
	case PBF_TERM_TID:
		if (entry->part)
			return kctx->data->tid_entry.pid;
		else
			return kctx->data->tid_entry.tid;
	case PBF_TERM_CPU:
		return kctx->data->cpu_entry.cpu;
	case PBF_TERM_TIME:
		return kctx->data->time;
	case PBF_TERM_ADDR:
		return kctx->data->addr;
	case PBF_TERM_PERIOD:
		return kctx->data->period;
	case PBF_TERM_TRANSACTION:
		return kctx->data->txn;
	case PBF_TERM_WEIGHT_STRUCT:
		if (entry->part == 1)
			return kctx->data->weight.var1_dw;
		if (entry->part == 2)
			return kctx->data->weight.var2_w;
		if (entry->part == 3)
			return kctx->data->weight.var3_w;
		/* fall through */
	case PBF_TERM_WEIGHT:
		return kctx->data->weight.full;
	case PBF_TERM_PHYS_ADDR:
		return kctx->data->phys_addr;
	case PBF_TERM_CGROUP:
		return kctx->data->cgroup;
	case PBF_TERM_CODE_PAGE_SIZE:
		return kctx->data->code_page_size;
	case PBF_TERM_DATA_PAGE_SIZE:
		return kctx->data->data_page_size;
	case PBF_TERM_DATA_SRC:
		if (entry->part == 1)
			return kctx->data->data_src.mem_op;
		if (entry->part == 2)
			return kctx->data->data_src.mem_lvl_num;
		if (entry->part == 3) {
			__u32 snoop = kctx->data->data_src.mem_snoop;
			__u32 snoopx = kctx->data->data_src.mem_snoopx;

			return (snoopx << 5) | snoop;
		}
		if (entry->part == 4)
			return kctx->data->data_src.mem_remote;
		if (entry->part == 5)
			return kctx->data->data_src.mem_lock;
		if (entry->part == 6)
			return kctx->data->data_src.mem_dtlb;
		if (entry->part == 7)
			return kctx->data->data_src.mem_blk;
		if (entry->part == 8) {
			union perf_mem_data_src___new *data = (void *)&kctx->data->data_src;

			if (bpf_core_field_exists(data->mem_hops))
				return data->mem_hops;

			return 0;
		}
		/* return the whole word */
		return kctx->data->data_src.val;
	case PBF_TERM_UID:
		return bpf_get_current_uid_gid() & 0xFFFFFFFF;
	case PBF_TERM_GID:
		return bpf_get_current_uid_gid() >> 32;
	case PBF_TERM_NONE:
	case __PBF_UNUSED_TERM4:
	case __PBF_UNUSED_TERM5:
	case __PBF_UNUSED_TERM9:
	case __PBF_UNUSED_TERM10:
	case __PBF_UNUSED_TERM11:
	case __PBF_UNUSED_TERM12:
	case __PBF_UNUSED_TERM13:
	case __PBF_UNUSED_TERM16:
	case __PBF_UNUSED_TERM18:
	case __PBF_UNUSED_TERM20:
	default:
		break;
	}
	return 0;
}

#define CHECK_RESULT(data, op, val)			\
	if (!(data op val)) {				\
		if (!in_group)				\
			goto drop;			\
	} else if (in_group) {				\
		group_result = 1;			\
	}

/* BPF program to be called from perf event overflow handler */
SEC("perf_event")
int perf_sample_filter(void *ctx)
{
	struct bpf_perf_event_data_kern *kctx;
	struct perf_bpf_filter_entry *entry;
	__u64 sample_data;
	int in_group = 0;
	int group_result = 0;
	int i, k;
	int *losts;

	kctx = bpf_cast_to_kern_ctx(ctx);

	k = 0;

	if (use_idx_hash) {
		struct idx_hash_key key = {
			.tgid = bpf_get_current_pid_tgid() >> 32,
		};
		__u64 eid = kctx->event->id;
		__u64 *key_id;
		int *idx;

		/* get primary_event_id */
		if (kctx->event->parent)
			eid = kctx->event->parent->id;

		key_id = bpf_map_lookup_elem(&event_hash, &eid);
		if (key_id == NULL)
			goto drop;

		key.evt_id = *key_id;

		idx = bpf_map_lookup_elem(&idx_hash, &key);
		if (idx)
			k = *idx;
		else
			goto drop;
	}

	entry = bpf_map_lookup_elem(&filters, &k);
	if (entry == NULL)
		goto drop;

	for (i = 0; i < MAX_FILTERS; i++) {
		sample_data = perf_get_sample(kctx, &entry[i]);

		switch (entry[i].op) {
		case PBF_OP_EQ:
			CHECK_RESULT(sample_data, ==, entry[i].value)
			break;
		case PBF_OP_NEQ:
			CHECK_RESULT(sample_data, !=, entry[i].value)
			break;
		case PBF_OP_GT:
			CHECK_RESULT(sample_data, >, entry[i].value)
			break;
		case PBF_OP_GE:
			CHECK_RESULT(sample_data, >=, entry[i].value)
			break;
		case PBF_OP_LT:
			CHECK_RESULT(sample_data, <, entry[i].value)
			break;
		case PBF_OP_LE:
			CHECK_RESULT(sample_data, <=, entry[i].value)
			break;
		case PBF_OP_AND:
			CHECK_RESULT(sample_data, &, entry[i].value)
			break;
		case PBF_OP_GROUP_BEGIN:
			in_group = 1;
			group_result = 0;
			break;
		case PBF_OP_GROUP_END:
			if (group_result == 0)
				goto drop;
			in_group = 0;
			break;
		case PBF_OP_DONE:
			/* no failures so far, accept it */
			return 1;
		}
	}
	/* generate sample data */
	return 1;

drop:
	losts = bpf_map_lookup_elem(&dropped, &k);
	if (losts != NULL)
		__sync_fetch_and_add(losts, 1);

	return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * tegra210_amx.h - Definitions for Tegra210 AMX driver
 *
 * Copyright (c) 2021, NVIDIA CORPORATION.  All rights reserved.
 *
 */

#ifndef __TEGRA210_AMX_H__
#define __TEGRA210_AMX_H__

/* Register offsets from TEGRA210_AMX*_BASE */
#define TEGRA210_AMX_RX_STATUS			0x0c
#define TEGRA210_AMX_RX_INT_STATUS		0x10
#define TEGRA210_AMX_RX_INT_MASK		0x14
#define TEGRA210_AMX_RX_INT_SET			0x18
#define TEGRA210_AMX_RX_INT_CLEAR		0x1c
#define TEGRA210_AMX_RX1_CIF_CTRL		0x20
#define TEGRA210_AMX_RX2_CIF_CTRL		0x24
#define TEGRA210_AMX_RX3_CIF_CTRL		0x28
#define TEGRA210_AMX_RX4_CIF_CTRL		0x2c
#define TEGRA210_AMX_TX_STATUS			0x4c
#define TEGRA210_AMX_TX_INT_STATUS		0x50
#define TEGRA210_AMX_TX_INT_MASK		0x54
#define TEGRA210_AMX_TX_INT_SET			0x58
#define TEGRA210_AMX_TX_INT_CLEAR		0x5c
#define TEGRA210_AMX_TX_CIF_CTRL		0x60
#define TEGRA210_AMX_ENABLE			0x80
#define TEGRA210_AMX_SOFT_RESET			0x84
#define TEGRA210_AMX_CG				0x88
#define TEGRA210_AMX_STATUS			0x8c
#define TEGRA210_AMX_INT_STATUS			0x90
#define TEGRA210_AMX_CTRL			0xa4
#define TEGRA210_AMX_OUT_BYTE_EN0		0xa8
#define TEGRA210_AMX_OUT_BYTE_EN1		0xac
#define TEGRA210_AMX_CYA			0xb0
#define TEGRA210_AMX_CFG_RAM_CTRL		0xb8
#define TEGRA210_AMX_CFG_RAM_DATA		0xbc

#define TEGRA194_AMX_RX1_FRAME_PERIOD		0xc0
#define TEGRA194_AMX_RX4_FRAME_PERIOD		0xcc
#define TEGRA194_AMX_RX4_LAST_FRAME_PERIOD	0xdc

/* Fields in TEGRA210_AMX_ENABLE */
#define TEGRA210_AMX_ENABLE_SHIFT			0

/* Fields in TEGRA210_AMX_CTRL */
#define TEGRA210_AMX_CTRL_MSTR_RX_NUM_SHIFT		14
#define TEGRA210_AMX_CTRL_MSTR_RX_NUM_MASK		(3 << TEGRA210_AMX_CTRL_MSTR_RX_NUM_SHIFT)

#define TEGRA210_AMX_CTRL_RX_DEP_SHIFT			12
#define TEGRA210_AMX_CTRL_RX_DEP_MASK			(3 << TEGRA210_AMX_CTRL_RX_DEP_SHIFT)

/* Fields in TEGRA210_AMX_CFG_RAM_CTRL */
#define TEGRA210_AMX_CFG_RAM_CTRL_RW_SHIFT		14
#define TEGRA210_AMX_CFG_RAM_CTRL_RW_WRITE		(1 << TEGRA210_AMX_CFG_RAM_CTRL_RW_SHIFT)

#define TEGRA210_AMX_CFG_RAM_CTRL_ADDR_INIT_EN_SHIFT	13
#define TEGRA210_AMX_CFG_RAM_CTRL_ADDR_INIT_EN		(1 << TEGRA210_AMX_CFG_RAM_CTRL_ADDR_INIT_EN_SHIFT)

#define TEGRA210_AMX_CFG_RAM_CTRL_SEQ_ACCESS_EN_SHIFT	12
#define TEGRA210_AMX_CFG_RAM_CTRL_SEQ_ACCESS_EN		(1 << TEGRA210_AMX_CFG_RAM_CTRL_SEQ_ACCESS_EN_SHIFT)

#define TEGRA210_AMX_CFG_CTRL_RAM_ADDR_SHIFT		0

/* Fields in TEGRA210_AMX_SOFT_RESET */
#define TEGRA210_AMX_SOFT_RESET_SOFT_EN			1
#define TEGRA210_AMX_SOFT_RESET_SOFT_RESET_MASK		TEGRA210_AMX_SOFT_RESET_SOFT_EN

#define TEGRA210_AMX_AUDIOCIF_CH_STRIDE		4
#define TEGRA210_AMX_RAM_DEPTH			16
#define TEGRA210_AMX_MAP_STREAM_NUM_SHIFT	6
#define TEGRA210_AMX_MAP_WORD_NUM_SHIFT		2
#define TEGRA210_AMX_MAP_BYTE_NUM_SHIFT		0

enum {
	TEGRA210_AMX_WAIT_ON_ALL,
	TEGRA210_AMX_WAIT_ON_ANY,
};

struct tegra210_amx_soc_data {
	const struct regmap_config *regmap_conf;
	bool auto_disable;
};

struct tegra210_amx {
	const struct tegra210_amx_soc_data *soc_data;
	unsigned int map[TEGRA210_AMX_RAM_DEPTH];
	struct regmap *regmap;
	unsigned int byte_mask[2];
};

#endif
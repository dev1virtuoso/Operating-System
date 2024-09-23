// SPDX-License-Identifier: GPL-2.0-only
// This file incorporates work covered by the following copyright notice:
// Copyright (c) 2023 Intel Corporation
// Copyright (c) 2024 Advanced Micro Devices, Inc.

/*
 *  soc_sdw_rt712_sdca - Helpers to handle RT712-SDCA from generic machine driver
 */

#include <linux/device.h>
#include <linux/errno.h>
#include <linux/soundwire/sdw.h>
#include <linux/soundwire/sdw_type.h>
#include <sound/control.h>
#include <sound/soc.h>
#include <sound/soc-acpi.h>
#include <sound/soc-dapm.h>
#include <sound/soc_sdw_utils.h>

/*
 * dapm routes for rt712 spk will be registered dynamically according
 * to the number of rt712 spk used. The first two entries will be registered
 * for one codec case, and the last two entries are also registered
 * if two rt712s are used.
 */
static const struct snd_soc_dapm_route rt712_spk_map[] = {
	{ "Speaker", NULL, "rt712 SPOL" },
	{ "Speaker", NULL, "rt712 SPOR" },
};

int asoc_sdw_rt712_spk_rtd_init(struct snd_soc_pcm_runtime *rtd, struct snd_soc_dai *dai)
{
	struct snd_soc_card *card = rtd->card;
	int ret;

	card->components = devm_kasprintf(card->dev, GFP_KERNEL,
					  "%s spk:rt712",
					  card->components);
	if (!card->components)
		return -ENOMEM;

	ret = snd_soc_dapm_add_routes(&card->dapm, rt712_spk_map, ARRAY_SIZE(rt712_spk_map));
	if (ret)
		dev_err(rtd->dev, "failed to add SPK map: %d\n", ret);

	return ret;
}
EXPORT_SYMBOL_NS(asoc_sdw_rt712_spk_rtd_init, SND_SOC_SDW_UTILS);

/*
 * Copyright 2012-15 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef __DC_OPTC_DCN201_H__
#define __DC_OPTC_DCN201_H__

#include "dcn20/dcn20_optc.h"

#define TG_COMMON_REG_LIST_DCN201(inst) \
	TG_COMMON_REG_LIST_DCN(inst),\
	SRI(OTG_GLOBAL_CONTROL1, OTG, inst),\
	SRI(OTG_GLOBAL_CONTROL2, OTG, inst),\
	SRI(OTG_GSL_WINDOW_X, OTG, inst),\
	SRI(OTG_GSL_WINDOW_Y, OTG, inst),\
	SRI(OTG_VUPDATE_KEEPOUT, OTG, inst),\
	SRI(OTG_DSC_START_POSITION, OTG, inst),\
	SRI(OPTC_DATA_FORMAT_CONTROL, ODM, inst),\
	SRI(OPTC_BYTES_PER_PIXEL, ODM, inst),\
	SRI(OPTC_WIDTH_CONTROL, ODM, inst),\
	SR(DWB_SOURCE_SELECT)

#define TG_COMMON_MASK_SH_LIST_DCN201(mask_sh)\
	TG_COMMON_MASK_SH_LIST_DCN(mask_sh),\
	SF(OTG0_OTG_GLOBAL_CONTROL1, MASTER_UPDATE_LOCK_DB_X, mask_sh),\
	SF(OTG0_OTG_GLOBAL_CONTROL1, MASTER_UPDATE_LOCK_DB_Y, mask_sh),\
	SF(OTG0_OTG_GLOBAL_CONTROL1, MASTER_UPDATE_LOCK_DB_EN, mask_sh),\
	SF(OTG0_OTG_GLOBAL_CONTROL2, GLOBAL_UPDATE_LOCK_EN, mask_sh),\
	SF(OTG0_OTG_DOUBLE_BUFFER_CONTROL, OTG_RANGE_TIMING_DBUF_UPDATE_MODE, mask_sh),\
	SF(OTG0_OTG_GSL_WINDOW_X, OTG_GSL_WINDOW_START_X, mask_sh),\
	SF(OTG0_OTG_GSL_WINDOW_X, OTG_GSL_WINDOW_END_X, mask_sh), \
	SF(OTG0_OTG_GSL_WINDOW_Y, OTG_GSL_WINDOW_START_Y, mask_sh),\
	SF(OTG0_OTG_GSL_WINDOW_Y, OTG_GSL_WINDOW_END_Y, mask_sh),\
	SF(OTG0_OTG_VUPDATE_KEEPOUT, OTG_MASTER_UPDATE_LOCK_VUPDATE_KEEPOUT_EN, mask_sh), \
	SF(OTG0_OTG_VUPDATE_KEEPOUT, MASTER_UPDATE_LOCK_VUPDATE_KEEPOUT_START_OFFSET, mask_sh), \
	SF(OTG0_OTG_VUPDATE_KEEPOUT, MASTER_UPDATE_LOCK_VUPDATE_KEEPOUT_END_OFFSET, mask_sh), \
	SF(OTG0_OTG_GSL_CONTROL, OTG_GSL_MASTER_MODE, mask_sh), \
	SF(OTG0_OTG_GSL_CONTROL, OTG_MASTER_UPDATE_LOCK_GSL_EN, mask_sh), \
	SF(OTG0_OTG_DSC_START_POSITION, OTG_DSC_START_POSITION_X, mask_sh), \
	SF(OTG0_OTG_DSC_START_POSITION, OTG_DSC_START_POSITION_LINE_NUM, mask_sh),\
	SF(ODM0_OPTC_DATA_SOURCE_SELECT, OPTC_SEG0_SRC_SEL, mask_sh),\
	SF(ODM0_OPTC_DATA_FORMAT_CONTROL, OPTC_DSC_MODE, mask_sh),\
	SF(ODM0_OPTC_BYTES_PER_PIXEL, OPTC_DSC_BYTES_PER_PIXEL, mask_sh),\
	SF(ODM0_OPTC_WIDTH_CONTROL, OPTC_DSC_SLICE_WIDTH, mask_sh),\
	SF(DWB_SOURCE_SELECT, OPTC_DWB0_SOURCE_SELECT, mask_sh),\
	SF(DWB_SOURCE_SELECT, OPTC_DWB1_SOURCE_SELECT, mask_sh),\
	SF(DWB_SOURCE_SELECT, OPTC_DWB1_SOURCE_SELECT, mask_sh)

void dcn201_timing_generator_init(struct optc *optc);
#endif

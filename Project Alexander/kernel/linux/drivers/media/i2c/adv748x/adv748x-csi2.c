// SPDX-License-Identifier: GPL-2.0+
/*
 * Driver for Analog Devices ADV748X CSI-2 Transmitter
 *
 * Copyright (C) 2017 Renesas Electronics Corp.
 */

#include <linux/module.h>

#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

#include "adv748x.h"

static const unsigned int adv748x_csi2_txa_fmts[] = {
	MEDIA_BUS_FMT_UYVY8_1X16,
	MEDIA_BUS_FMT_RGB888_1X24,
};

static const unsigned int adv748x_csi2_txb_fmts[] = {
	MEDIA_BUS_FMT_UYVY8_1X16,
};

int adv748x_csi2_set_virtual_channel(struct adv748x_csi2 *tx, unsigned int vc)
{
	return tx_write(tx, ADV748X_CSI_VC_REF, vc << ADV748X_CSI_VC_REF_SHIFT);
}

/**
 * adv748x_csi2_register_link : Register and link internal entities
 *
 * @tx: CSI2 private entity
 * @v4l2_dev: Video registration device
 * @src: Source subdevice to establish link
 * @src_pad: Pad number of source to link to this @tx
 * @enable: Link enabled flag
 *
 * Ensure that the subdevice is registered against the v4l2_device, and link the
 * source pad to the sink pad of the CSI2 bus entity.
 */
static int adv748x_csi2_register_link(struct adv748x_csi2 *tx,
				      struct v4l2_device *v4l2_dev,
				      struct v4l2_subdev *src,
				      unsigned int src_pad,
				      bool enable)
{
	int ret;

	if (!src->v4l2_dev) {
		ret = v4l2_device_register_subdev(v4l2_dev, src);
		if (ret)
			return ret;
	}

	ret = media_create_pad_link(&src->entity, src_pad,
				    &tx->sd.entity, ADV748X_CSI2_SINK,
				    enable ? MEDIA_LNK_FL_ENABLED : 0);
	if (ret)
		return ret;

	if (enable)
		tx->src = src;

	return 0;
}

/* -----------------------------------------------------------------------------
 * v4l2_subdev_internal_ops
 */

static int adv748x_csi2_init_state(struct v4l2_subdev *sd,
				   struct v4l2_subdev_state *state)
{
	static const struct v4l2_mbus_framefmt adv748x_csi2_default_fmt = {
		.width		= 1280,
		.height		= 720,
		.code		= MEDIA_BUS_FMT_UYVY8_1X16,
		.colorspace	= V4L2_COLORSPACE_SRGB,
		.field		= V4L2_FIELD_NONE,
		.ycbcr_enc	= V4L2_YCBCR_ENC_DEFAULT,
		.quantization	= V4L2_QUANTIZATION_DEFAULT,
		.xfer_func	= V4L2_XFER_FUNC_DEFAULT,
	};
	struct v4l2_mbus_framefmt *fmt;

	fmt = v4l2_subdev_state_get_format(state, ADV748X_CSI2_SINK);
	*fmt = adv748x_csi2_default_fmt;

	fmt = v4l2_subdev_state_get_format(state, ADV748X_CSI2_SOURCE);
	*fmt = adv748x_csi2_default_fmt;

	return 0;
}

/*
 * We use the internal registered operation to be able to ensure that our
 * incremental subdevices (not connected in the forward path) can be registered
 * against the resulting video path and media device.
 */

static int adv748x_csi2_registered(struct v4l2_subdev *sd)
{
	struct adv748x_csi2 *tx = adv748x_sd_to_csi2(sd);
	struct adv748x_state *state = tx->state;
	int ret;

	adv_dbg(state, "Registered %s (%s)", is_txa(tx) ? "TXA":"TXB",
			sd->name);

	/*
	 * Link TXA to AFE and HDMI, and TXB to AFE only as TXB cannot output
	 * HDMI.
	 *
	 * The HDMI->TXA link is enabled by default, as is the AFE->TXB one.
	 */
	if (is_afe_enabled(state)) {
		ret = adv748x_csi2_register_link(tx, sd->v4l2_dev,
						 &state->afe.sd,
						 ADV748X_AFE_SOURCE,
						 is_txb(tx));
		if (ret)
			return ret;

		/* TXB can output AFE signals only. */
		if (is_txb(tx))
			state->afe.tx = tx;
	}

	/* Register link to HDMI for TXA only. */
	if (is_txb(tx) || !is_hdmi_enabled(state))
		return 0;

	ret = adv748x_csi2_register_link(tx, sd->v4l2_dev, &state->hdmi.sd,
					 ADV748X_HDMI_SOURCE, true);
	if (ret)
		return ret;

	/* The default HDMI output is TXA. */
	state->hdmi.tx = tx;

	return 0;
}

static const struct v4l2_subdev_internal_ops adv748x_csi2_internal_ops = {
	.init_state = adv748x_csi2_init_state,
	.registered = adv748x_csi2_registered,
};

/* -----------------------------------------------------------------------------
 * v4l2_subdev_video_ops
 */

static int adv748x_csi2_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct adv748x_csi2 *tx = adv748x_sd_to_csi2(sd);
	struct v4l2_subdev *src;

	src = adv748x_get_remote_sd(&tx->pads[ADV748X_CSI2_SINK]);
	if (!src)
		return -EPIPE;

	return v4l2_subdev_call(src, video, s_stream, enable);
}

static const struct v4l2_subdev_video_ops adv748x_csi2_video_ops = {
	.s_stream = adv748x_csi2_s_stream,
};

/* -----------------------------------------------------------------------------
 * v4l2_subdev_pad_ops
 *
 * The CSI2 bus pads are ignorant to the data sizes or formats.
 * But we must support setting the pad formats for format propagation.
 */

static int adv748x_csi2_enum_mbus_code(struct v4l2_subdev *sd,
				       struct v4l2_subdev_state *sd_state,
				       struct v4l2_subdev_mbus_code_enum *code)
{
	struct adv748x_csi2 *tx = adv748x_sd_to_csi2(sd);
	const unsigned int *codes = is_txa(tx) ?
				    adv748x_csi2_txa_fmts :
				    adv748x_csi2_txb_fmts;
	size_t num_fmts = is_txa(tx) ? ARRAY_SIZE(adv748x_csi2_txa_fmts)
				     : ARRAY_SIZE(adv748x_csi2_txb_fmts);

	/*
	 * The format available on the source pad is the one applied on the sink
	 * pad.
	 */
	if (code->pad == ADV748X_CSI2_SOURCE) {
		struct v4l2_mbus_framefmt *fmt;

		if (code->index)
			return -EINVAL;

		fmt = v4l2_subdev_state_get_format(sd_state, ADV748X_CSI2_SINK);
		code->code = fmt->code;

		return 0;
	}

	if (code->index >= num_fmts)
		return -EINVAL;

	code->code = codes[code->index];

	return 0;
}

static bool adv748x_csi2_is_fmt_supported(struct adv748x_csi2 *tx, u32 code)
{
	const unsigned int *codes = is_txa(tx) ?
				    adv748x_csi2_txa_fmts :
				    adv748x_csi2_txb_fmts;
	size_t num_fmts = is_txa(tx) ? ARRAY_SIZE(adv748x_csi2_txa_fmts)
				     : ARRAY_SIZE(adv748x_csi2_txb_fmts);

	for (unsigned int i = 0; i < num_fmts; i++) {
		if (codes[i] == code)
			return true;
	}

	return false;
}

static int adv748x_csi2_set_format(struct v4l2_subdev *sd,
				   struct v4l2_subdev_state *sd_state,
				   struct v4l2_subdev_format *sdformat)
{
	struct adv748x_csi2 *tx = adv748x_sd_to_csi2(sd);
	struct v4l2_mbus_framefmt *mbusformat;

	if (sdformat->pad == ADV748X_CSI2_SOURCE)
		return v4l2_subdev_get_fmt(sd, sd_state, sdformat);

	/*
	 * Make sure the format is supported, if not default it to
	 * UYVY8 as it's supported by both TXes.
	 */
	if (!adv748x_csi2_is_fmt_supported(tx, sdformat->format.code))
		sdformat->format.code = MEDIA_BUS_FMT_UYVY8_1X16;

	mbusformat = v4l2_subdev_state_get_format(sd_state, sdformat->pad);
	*mbusformat = sdformat->format;

	/* Propagate format to the source pad. */
	mbusformat = v4l2_subdev_state_get_format(sd_state, ADV748X_CSI2_SOURCE);
	*mbusformat = sdformat->format;

	return 0;
}

static int adv748x_csi2_get_mbus_config(struct v4l2_subdev *sd, unsigned int pad,
					struct v4l2_mbus_config *config)
{
	struct adv748x_csi2 *tx = adv748x_sd_to_csi2(sd);

	if (pad != ADV748X_CSI2_SOURCE)
		return -EINVAL;

	config->type = V4L2_MBUS_CSI2_DPHY;
	config->bus.mipi_csi2.num_data_lanes = tx->active_lanes;

	return 0;
}

static const struct v4l2_subdev_pad_ops adv748x_csi2_pad_ops = {
	.enum_mbus_code = adv748x_csi2_enum_mbus_code,
	.get_fmt = v4l2_subdev_get_fmt,
	.set_fmt = adv748x_csi2_set_format,
	.get_mbus_config = adv748x_csi2_get_mbus_config,
};

/* -----------------------------------------------------------------------------
 * v4l2_subdev_ops
 */

static const struct v4l2_subdev_ops adv748x_csi2_ops = {
	.video = &adv748x_csi2_video_ops,
	.pad = &adv748x_csi2_pad_ops,
};

/* -----------------------------------------------------------------------------
 * Subdev module and controls
 */

int adv748x_csi2_set_pixelrate(struct v4l2_subdev *sd, s64 rate)
{
	struct adv748x_csi2 *tx = adv748x_sd_to_csi2(sd);

	if (!tx->pixel_rate)
		return -EINVAL;

	return v4l2_ctrl_s_ctrl_int64(tx->pixel_rate, rate);
}

static int adv748x_csi2_s_ctrl(struct v4l2_ctrl *ctrl)
{
	switch (ctrl->id) {
	case V4L2_CID_PIXEL_RATE:
		return 0;
	default:
		return -EINVAL;
	}
}

static const struct v4l2_ctrl_ops adv748x_csi2_ctrl_ops = {
	.s_ctrl = adv748x_csi2_s_ctrl,
};

static int adv748x_csi2_init_controls(struct adv748x_csi2 *tx)
{

	v4l2_ctrl_handler_init(&tx->ctrl_hdl, 1);

	tx->pixel_rate = v4l2_ctrl_new_std(&tx->ctrl_hdl,
					   &adv748x_csi2_ctrl_ops,
					   V4L2_CID_PIXEL_RATE, 1, INT_MAX,
					   1, 1);

	tx->sd.ctrl_handler = &tx->ctrl_hdl;
	if (tx->ctrl_hdl.error) {
		v4l2_ctrl_handler_free(&tx->ctrl_hdl);
		return tx->ctrl_hdl.error;
	}

	return v4l2_ctrl_handler_setup(&tx->ctrl_hdl);
}

int adv748x_csi2_init(struct adv748x_state *state, struct adv748x_csi2 *tx)
{
	int ret;

	if (!is_tx_enabled(tx))
		return 0;

	adv748x_subdev_init(&tx->sd, state, &adv748x_csi2_ops,
			    MEDIA_ENT_F_VID_IF_BRIDGE,
			    is_txa(tx) ? "txa" : "txb");

	/* Register internal ops for incremental subdev registration */
	tx->sd.internal_ops = &adv748x_csi2_internal_ops;

	tx->pads[ADV748X_CSI2_SINK].flags = MEDIA_PAD_FL_SINK;
	tx->pads[ADV748X_CSI2_SOURCE].flags = MEDIA_PAD_FL_SOURCE;

	ret = media_entity_pads_init(&tx->sd.entity, ADV748X_CSI2_NR_PADS,
				     tx->pads);
	if (ret)
		return ret;

	ret = v4l2_async_subdev_endpoint_add(&tx->sd,
					     of_fwnode_handle(state->endpoints[tx->port]));
	if (ret)
		goto err_free_media;

	ret = adv748x_csi2_init_controls(tx);
	if (ret)
		goto err_cleanup_subdev;

	tx->sd.state_lock = &state->mutex;
	ret = v4l2_subdev_init_finalize(&tx->sd);
	if (ret)
		goto err_free_ctrl;

	ret = v4l2_async_register_subdev(&tx->sd);
	if (ret)
		goto err_free_ctrl;

	return 0;

err_free_ctrl:
	v4l2_ctrl_handler_free(&tx->ctrl_hdl);
err_cleanup_subdev:
	v4l2_subdev_cleanup(&tx->sd);
err_free_media:
	media_entity_cleanup(&tx->sd.entity);

	return ret;
}

void adv748x_csi2_cleanup(struct adv748x_csi2 *tx)
{
	if (!is_tx_enabled(tx))
		return;

	v4l2_async_unregister_subdev(&tx->sd);
	media_entity_cleanup(&tx->sd.entity);
	v4l2_ctrl_handler_free(&tx->ctrl_hdl);
	v4l2_subdev_cleanup(&tx->sd);
}

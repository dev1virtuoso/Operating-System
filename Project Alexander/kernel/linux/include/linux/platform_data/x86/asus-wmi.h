/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PLATFORM_DATA_X86_ASUS_WMI_H
#define __PLATFORM_DATA_X86_ASUS_WMI_H

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/dmi.h>

/* WMI Methods */
#define ASUS_WMI_METHODID_SPEC	        0x43455053 /* BIOS SPECification */
#define ASUS_WMI_METHODID_SFBD		0x44424653 /* Set First Boot Device */
#define ASUS_WMI_METHODID_GLCD		0x44434C47 /* Get LCD status */
#define ASUS_WMI_METHODID_GPID		0x44495047 /* Get Panel ID?? (Resol) */
#define ASUS_WMI_METHODID_QMOD		0x444F4D51 /* Quiet MODe */
#define ASUS_WMI_METHODID_SPLV		0x4C425053 /* Set Panel Light Value */
#define ASUS_WMI_METHODID_AGFN		0x4E464741 /* Atk Generic FuNction */
#define ASUS_WMI_METHODID_SFUN		0x4E554653 /* FUNCtionalities */
#define ASUS_WMI_METHODID_SDSP		0x50534453 /* Set DiSPlay output */
#define ASUS_WMI_METHODID_GDSP		0x50534447 /* Get DiSPlay output */
#define ASUS_WMI_METHODID_DEVP		0x50564544 /* DEVice Policy */
#define ASUS_WMI_METHODID_OSVR		0x5256534F /* OS VeRsion */
#define ASUS_WMI_METHODID_DCTS		0x53544344 /* Device status (DCTS) */
#define ASUS_WMI_METHODID_DSTS		0x53545344 /* Device status (DSTS) */
#define ASUS_WMI_METHODID_BSTS		0x53545342 /* Bios STatuS ? */
#define ASUS_WMI_METHODID_DEVS		0x53564544 /* DEVice Set */
#define ASUS_WMI_METHODID_CFVS		0x53564643 /* CPU Frequency Volt Set */
#define ASUS_WMI_METHODID_KBFT		0x5446424B /* KeyBoard FilTer */
#define ASUS_WMI_METHODID_INIT		0x54494E49 /* INITialize */
#define ASUS_WMI_METHODID_HKEY		0x59454B48 /* Hot KEY ?? */

#define ASUS_WMI_UNSUPPORTED_METHOD	0xFFFFFFFE

/* Wireless */
#define ASUS_WMI_DEVID_HW_SWITCH	0x00010001
#define ASUS_WMI_DEVID_WIRELESS_LED	0x00010002
#define ASUS_WMI_DEVID_CWAP		0x00010003
#define ASUS_WMI_DEVID_WLAN		0x00010011
#define ASUS_WMI_DEVID_WLAN_LED		0x00010012
#define ASUS_WMI_DEVID_BLUETOOTH	0x00010013
#define ASUS_WMI_DEVID_GPS		0x00010015
#define ASUS_WMI_DEVID_WIMAX		0x00010017
#define ASUS_WMI_DEVID_WWAN3G		0x00010019
#define ASUS_WMI_DEVID_UWB		0x00010021

/* Leds */
/* 0x000200XX and 0x000400XX */
#define ASUS_WMI_DEVID_LED1		0x00020011
#define ASUS_WMI_DEVID_LED2		0x00020012
#define ASUS_WMI_DEVID_LED3		0x00020013
#define ASUS_WMI_DEVID_LED4		0x00020014
#define ASUS_WMI_DEVID_LED5		0x00020015
#define ASUS_WMI_DEVID_LED6		0x00020016
#define ASUS_WMI_DEVID_MICMUTE_LED		0x00040017

/* Disable Camera LED */
#define ASUS_WMI_DEVID_CAMERA_LED_NEG	0x00060078 /* 0 = on (unused) */
#define ASUS_WMI_DEVID_CAMERA_LED	0x00060079 /* 1 = on */

/* Backlight and Brightness */
#define ASUS_WMI_DEVID_ALS_ENABLE	0x00050001 /* Ambient Light Sensor */
#define ASUS_WMI_DEVID_BACKLIGHT	0x00050011
#define ASUS_WMI_DEVID_BRIGHTNESS	0x00050012
#define ASUS_WMI_DEVID_KBD_BACKLIGHT	0x00050021
#define ASUS_WMI_DEVID_LIGHT_SENSOR	0x00050022 /* ?? */
#define ASUS_WMI_DEVID_LIGHTBAR		0x00050025
#define ASUS_WMI_DEVID_OOBE		0x0005002F
/* This can only be used to disable the screen, not re-enable */
#define ASUS_WMI_DEVID_SCREENPAD_POWER	0x00050031
/* Writing a brightness re-enables the screen if disabled */
#define ASUS_WMI_DEVID_SCREENPAD_LIGHT	0x00050032
#define ASUS_WMI_DEVID_FAN_BOOST_MODE	0x00110018
#define ASUS_WMI_DEVID_THROTTLE_THERMAL_POLICY 0x00120075
#define ASUS_WMI_DEVID_THROTTLE_THERMAL_POLICY_VIVO 0x00110019

/* Misc */
#define ASUS_WMI_DEVID_PANEL_OD		0x00050019
#define ASUS_WMI_DEVID_CAMERA		0x00060013
#define ASUS_WMI_DEVID_LID_FLIP		0x00060062
#define ASUS_WMI_DEVID_LID_FLIP_ROG	0x00060077
#define ASUS_WMI_DEVID_MINI_LED_MODE	0x0005001E
#define ASUS_WMI_DEVID_MINI_LED_MODE2	0x0005002E

/* Storage */
#define ASUS_WMI_DEVID_CARDREADER	0x00080013

/* Input */
#define ASUS_WMI_DEVID_TOUCHPAD		0x00100011
#define ASUS_WMI_DEVID_TOUCHPAD_LED	0x00100012
#define ASUS_WMI_DEVID_FNLOCK		0x00100023

/* Fan, Thermal */
#define ASUS_WMI_DEVID_THERMAL_CTRL	0x00110011
#define ASUS_WMI_DEVID_FAN_CTRL		0x00110012 /* deprecated */
#define ASUS_WMI_DEVID_CPU_FAN_CTRL	0x00110013
#define ASUS_WMI_DEVID_GPU_FAN_CTRL	0x00110014
#define ASUS_WMI_DEVID_MID_FAN_CTRL 0x00110031
#define ASUS_WMI_DEVID_CPU_FAN_CURVE	0x00110024
#define ASUS_WMI_DEVID_GPU_FAN_CURVE	0x00110025
#define ASUS_WMI_DEVID_MID_FAN_CURVE	0x00110032

/* Tunables for AUS ROG laptops */
#define ASUS_WMI_DEVID_PPT_PL2_SPPT		0x001200A0
#define ASUS_WMI_DEVID_PPT_PL1_SPL		0x001200A3
#define ASUS_WMI_DEVID_PPT_APU_SPPT		0x001200B0
#define ASUS_WMI_DEVID_PPT_PLAT_SPPT	0x001200B1
#define ASUS_WMI_DEVID_PPT_FPPT			0x001200C1
#define ASUS_WMI_DEVID_NV_DYN_BOOST		0x001200C0
#define ASUS_WMI_DEVID_NV_THERM_TARGET	0x001200C2

/* Power */
#define ASUS_WMI_DEVID_PROCESSOR_STATE	0x00120012

/* Deep S3 / Resume on LID open */
#define ASUS_WMI_DEVID_LID_RESUME	0x00120031

/* Maximum charging percentage */
#define ASUS_WMI_DEVID_RSOC		0x00120057

/* Keyboard dock */
#define ASUS_WMI_DEVID_KBD_DOCK		0x00120063

/* Charging mode - 1=Barrel, 2=USB */
#define ASUS_WMI_DEVID_CHARGE_MODE	0x0012006C

/* MCU powersave mode */
#define ASUS_WMI_DEVID_MCU_POWERSAVE   0x001200E2

/* epu is connected? 1 == true */
#define ASUS_WMI_DEVID_EGPU_CONNECTED	0x00090018
/* egpu on/off */
#define ASUS_WMI_DEVID_EGPU		0x00090019

/* dgpu on/off */
#define ASUS_WMI_DEVID_DGPU		0x00090020

/* gpu mux switch, 0 = dGPU, 1 = Optimus */
#define ASUS_WMI_DEVID_GPU_MUX		0x00090016
#define ASUS_WMI_DEVID_GPU_MUX_VIVO	0x00090026

/* TUF laptop RGB modes/colours */
#define ASUS_WMI_DEVID_TUF_RGB_MODE	0x00100056
#define ASUS_WMI_DEVID_TUF_RGB_MODE2	0x0010005A

/* TUF laptop RGB power/state */
#define ASUS_WMI_DEVID_TUF_RGB_STATE	0x00100057

/* Bootup sound control */
#define ASUS_WMI_DEVID_BOOT_SOUND	0x00130022

/* DSTS masks */
#define ASUS_WMI_DSTS_STATUS_BIT	0x00000001
#define ASUS_WMI_DSTS_UNKNOWN_BIT	0x00000002
#define ASUS_WMI_DSTS_PRESENCE_BIT	0x00010000
#define ASUS_WMI_DSTS_USER_BIT		0x00020000
#define ASUS_WMI_DSTS_BIOS_BIT		0x00040000
#define ASUS_WMI_DSTS_BRIGHTNESS_MASK	0x000000FF
#define ASUS_WMI_DSTS_MAX_BRIGTH_MASK	0x0000FF00
#define ASUS_WMI_DSTS_LIGHTBAR_MASK	0x0000000F

#if IS_REACHABLE(CONFIG_ASUS_WMI)
int asus_wmi_evaluate_method(u32 method_id, u32 arg0, u32 arg1, u32 *retval);
#else
static inline int asus_wmi_evaluate_method(u32 method_id, u32 arg0, u32 arg1,
					   u32 *retval)
{
	return -ENODEV;
}
#endif

/* To be used by both hid-asus and asus-wmi to determine which controls kbd_brightness */
static const struct dmi_system_id asus_use_hid_led_dmi_ids[] = {
	{
		.matches = {
			DMI_MATCH(DMI_PRODUCT_FAMILY, "ROG Zephyrus"),
		},
	},
	{
		.matches = {
			DMI_MATCH(DMI_PRODUCT_FAMILY, "ROG Strix"),
		},
	},
	{
		.matches = {
			DMI_MATCH(DMI_PRODUCT_FAMILY, "ROG Flow"),
		},
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_NAME, "GA403U"),
		},
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_NAME, "GU605M"),
		},
	},
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_NAME, "RC71L"),
		},
	},
	{ },
};

#endif	/* __PLATFORM_DATA_X86_ASUS_WMI_H */

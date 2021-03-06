/* drivers/input/touchscreen/gt9xx.h
 * 
 * 2010 - 2013 Goodix Technology.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be a reference 
 * to you, when you are integrating the GOODiX's CTP IC into your system, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 * 
 */

#ifndef _GOODIX_GT9XX_H_
#define _GOODIX_GT9XX_H_

#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/gpio.h>
#if defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>
#elif defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
#define GOODIX_SUSPEND_LEVEL 1
#endif




//***************************PART1:ON/OFF define*******************************
#define GTP_CUSTOM_CFG        1
#define GTP_CHANGE_X2Y        0
#define GTP_DRIVER_SEND_CFG   1
#define GTP_HAVE_TOUCH_KEY    1
#define GTP_POWER_CTRL_SLEEP  0
#define GTP_ICS_SLOT_REPORT   1 

#define GTP_AUTO_UPDATE       1    // auto update fw by .bin file as default
#define GTP_HEADER_FW_UPDATE  1    // auto update fw by gtp_default_FW in gt9xx_firmware.h, function together with GTP_AUTO_UPDATE
#define GTP_AUTO_UPDATE_CFG   0    // auto update config by .cfg file, function together with GTP_AUTO_UPDATE

#define GTP_COMPATIBLE_MODE   0    // compatible with GT9XXF

#define GTP_CREATE_WR_NODE    1
#define GTP_ESD_PROTECT       1    // esd protection with a cycle of 2 seconds
#define GTP_WITH_PEN          0

#define GTP_PEN_HAVE_BUTTON   0    // active pen has buttons, function together with GTP_WITH_PEN
#define GTP_GESTURE_WAKEUP    0    // gesture wakeup

#define GTP_DEBUG_ON          0
#define GTP_DEBUG_ARRAY_ON    0
#define GTP_DEBUG_FUNC_ON     0

#if GTP_COMPATIBLE_MODE
typedef enum
{
    CHIP_TYPE_GT9  = 0,
    CHIP_TYPE_GT9F = 1,
} CHIP_TYPE_T;
#endif

struct goodix_ts_data {
    spinlock_t irq_lock;
    struct i2c_client *client;
    struct input_dev  *input_dev;
    struct hrtimer timer;
    struct work_struct  work;
    s32 irq_is_disable;
    s32 use_irq;
    u16 abs_x_max;
    u16 abs_y_max;
	u16 version;
	u16 irq_gpio;
	u16 reset_gpio;	
	u8  sensor_id;
    u8  max_touch_num;
    u8  int_trigger_type;
    u8  green_wake_mode;
    u8  enter_update;
    u8  gtp_is_suspend;
    u8  gtp_rawdiff_mode;
    u8  gtp_cfg_len;
    u8  fixed_cfg;
    u8  fw_error;
    u8  pnl_init_error;
    
#if GTP_WITH_PEN
    struct input_dev *pen_dev;
#endif

#if GTP_ESD_PROTECT
    spinlock_t esd_lock;
    u8  esd_running;
    s32 clk_tick_cnt;
#endif

#if GTP_COMPATIBLE_MODE
    u16 bak_ref_len;
    s32 ref_chk_fs_times;
    s32 clk_chk_fs_times;
    CHIP_TYPE_T chip_type;
    u8 rqst_processing;
    u8 is_950;
#endif
	struct mutex lock;
#if defined(CONFIG_FB)
	struct notifier_block fb_notif;
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif    
};

extern u16 show_len;
extern u16 total_len;


//*************************** PART2:TODO define **********************************
// STEP_1(REQUIRED): Define Configuration Information Group(s)
// Sensor_ID Map:
/* sensor_opt1 sensor_opt2 Sensor_ID
    GND         GND         0 
    VDDIO       GND         1 
    NC          GND         2 
    GND         NC/300K     3 
    VDDIO       NC/300K     4 
    NC          NC/300K     5 
*/
#ifdef CONFIG_BOARD_BONNIE
	// TODO: define your own default or for Sensor_ID == 0 config here. 
	// The predefined one is just a sample config, which is not suitable for your tp in most cases.
	//007206_中兴通讯上海_P821A16_宇顺_GT968_106C_v68_Config_20140217.cfg
#define CTP_CFG_GROUP1 {\
		0x45,0x40,0x01,0xE0,0x01,0x05,0x35,0x05,0x01,0x0A,\
		0x28,0x0A,0x50,0x28,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x12,0x17,0x21,0x14,0x87,0x07,0x0A,\
		0x32,0x00,0xB9,0x08,0x00,0x00,0x00,0x9B,0x03,0x1D,\
		0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x24,0x46,0x94,0xC5,0x04,0x05,0x00,0x00,0x04,\
		0xA6,0x26,0x00,0x90,0x2C,0x00,0x7F,0x32,0x00,0x71,\
		0x39,0x00,0x64,0x41,0x00,0x64,0x10,0x30,0x48,0x00,\
		0x07,0x40,0x30,0x99,0x99,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x1D,\
		0x1E,0x1F,0x20,0x22,0x24,0x28,0xFF,0xFF,0xFF,0xFF,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x01\
	}
	// TODO: define your config for Sensor_ID == 1 here, if needed
#define CTP_CFG_GROUP2 {\
		}
	
	// TODO: define your config for Sensor_ID == 2 here, if needed
#define CTP_CFG_GROUP3 {\
		0x41,0x40,0x01,0xE0,0x01,0x05,0x05,0x05,0x01,0x0A,\
		0x28,0x0A,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x12,0x17,0x21,0x14,0x87,0x08,0x0A,\
		0x3C,0x00,0x50,0x06,0x00,0x00,0x00,0x9B,0x03,0x1D,\
		0x00,0x41,0x00,0x00,0x00,0x03,0x00,0x02,0x00,0x00,\
		0x00,0x1E,0x5A,0x94,0xD5,0x04,0x05,0x00,0x00,0x04,\
		0xDF,0x21,0x00,0xB6,0x29,0x00,0x92,0x34,0x00,0x76,\
		0x41,0x00,0x61,0x51,0x00,0x61,0x10,0x30,0x48,0x00,\
		0xF5,0x4A,0x3A,0x66,0x66,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x1D,\
		0x1E,0x1F,0x20,0x22,0x24,0x28,0x29,0xFF,0xFF,0xFF,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 3 here, if needed
#define CTP_CFG_GROUP4 {\
		}
	
	// TODO: define your config for Sensor_ID == 4 here, if needed
#define CTP_CFG_GROUP5 {\
		}
	
	// TODO: define your config for Sensor_ID == 5 here, if needed
#define CTP_CFG_GROUP6 {\
		}
	
	//////////////////////////////////////////////////////////////
	////  put	GT960's config here //////////////////////////////
	//////////////////////////////////////////////////////////////
	// TODO: define your config for Sensor_ID == 0 here, if needed
	//008656_中兴移动_Z667T_超声_GT960_V41_00_Config_20140822
#define CTP_CFG_GROUP7 {\
		0x41,0x40,0x01,0xE0,0x01,0x05,0x35,0x85,0x01,0x08,\
		0x28,0x08,0x4B,0x32,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x08,0x12,0x17,0x21,0x14,0x88,0x07,0x0A,\
		0x2D,0x00,0xB9,0x08,0x00,0x00,0x00,0x9B,0x03,0x1D,\
		0x41,0x81,0x0A,0x00,0x00,0x03,0x00,0x00,0x00,0x00,\
		0x00,0x19,0x41,0x94,0xD5,0x03,0x05,0x0A,0x00,0x04,\
		0xC7,0x1B,0x00,0xA9,0x21,0x00,0x93,0x28,0x00,0x76,\
		0x31,0x00,0x63,0x3B,0x00,0x63,0x10,0x28,0x48,0x00,\
		0x05,0x4A,0x3A,0x77,0x7F,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x55,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x0C,\
		0x1D,0x1E,0x1F,0x20,0x22,0x24,0x28,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 1 here, if needed
	//008639_欧菲_MCF_0355594_中兴移动_Z667T_GT960_V42_01_Config_20140724
#define CTP_CFG_GROUP8 {\
		0x42,0x40,0x01,0xE0,0x01,0x05,0x35,0x05,0x12,0x08,\
		0x28,0x08,0x4B,0x32,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x08,0x12,0x17,0x21,0x14,0x88,0x07,0x0A,\
		0x2D,0x00,0xB9,0x08,0x00,0x00,0x00,0x9B,0x03,0x1D,\
		0x41,0x81,0x0A,0x00,0x00,0x03,0x00,0x00,0x00,0x00,\
		0x00,0x19,0x41,0x94,0xD5,0x03,0x05,0x0A,0x00,0x04,\
		0xC7,0x1B,0x00,0xA9,0x21,0x00,0x93,0x28,0x00,0x76,\
		0x31,0x00,0x63,0x3B,0x00,0x63,0x10,0x30,0x48,0x00,\
		0x05,0x4A,0x3A,0x77,0x77,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x55,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x0C,\
		0x1D,0x1E,0x1F,0x20,0x22,0x24,0x28,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 2 here, if needed
	//中兴_Z667T_联创_02_GT960_1040_V68_Config_20140918
#define CTP_CFG_GROUP9 {\
		0x44,0x40,0x01,0xE0,0x01,0x05,0x35,0x05,0x01,0x08,\
		0x28,0x08,0x4B,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x06,0x12,0x17,0x21,0x14,0x88,0x27,0x0A,\
		0x50,0x52,0x40,0x04,0x00,0x00,0x00,0x02,0x03,0x11,\
		0x00,0xC1,0x0A,0x00,0x00,0x03,0x00,0x00,0x00,0x00,\
		0x00,0x23,0x82,0x94,0xD5,0x02,0x05,0x0A,0x00,0x04,\
		0xF6,0x28,0x00,0xC0,0x34,0x00,0x95,0x44,0x00,0x75,\
		0x58,0x00,0x5C,0x72,0x00,0x5C,0x10,0x30,0x48,0x00,\
		0x05,0x4A,0x3A,0x77,0x77,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x55,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x0C,\
		0x1D,0x1E,0x1F,0x20,0x22,0x24,0x28,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 3 here, if needed
#define CTP_CFG_GROUP10 {\
		}
	
	// TODO: define your config for Sensor_ID == 4 here, if needed
#define CTP_CFG_GROUP11 {\
		}
	
	// TODO: define your config for Sensor_ID == 5 here, if needed
#define CTP_CFG_GROUP12 {\
		}
#elif defined(CONFIG_BOARD_EROS)||defined(CONFIG_BOARD_FAERIE)||defined(CONFIG_BOARD_IRIS)
	//5TODO: define your own default or for Sensor_ID == 0 config here. 
	// The predefined one is just a sample config, which is not suitable for your tp in most cases.
	
	//008692_中兴_Z752C_宇顺_GT9147_1040_V68_00_Config_20141112
	//解决触控距离和改善湿手指问题
	//(整机用)008692_中兴通讯南京_Z752C_宇顺_S80260_00_GT9147_1073_V46_Config_20141208
	//(整机用)008692_中兴通讯南京_Z752C_宇顺_S80260_00_GT9147_1073_V47_Config_20141219
	//改善边缘效果
	//008692_中兴_Z752C_宇顺_00_GT9147_1073_V48_Config_20141224
	//改善按键效果
	//008692_中兴通讯南京_Z752C_宇顺_S80260_00_GT9147_1073_V4B_Config_20150121
	//改善边缘点击效果
	//008692_中兴通讯南京_Z752C_宇顺_S80260_00_GT9147_1073_V4D_Config_20150327
	//改善边缘划线效果
#define CTP_CFG_GROUP1 {\
		0x4D,0xE0,0x01,0x20,0x03,0x05,0xB5,0xC1,0x01,0x0F,\
		0x28,0x0F,0x64,0x4B,0x03,0x05,0x00,0x00,0xBF,0x7F,\
		0x00,0x00,0x08,0x17,0x18,0x1A,0x14,0x89,0x29,0x0A,\
		0x3E,0x40,0xD3,0x07,0x00,0x00,0x00,0x9A,0x33,0x11,\
		0x00,0x01,0x2A,0x44,0x50,0x00,0x0A,0x32,0x09,0x00,\
		0x2B,0x1C,0x64,0x94,0xC5,0x04,0x00,0x0C,0x00,0x04,\
		0xE9,0x20,0x00,0xB9,0x29,0x00,0x93,0x35,0x00,0x76,\
		0x44,0x00,0x5F,0x58,0x00,0x5F,0x10,0x30,0x48,0x00,\
		0xF5,0x4A,0x3A,0xCC,0xCC,0x17,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,\
		0x14,0x03,0x85,0x10,0x42,0x84,0x0F,0x19,0x02,0x11,\
		0x50,0x28,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x0E,0x00,0x0C,0x02,0x0A,0x04,0x08,0x05,\
		0x06,0x1D,0x29,0x1E,0x28,0x1F,0x24,0x20,0x22,0x2A,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0x10,0x01\
	}
	// TODO: define your config for Sensor_ID == 1 here, if needed
	//008687_中兴_Z752C_联创_GT9147_1040_V67_01_Config_20141023
	//008687_中兴_Z752C_联创_01_GT9147_1073_V43_Config_20141210
	//008687_中兴_Z752C_联创_01_GT9147_1073_V44_Config_20141219
	//改善边缘效果
	//008687_中兴_Z752C_联创_01_GT9147_1073_V45_Config_20141224
	//改善按键反应
#define CTP_CFG_GROUP2 {\
		0x45,0xE0,0x01,0x20,0x03,0x05,0xB5,0xC1,0x01,0x0F,\
		0x28,0x0F,0x64,0x4B,0x03,0x05,0x00,0x00,0xEF,0x03,\
		0x01,0x01,0x08,0x16,0x1B,0x1E,0x14,0x89,0x29,0x0A,\
		0x3E,0x40,0xD3,0x07,0x00,0x0A,0x00,0x9A,0x33,0x11,\
		0x00,0x01,0x2A,0x44,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x2B,0x1C,0x64,0x94,0xC5,0x04,0x00,0x0C,0x00,0x04,\
		0xE9,0x20,0x00,0xB9,0x29,0x00,0x93,0x35,0x00,0x76,\
		0x44,0x00,0x5F,0x58,0x00,0x5F,0x10,0x30,0x48,0x00,\
		0xF5,0x4A,0x3A,0xFF,0xFF,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x04,0x11,\
		0x50,0x28,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x0C,\
		0x0E,0x1D,0x1E,0x1F,0x20,0x22,0x24,0x28,0x29,0x2A,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0xF8,0x01\
	}
	
	// TODO: define your config for Sensor_ID == 2 here, if needed
	//008692_中兴_Z752C_超声_GT9147_1040_V65_02_Config_20141103
	//008692_中兴_Z752C_超声_02_GT9147_1073_V42_Config_20141210
	//008692_中兴_Z752C_超声_02_GT9147_1073_V43_Config_20141219
	//解决边缘问题
	//008692_中兴_Z752C_超声_02_GT9147_1073_V44_Config_20141224
	//解决按键问题
#define CTP_CFG_GROUP3 {\
		0x44,0xE0,0x01,0x20,0x03,0x05,0xB5,0xC1,0x01,0x0F,\
		0x28,0x0F,0x64,0x4B,0x03,0x05,0x00,0x00,0x00,0x02,\
		0x01,0x01,0x08,0x16,0x1B,0x1E,0x14,0x89,0x29,0x0A,\
		0x3E,0x40,0xD3,0x07,0x00,0x0A,0x00,0x9A,0x33,0x11,\
		0x00,0x01,0x2A,0x44,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x2B,0x1C,0x64,0x94,0xC5,0x04,0x00,0x0C,0x00,0x04,\
		0xE9,0x20,0x00,0xB9,0x29,0x00,0x93,0x35,0x00,0x76,\
		0x44,0x00,0x5F,0x58,0x00,0x5F,0x10,0x30,0x48,0x00,\
		0xF5,0x4A,0x3A,0xFF,0xFF,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x04,0x11,\
		0x50,0x28,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x0E,0x00,0x0C,0x02,0x0A,0x04,0x08,0x05,\
		0x06,0x1D,0x29,0x1E,0x28,0x1F,0x24,0x20,0x22,0x2A,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0xE9,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 3 here, if needed
#define CTP_CFG_GROUP4 {\
		}
	
	// TODO: define your config for Sensor_ID == 4 here, if needed
#define CTP_CFG_GROUP5 {\
		}
	
	// TODO: define your config for Sensor_ID == 5 here, if needed
#define CTP_CFG_GROUP6 {\
		}
	
	//////////////////////////////////////////////////////////////
	////  put	GT960's config here //////////////////////////////
	//////////////////////////////////////////////////////////////
	// TODO: define your config for Sensor_ID == 6 here, if needed
#define CTP_CFG_GROUP7 {\
		}
	
	// TODO: define your config for Sensor_ID == 7 here, if needed
#define CTP_CFG_GROUP8 {\
		}
	
	// TODO: define your config for Sensor_ID == 8 here, if needed
#define CTP_CFG_GROUP9 {\
		}
	
	// TODO: define your config for Sensor_ID == 9 here, if needed
#define CTP_CFG_GROUP10 {\
		}
	
	// TODO: define your config for Sensor_ID == 10 here, if needed
#define CTP_CFG_GROUP11 {\
		}
	
	// TODO: define your config for Sensor_ID == 11 here, if needed
#define CTP_CFG_GROUP12 {\
		}
#elif defined(CONFIG_BOARD_GIANT)||defined(CONFIG_BOARD_HERA)
	//(整机用)009031_中兴移动_Z791_宇顺_S80291_00_GT9147_1040_V65_Config_20141024
	//(整机用)009031_中兴移动_Z791_宇顺_S80291_00_GT9147_1070_V66_config_20141117
	//（更改分辨率）009031_中兴移动_Z791_宇顺_S80291_00_GT9147_1073_V68_config_20141212
	//（更改按键）009031_中兴移动_Z791_宇顺_S80291_00_GT9147_1073_V69_config_20141225
#define CTP_CFG_GROUP1 {\
		0x46,0xE0,0x01,0x56,0x03,0x05,0x35,0x41,0x01,0x08,\
		0x23,0x08,0x4B,0x32,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x06,0x17,0x19,0x1B,0x14,0x89,0x28,0x0A,\
		0x4A,0x4C,0x0C,0x08,0x00,0x00,0x00,0x03,0x02,0x11,\
		0x50,0x11,0x20,0x00,0x00,0x00,0x00,0x32,0x09,0x00,\
		0x00,0x2F,0x76,0x8F,0xC5,0x02,0x00,0x00,0x00,0x04,\
		0x9B,0x33,0x00,0x83,0x3E,0x00,0x71,0x4A,0x00,0x62,\
		0x59,0x50,0x55,0x6C,0x00,0x55,0x10,0x28,0x40,0x00,\
		0xF0,0x4A,0x3A,0xFF,0xFF,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x11,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0xFF,0x00,0x29,0x1E,0x28,0x1F,0x24,0x20,0x1D,0x22,\
		0x05,0x0C,0x04,0x0A,0x02,0x08,0x00,0x06,0x0E,0xFF,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x22,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0x22,0x01\
	}
	// TODO: define your config for Sensor_ID == 1 here, if needed
	//008888_中兴移动_Z792_联创_TBD_00_GT9147_1073_V66_Config_20150110
	//008888_中兴移动_Z792_联创_TBD_01_GT9147_1073_V67_Config_20150327
	//改善防水和准确度
#define CTP_CFG_GROUP2 {\
		0x43,0xE0,0x01,0x56,0x03,0x05,0x35,0x51,0x01,0x08,\
		0x28,0x05,0x55,0x37,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x13,0x18,0x20,0x14,0x89,0x28,0x0A,\
		0x32,0x34,0x46,0x08,0x00,0x00,0x00,0x01,0x03,0x11,\
		0x40,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x00,\
		0x00,0x2F,0x76,0x94,0xC5,0x02,0x00,0x00,0x00,0x04,\
		0x7D,0x33,0x00,0x6B,0x3E,0x00,0x5E,0x4A,0x00,0x52,\
		0x59,0x55,0x49,0x6C,0x00,0x49,0x10,0x28,0x40,0x00,\
		0xF5,0x4A,0x3A,0xFF,0xFF,0x17,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,\
		0xFF,0x00,0x29,0x28,0x24,0x22,0x20,0x1F,0x1D,0x1E,\
		0x0C,0x0E,0x0A,0x08,0x06,0x05,0x04,0x02,0x00,0xFF,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,\
		0xFF,0x2F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0xC5,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 2 here, if needed
	//009213_中兴移动_Z791_欧菲_5676_02_GT9147_1073_V66_Config_20141215
	//009213_中兴移动_Z791_欧菲_5676_02_GT9147_1073_V67_Config_20141225
	//009213_中兴移动_Z791_欧菲_5676_02_GT9147_1073_V68_Config_20150115
	//改善防水和其他硬件测试故障
	//009213_中兴移动_Z791_欧菲_5676_02_GT9147_1073_V69_Config_20150420
	//再次改善防水和准确度问题
#define CTP_CFG_GROUP3 {\
		0x45,0xE0,0x01,0x56,0x03,0x05,0x35,0x51,0x01,0x08,\
		0x28,0x08,0x50,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x04,0x12,0x18,0x1F,0x14,0x89,0x28,0x0A,\
		0x32,0x34,0x46,0x08,0x00,0x00,0x00,0x99,0x03,0x11,\
		0x40,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x00,\
		0x00,0x2F,0x76,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,\
		0x7D,0x33,0x00,0x6A,0x3E,0x00,0x5C,0x4A,0x00,0x50,\
		0x59,0x5A,0x46,0x6C,0x00,0x46,0x10,0x28,0x40,0x00,\
		0xF5,0x4A,0x3A,0xFF,0xFF,0x17,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x55,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,\
		0xFF,0x00,0x29,0x1D,0x28,0x1E,0x24,0x1F,0x22,0x20,\
		0x0C,0x00,0x02,0x0A,0x04,0x08,0x05,0x06,0x0E,0xFF,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,\
		0xFF,0xFF,0xFF,0xFF,0x03,0x01\
		}
	
	// TODO: define your config for Sensor_ID == 3 here, if needed
#define CTP_CFG_GROUP4 {\
		}
	
	// TODO: define your config for Sensor_ID == 4 here, if needed
#define CTP_CFG_GROUP5 {\
		}
	
	// TODO: define your config for Sensor_ID == 5 here, if needed
#define CTP_CFG_GROUP6 {\
		}
	
	//////////////////////////////////////////////////////////////
	////  put	GT960's config here //////////////////////////////
	//////////////////////////////////////////////////////////////
	// TODO: define your config for Sensor_ID == 6 here, if needed
#define CTP_CFG_GROUP7 {\
		}
	
	// TODO: define your config for Sensor_ID == 7 here, if needed
#define CTP_CFG_GROUP8 {\
		}
	
	// TODO: define your config for Sensor_ID == 8 here, if needed
#define CTP_CFG_GROUP9 {\
		}
	
	// TODO: define your config for Sensor_ID == 9 here, if needed
#define CTP_CFG_GROUP10 {\
		}
	
	// TODO: define your config for Sensor_ID == 10 here, if needed
#define CTP_CFG_GROUP11 {\
		}
	
	// TODO: define your config for Sensor_ID == 11 here, if needed
#define CTP_CFG_GROUP12 {\
		}
	
#else //demi
	//007206_中兴通讯上海_P821A16_宇顺_GT968_106C_v68_Config_20140217.cfg
#define CTP_CFG_GROUP1 {\
		0x45,0x40,0x01,0xE0,0x01,0x05,0x35,0x05,0x01,0x0A,\
		0x28,0x0A,0x50,0x28,0x03,0x05,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x12,0x17,0x21,0x14,0x87,0x07,0x0A,\
		0x32,0x00,0xB9,0x08,0x00,0x00,0x00,0x9B,0x03,0x1D,\
		0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x24,0x46,0x94,0xC5,0x04,0x05,0x00,0x00,0x04,\
		0xA6,0x26,0x00,0x90,0x2C,0x00,0x7F,0x32,0x00,0x71,\
		0x39,0x00,0x64,0x41,0x00,0x64,0x10,0x30,0x48,0x00,\
		0x07,0x40,0x30,0x99,0x99,0x27,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,\
		0x18,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x1D,\
		0x1E,0x1F,0x20,0x22,0x24,0x28,0xFF,0xFF,0xFF,0xFF,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
		0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x01\
	}
	// TODO: define your config for Sensor_ID == 1 here, if needed
#define CTP_CFG_GROUP2 {\
		}
	
	// TODO: define your config for Sensor_ID == 2 here, if needed
#define CTP_CFG_GROUP3 {\
		}
	
	// TODO: define your config for Sensor_ID == 3 here, if needed
#define CTP_CFG_GROUP4 {\
		}
	
	// TODO: define your config for Sensor_ID == 4 here, if needed
#define CTP_CFG_GROUP5 {\
		}
	
	// TODO: define your config for Sensor_ID == 5 here, if needed
#define CTP_CFG_GROUP6 {\
		}
	
	//////////////////////////////////////////////////////////////
	////  put	GT960's config here //////////////////////////////
	//////////////////////////////////////////////////////////////
	// TODO: define your config for Sensor_ID == 6 here, if needed
#define CTP_CFG_GROUP7 {\
		}
	
	// TODO: define your config for Sensor_ID == 7 here, if needed
#define CTP_CFG_GROUP8 {\
		}
	
	// TODO: define your config for Sensor_ID == 8 here, if needed
#define CTP_CFG_GROUP9 {\
		}
	
	// TODO: define your config for Sensor_ID == 9 here, if needed
#define CTP_CFG_GROUP10 {\
		}
	
	// TODO: define your config for Sensor_ID == 10 here, if needed
#define CTP_CFG_GROUP11 {\
		}
	
	// TODO: define your config for Sensor_ID == 11 here, if needed
#define CTP_CFG_GROUP12 {\
		}
	
#endif
// STEP_2(REQUIRED): Customize your I/O ports & I/O operations
//#define GTP_RST_PORT    12//S5PV210_GPJ3(6)
//#define GTP_INT_PORT    13//S5PV210_GPH1(3)
//#define GTP_INT_IRQ     gpio_to_irq(GTP_INT_PORT)
//#define GTP_INT_CFG     S3C_GPIO_SFN(0xF)


#define GTP_GPIO_AS_INPUT(pin)          do{\
                                            gpio_direction_input(pin);\
											gpio_tlmm_config(GPIO_CFG(pin, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA), GPIO_CFG_ENABLE);\
                                        }while(0)
#define GTP_GPIO_AS_INT(pin)            do{\
                                            GTP_GPIO_AS_INPUT(pin);\
											gpio_tlmm_config(GPIO_CFG(pin, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);\
                                        }while(0)
#define GTP_GPIO_GET_VALUE(pin)         gpio_get_value(pin)
#define GTP_GPIO_OUTPUT(pin,level)      gpio_direction_output(pin,level)
#define GTP_GPIO_REQUEST(pin, label)    gpio_request(pin, label)
#define GTP_GPIO_FREE(pin)              gpio_free(pin)
#define GTP_IRQ_TAB                     {IRQ_TYPE_EDGE_RISING, IRQ_TYPE_EDGE_FALLING, IRQ_TYPE_LEVEL_LOW, IRQ_TYPE_LEVEL_HIGH}

// STEP_3(optional): Specify your special config info if needed
#if GTP_CUSTOM_CFG
#if defined(CONFIG_BOARD_EROS)||defined(CONFIG_BOARD_FAERIE)||defined(CONFIG_BOARD_IRIS)
#define GTP_MAX_HEIGHT	 800
#define GTP_MAX_WIDTH	 480
#define GTP_INT_TRIGGER  1			  // 0: Rising 1: Falling
#elif defined(CONFIG_BOARD_GIANT)||defined(CONFIG_BOARD_HERA)
#define GTP_MAX_HEIGHT	 854
#define GTP_MAX_WIDTH	 480
#define GTP_INT_TRIGGER  1			  // 0: Rising 1: Falling

#else
  #define GTP_MAX_HEIGHT   480
  #define GTP_MAX_WIDTH    320
  #define GTP_INT_TRIGGER  1            // 0: Rising 1: Falling
#endif
#else
  #define GTP_MAX_HEIGHT   4096
  #define GTP_MAX_WIDTH    4096
  #define GTP_INT_TRIGGER  1
#endif

#define GTP_MAX_TOUCH         5

// STEP_4(optional): If keys are available and reported as keys, config your key info here                             
#if GTP_HAVE_TOUCH_KEY
    #define GTP_KEY_TAB  {KEY_BACK, KEY_HOME, KEY_MENU}
#endif

//***************************PART3:OTHER define*********************************
#define GTP_DRIVER_VERSION          "V2.2<2014/01/14>"
#define GTP_I2C_NAME                "Goodix-TS"
#define GT91XX_CONFIG_PROC_FILE     "gt9xx_config"
#define GTP_POLL_TIME         10    
#define GTP_ADDR_LENGTH       2
#define GTP_CONFIG_MIN_LENGTH 186
#define GTP_CONFIG_MAX_LENGTH 240
#define FAIL                  0
#define SUCCESS_OK               1
#define SWITCH_OFF            0
#define SWITCH_ON             1

//******************** For GT9XXF Start **********************//
#define GTP_REG_BAK_REF                 0x99D0
#define GTP_REG_MAIN_CLK                0x8020
#define GTP_REG_CHIP_TYPE               0x8000
#define GTP_REG_HAVE_KEY                0x804E
#define GTP_REG_MATRIX_DRVNUM           0x8069     
#define GTP_REG_MATRIX_SENNUM           0x806A

#define GTP_FL_FW_BURN              0x00
#define GTP_FL_ESD_RECOVERY         0x01
#define GTP_FL_READ_REPAIR          0x02

#define GTP_BAK_REF_SEND                0
#define GTP_BAK_REF_STORE               1
#define CFG_LOC_DRVA_NUM                29
#define CFG_LOC_DRVB_NUM                30
#define CFG_LOC_SENS_NUM                31

#define GTP_CHK_FW_MAX                  40
#define GTP_CHK_FS_MNT_MAX              300
#define GTP_BAK_REF_PATH                "/data/gtp_ref.bin"
#define GTP_MAIN_CLK_PATH               "/data/gtp_clk.bin"
#define GTP_RQST_CONFIG                 0x01
#define GTP_RQST_BAK_REF                0x02
#define GTP_RQST_RESET                  0x03
#define GTP_RQST_MAIN_CLOCK             0x04
#define GTP_RQST_RESPONDED              0x00
#define GTP_RQST_IDLE                   0xFF

//******************** For GT9XXF End **********************//
// Registers define
#define GTP_READ_COOR_ADDR    0x814E
#define GTP_REG_SLEEP         0x8040
#define GTP_REG_SENSOR_ID     0x814A
#define GTP_REG_CONFIG_DATA   0x8047
#define GTP_REG_VERSION       0x8140

#define RESOLUTION_LOC        3
#define TRIGGER_LOC           8

#define CFG_GROUP_LEN(p_cfg_grp)  (sizeof(p_cfg_grp) / sizeof(p_cfg_grp[0]))
// Log define
#define GTP_INFO(fmt,arg...)           printk("<<-GTP-INFO->> "fmt"\n",##arg)
#define GTP_ERROR(fmt,arg...)          printk("<<-GTP-ERROR->> "fmt"\n",##arg)
#define GTP_DEBUG(fmt,arg...)          do{\
                                         if(GTP_DEBUG_ON)\
                                         printk("<<-GTP-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define GTP_DEBUG_ARRAY(array, num)    do{\
                                         s32 i;\
                                         u8* a = array;\
                                         if(GTP_DEBUG_ARRAY_ON)\
                                         {\
                                            printk("<<-GTP-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printk("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printk("\n");\
                                                }\
                                            }\
                                            printk("\n");\
                                        }\
                                       }while(0)
#define GTP_DEBUG_FUNC()               do{\
                                         if(GTP_DEBUG_FUNC_ON)\
                                         printk("<<-GTP-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)
#define GTP_SWAP(x, y)                 do{\
                                         typeof(x) z = x;\
                                         x = y;\
                                         y = z;\
                                       }while (0)

//*****************************End of Part III********************************

#endif /* _GOODIX_GT9XX_H_ */

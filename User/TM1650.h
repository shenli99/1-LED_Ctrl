#ifndef __TM1650_H__
#define __TM1650_H__

#include "common.h"
#include "Soft_I2C.h"

// TM1650地址
#define TM1650_ADDR 0x24

// 亮度设置
#define TM1650_BRIGHTNESS_8 0x70  // 最亮
#define TM1650_BRIGHTNESS_7 0x60
#define TM1650_BRIGHTNESS_6 0x50
#define TM1650_BRIGHTNESS_4 0x30
#define TM1650_BRIGHTNESS_3 0x20
#define TM1650_BRIGHTNESS_2 0x10
#define TM1650_BRIGHTNESS_2 0x10
#define TM1650_BRIGHTNESS_1 0x00  // 最暗

// 函数声明
void TM1650_Init(u8 brightness);
void TM1650_Write(u8 addr, u8 dat);
void TM1650_Display_Time(u8 minute, u8 second, bit dot_flag);
void TM1650_Clear(void);
void TM1650_Display_Zero(void);

#endif /* __TM1650_H__ */
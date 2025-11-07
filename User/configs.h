#ifndef _CONFIGS_H_
#define _CONFIGS_H_

#include "Type_def.h"
#include "STC8H.h"

// 风扇控制引脚定义
sbit FAN_H = P2^0;
sbit FAN_L = P2^1;

// DS18B20 引脚定义
sbit DS18B20 = P5^4;

// 数码管
sbit SCL = P1^5;
sbit SDA = P1^4;

// KEY 引脚定义
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;

// LED 引脚定义
sbit LED_G = P0^0;
sbit LED_R = P0^1;
sbit LED_B = P0^2;

#define MAIN_Fosc		24000000L	//定义主时钟

#define MAX_TASK_NUM		10		//定义最大任务数

#endif // !_CONFIGS_H_
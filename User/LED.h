#ifndef __LED_H__
#define __LED_H__

#include "common.h"

// LED状态定义
#define LED_STATE_RUN    0
#define LED_STATE_STOP   1
#define LED_STATE_AUTO  2

// 函数声明
void LED_Init(void);

void LED_Green_On(void);
void LED_Green_Off(void);
void LED_Red_On(void);
void LED_Red_Off(void);
void LED_Blue_On(void);
void LED_Blue_Off(void);

void LED_Update(u8 state);

#endif /* __LED_H__ */
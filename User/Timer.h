#ifndef __TIMER_H__
#define __TIMER_H__

#include "common.h"

typedef void (*TimerCallback)(void);

// 函数声明
void TimerInit(void);
void Timer0_SetCallback(TimerCallback callback);

#endif /* __TIMER_H__ */
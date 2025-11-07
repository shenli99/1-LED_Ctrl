#ifndef _USERFANCONTROL_H_
#define _USERFANCONTROL_H_

#include "common.h"
#include "timer.h"
#include "Key.h"
#include "Task.h"
#include "Led.h"
#include "TM1650.h"
#include "Fan.h"

typedef enum
{
    manual_mode = 0,
    auto_mode,
} FanControlState;

void UserFanControl_Init(void);
void UserFanControl_Loop(void);

#endif
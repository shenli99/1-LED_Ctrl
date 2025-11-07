#ifndef __KEY_H__
#define __KEY_H__

#include "common.h"

#define KEY_NONE 0
#define KEY_PRESSED 1
#define KEY_CAPTURE 2

// 函数声明
void Key_Init(void);
void Key_Scan(void);
u8 Key_GetPressed();

#endif /* __KEY_H__ */
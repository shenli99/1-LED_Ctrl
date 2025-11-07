#ifndef __KEY_H__
#define __KEY_H__

#include "common.h"

#define KEY_NONE 0
#define KEY_PRESSED 1
#define KEY_CAPTURE 2

typedef enum {
    NONE = 0,
    KEY1_ONLY = 1,
    KEY2_ONLY = 2,
    KEY1_AND_KEY2 = 3,
} Key_res;

// 函数声明
void Key_Init(void);
void Key_Scan(void);
Key_res Key_GetPressed();

#endif /* __KEY_H__ */
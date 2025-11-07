#include "LED.h"

// LED初始化 - 共阳极，低电平点亮
void LED_Init(void)
{
    // 配置为推挽
    P0M1 &= ~0x07;
    P0M0 |= 0x07;
    P0 = 0xFF;
    
    // 共阳极LED：初始高电平熄灭
    LED_G = 1;
    LED_R = 1;
    LED_B = 1;
}

// 简化LED_Update函数，移除不必要的延时
void LED_Update(u8 state)
{
    // 先关闭所有LED
    LED_G = 1;
    LED_R = 1;
    LED_B = 1;
    
    // 根据状态点亮对应LED
    switch(state)
    {
        case LED_STATE_RUN:
            LED_G = 0;
            break;
        case LED_STATE_STOP:
            LED_R = 0;
            break;
        case LED_STATE_RESET:
            LED_B = 0;
            break;
        default:
            break;
    }
}


void LED_Green_On(void)
{
    LED_G = 0;  // 低电平点亮
}

void LED_Green_Off(void)
{
    LED_G = 1;  // 高电平熄灭
}

void LED_Red_On(void)
{
    LED_R = 0;  // 低电平点亮
}

void LED_Red_Off(void)
{
    LED_R = 1;  // 高电平熄灭
}

void LED_Blue_On(void)
{
    LED_B = 0;  // 低电平点亮
}

void LED_Blue_Off(void)
{
    LED_B = 1;  // 高电平熄灭
}
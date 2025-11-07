#include "Key.h"

// 按键计时变量
u8 key1_buffer = 0;
u8 key2_buffer = 0;

// 按键状态变量
u8 key1_state = KEY_NONE;
u8 key2_state = KEY_NONE;

// 按键初始化函数
void Key_Init(void)
{
    // 上拉电阻配置（按下接地）
    // P3.2和P3.3设置为输入模式，启用上拉电阻
    P3M1 &= ~0x0C;  // 清零P3.2和P3.3的模式位
    P3M0 &= ~0x0C;  // 设置为准双向口模式（默认上拉）
    
    // 使用内部上拉
    P3PU |= 0x0C;  // 设置P3.2和P3.3为上拉模式
    
    // 注册按键扫描任务
    // Task_Register(TASK_10MS, Key_Scan);
}

///按键扫描任务函数
void Key_Scan(void)
{
    if (KEY1 == 0 && key1_state == KEY_NONE)
    {
        key1_state = KEY_PRESSED;
    }else if (key1_state == KEY_PRESSED && KEY1 == 1 && (key1_buffer & 0x07) == 0x00)
    {
        key1_state = KEY_CAPTURE;
    }

    if (KEY2 == 0 && key1_state == KEY_NONE)
    {
        key2_state = KEY_PRESSED;
    }else if (key2_state == KEY_PRESSED && KEY1 == 1 && (key2_buffer & 0x07) == 0x00)
    {
        key2_state = KEY_CAPTURE;
    }
    
    key1_buffer = (key1_buffer << 1) | KEY1;
    key2_buffer = (key2_buffer << 1) | KEY2;
}

// 获取按键按下状态函数
u8 Key_GetPressed()
{
    if (key1_state == KEY_CAPTURE)
    {
        key1_state = KEY_NONE;
        return 1;
    }
    else if (key2_state == KEY_CAPTURE)
    {
        key2_state = KEY_NONE;
        return 2;
    }
    else
    {
        return 0;
    }
}
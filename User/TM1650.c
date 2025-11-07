#include "TM1650.h"

// 共阴数码管段码表（0-9）
u8 code SEG_TAB[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

u8 TM1650_state[] = {0x00, 0x00, 0x00, 0x00}; 

// TM1650初始化
void TM1650_Init(u8 brightness)
{
    u16 i;
    u8 ctrl_cmd;
    
    I2C_Init();
    
    I2C_delay();
    
    // 清除显示
    TM1650_Clear();
    
    // 短暂延时
    for(i = 0; i < 100; i++) _nop_();
    
    // TM1650控制命令：正确的控制命令格式
    ctrl_cmd = 0x01; 
    
    SI2C_WriteByteNoDev(0x48, ctrl_cmd | brightness);

    // 短暂延时
    I2C_delay();
    
    // 显示初始化图案以确认正常工作
    TM1650_Display_Zero();
}

// 显示时间 - 格式为 MM:SS
void TM1650_Display_Time(u8 w0, u8 w1, u8 w2, u8 w3, u8 bit_mask)
{
    TM1650_state[0] = SEG_TAB[w0] | (bit_mask & 0x00) << 7;
    TM1650_state[1] = SEG_TAB[w1] | (bit_mask & 0x01) << 6;
    TM1650_state[2] = SEG_TAB[w2] | (bit_mask & 0x02) << 5;
    TM1650_state[3] = SEG_TAB[w3] | (bit_mask & 0x04) << 4;
    
    // 直接写入，移除多余延时
    TM1650_Write(0x68, TM1650_state[0]);
    TM1650_Write(0x6A, TM1650_state[1]);
    TM1650_Write(0x6C, TM1650_state[2]);
    TM1650_Write(0x6E, TM1650_state[3]);
}

// 向TM1650写入一个字节
void TM1650_Write(unsigned char addr, unsigned char dat)
{
    SI2C_WriteByteNoDev(addr, dat);
}

// 清除显示
void TM1650_Clear(void)
{
    TM1650_Write(0x68, 0x00);
    TM1650_Write(0x6A, 0x00);
    TM1650_Write(0x6C, 0x00);
    TM1650_Write(0x6E, 0x00);
}

// 显示00:00
void TM1650_Display_Zero(void)
{
    TM1650_Write(0x68, SEG_TAB[0]);  // 第一位显示0
    TM1650_Write(0x6A, SEG_TAB[0]);  // 第二位显示0
    TM1650_Write(0x6C, SEG_TAB[0]);  // 第三位显示0
    TM1650_Write(0x6E, SEG_TAB[0]);  // 第四位显示0
}
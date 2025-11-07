#include "STC8H.H"
#include "intrins.h"

// 引脚定义
#define FAN_HIGH P20
#define FAN_LOW P21

#define LED_GREEN P00
#define LED_RED P01
#define LED_BLUE P02

#define KEY1 P32
#define KEY2 P33

#define DS18B20_PIN P54

#define TM1650_SCL P15
#define TM1650_SDA P14

// 模式定义
#define MANUAL_MODE 0
#define AUTO_MODE 1

// 全局变量
unsigned char g_mode = MANUAL_MODE;      // 当前模式
unsigned char g_fan_state = 0;           // 风扇状态 0:关 1:开
unsigned char g_threshold = 26;          // 温度阈值
unsigned char g_current_temp = 0;        // 当前温度

unsigned int g_key_timer = 0;            // 按键计时器
unsigned int g_temp_timer = 0;           // 温度计时器
unsigned int g_display_timer = 0;        // 显示计时器
unsigned int g_led_timer = 0;            // LED计时器

// TM1650数码管相关
unsigned char code digit_table[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x00  // 空
};

// 软件IIC延时
void iic_delay(void)
{
    _nop_(); _nop_(); _nop_(); _nop_();
}

// TM1650起始信号
void tm1650_start(void)
{
    TM1650_SDA = 1;
    TM1650_SCL = 1;
    iic_delay();
    TM1650_SDA = 0;
    iic_delay();
    TM1650_SCL = 0;
}

// TM1650停止信号
void tm1650_stop(void)
{
    TM1650_SDA = 0;
    TM1650_SCL = 1;
    iic_delay();
    TM1650_SDA = 1;
    iic_delay();
}

// TM1650发送字节
void tm1650_send_byte(unsigned char dat)
{
    unsigned char i;
    
    for(i = 0; i < 8; i++)
    {
        TM1650_SCL = 0;
        iic_delay();
        if(dat & 0x01)
            TM1650_SDA = 1;
        else
            TM1650_SDA = 0;
        iic_delay();
        TM1650_SCL = 1;
        iic_delay();
        dat >>= 1;
    }
    
    TM1650_SCL = 0;
    iic_delay();
    TM1650_SDA = 1;
    iic_delay();
    TM1650_SCL = 1;
    iic_delay();
    TM1650_SCL = 0;
}

// TM1650显示数字
void tm1650_display(unsigned char addr, unsigned char dat)
{
    tm1650_start();
    tm1650_send_byte(addr);
    tm1650_send_byte(dat);
    tm1650_stop();
}

// TM1650初始化
void tm1650_init(void)
{
    // 设置显示模式：4位7段数码管，亮度3
    tm1650_start();
    tm1650_send_byte(0x48); // 显示模式命令
    tm1650_stop();
    
    // 清除显示
    tm1650_display(0x68, 0x00); // 第1位
    tm1650_display(0x6A, 0x00); // 第2位
    tm1650_display(0x6C, 0x00); // 第3位
    tm1650_display(0x6E, 0x00); // 第4位
}

// DS18B20延时函数
void ds18b20_delay(unsigned int t)
{
    while(t--);
}

// DS18B20初始化
unsigned char ds18b20_init(void)
{
    unsigned char flag;
    
    DS18B20_PIN = 1;
    ds18b20_delay(8);
    DS18B20_PIN = 0;
    ds18b20_delay(80);
    DS18B20_PIN = 1;
    ds18b20_delay(14);
    flag = DS18B20_PIN;
    ds18b20_delay(20);
    
    return flag;
}

// DS18B20写一个字节
void ds18b20_write_byte(unsigned char dat)
{
    unsigned char i;
    
    for(i = 0; i < 8; i++)
    {
        DS18B20_PIN = 0;
        _nop_(); _nop_();
        DS18B20_PIN = dat & 0x01;
        ds18b20_delay(5);
        DS18B20_PIN = 1;
        dat >>= 1;
    }
}

// DS18B20读一个字节
unsigned char ds18b20_read_byte(void)
{
    unsigned char i, dat = 0;
    
    for(i = 0; i < 8; i++)
    {
        dat >>= 1;
        DS18B20_PIN = 0;
        _nop_(); _nop_();
        DS18B20_PIN = 1;
        _nop_(); _nop_();
        if(DS18B20_PIN)
            dat |= 0x80;
        ds18b20_delay(5);
    }
    
    return dat;
}

// 读取温度
unsigned char ds18b20_read_temp(void)
{
    unsigned char temp_l, temp_h;
    unsigned int temp;
    
    if(ds18b20_init() == 0)
    {
        ds18b20_write_byte(0xCC); // 跳过ROM
        ds18b20_write_byte(0x44); // 开始转换
        ds18b20_delay(100);
        
        ds18b20_init();
        ds18b20_write_byte(0xCC); // 跳过ROM
        ds18b20_write_byte(0xBE); // 读温度
        temp_l = ds18b20_read_byte();
        temp_h = ds18b20_read_byte();
        
        temp = temp_h;
        temp <<= 8;
        temp |= temp_l;
        temp = temp * 0.0625; // 转换温度值
        
        return (unsigned char)temp;
    }
    
    return 0;
}

// 风扇控制
void fan_control(unsigned char state)
{
    if(state)
    {
        FAN_HIGH = 1;
        FAN_LOW = 0;
        g_fan_state = 1;
    }
    else
    {
        FAN_HIGH = 0;
        FAN_LOW = 0;
        g_fan_state = 0;
    }
}

// LED控制
void led_control(void)
{
    // 关闭所有LED
    LED_GREEN = 1;
    LED_RED = 1;
    LED_BLUE = 1;
    
    if(g_mode == MANUAL_MODE)
    {
        if(g_fan_state)
            LED_GREEN = 0; // 手动模式风扇开，绿灯亮
        else
            LED_RED = 0;   // 手动模式风扇关，红灯亮
    }
    else // AUTO_MODE
    {
        LED_BLUE = 0;      // 自动模式，蓝灯亮
    }
}

// 按键处理
void key_process(void)
{
    static unsigned char key1_last = 1, key2_last = 1;
    static unsigned int key1_press_time = 0, key2_press_time = 0;
    
    unsigned char key1_current = KEY1;
    unsigned char key2_current = KEY2;
    
    // KEY1处理
    if(key1_current == 0 && key1_last == 1) // 按键按下
    {
        key1_press_time = 0;
    }
    else if(key1_current == 0) // 按键保持按下
    {
        key1_press_time++;
        if(key1_press_time >= 5) // 50ms消抖
        {
            if(g_mode == MANUAL_MODE)
            {
                fan_control(1); // 手动模式开风扇
            }
            else // AUTO_MODE
            {
                if(g_threshold < 36)
                    g_threshold++; // 自动模式增加阈值
            }
            key1_press_time = 4; // 防止连续触发
        }
    }
    
    // KEY2处理
    if(key2_current == 0 && key2_last == 1) // 按键按下
    {
        key2_press_time = 0;
    }
    else if(key2_current == 0) // 按键保持按下
    {
        key2_press_time++;
        if(key2_press_time >= 5) // 50ms消抖
        {
            if(g_mode == MANUAL_MODE)
            {
                fan_control(0); // 手动模式关风扇
            }
            else // AUTO_MODE
            {
                if(g_threshold > 16)
                    g_threshold--; // 自动模式减少阈值
            }
            key2_press_time = 4; // 防止连续触发
        }
    }
    
    // 同时按键检测（模式切换）
    if(key1_current == 0 && key2_current == 0)
    {
        static unsigned char both_pressed = 0;
        static unsigned int both_press_time = 0;
        
        if(!both_pressed)
        {
            both_press_time++;
            if(both_press_time >= 10) // 100ms消抖
            {
                // 切换模式
                if(g_mode == MANUAL_MODE)
                    g_mode = AUTO_MODE;
                else
                    g_mode = MANUAL_MODE;
                
                // 自动模式切换到手动模式时关闭风扇
                if(g_mode == MANUAL_MODE)
                    fan_control(0);
                
                both_pressed = 1;
                both_press_time = 0;
            }
        }
    }
   // else
   // {
     //   both_pressed = 0;
       // both_press_time = 0;
    //}
    
    key1_last = key1_current;
    key2_last = key2_current;
}

// 温度控制
void temp_control(void)
{
    if(g_mode == AUTO_MODE)
    {
        if(g_current_temp > g_threshold)
            fan_control(1); // 温度高于阈值，开风扇
        else
            fan_control(0); // 温度低于阈值，关风扇
    }
}

// 显示更新
void display_update(void)
{
    if(g_mode == MANUAL_MODE)
    {
        // 高两位显示当前温度
        tm1650_display(0x68, digit_table[g_current_temp / 10]);        // 十位
        tm1650_display(0x6A, digit_table[g_current_temp % 10]);        // 个位
        
        // 低两位显示风扇状态
        if(g_fan_state)
        {
            tm1650_display(0x6C, digit_table[0]);                      // 0
            tm1650_display(0x6E, digit_table[1]);                      // 1
        }
        else
        {
            tm1650_display(0x6C, digit_table[0]);                      // 0
            tm1650_display(0x6E, digit_table[0]);                      // 0
        }
    }
    else // AUTO_MODE
    {
        // 高两位显示当前温度
        tm1650_display(0x68, digit_table[g_current_temp / 10]);        // 十位
        tm1650_display(0x6A, digit_table[g_current_temp % 10]);        // 个位
        
        // 低两位显示阈值
        tm1650_display(0x6C, digit_table[g_threshold / 10]);           // 十位
        tm1650_display(0x6E, digit_table[g_threshold % 10]);           // 个位
    }
}

// 定时器0初始化
void timer0_init(void)
{
    AUXR |= 0x80;        // 定时器0为1T模式
    TMOD &= 0xF0;        // 设置定时器模式
    TL0 = 0xCD;          // 设置定时初值
    TH0 = 0xD4;          // 设置定时初值，10ms
    TF0 = 0;             // 清除TF0标志
    TR0 = 1;             // 定时器0开始计时
    ET0 = 1;             // 使能定时器0中断
    EA = 1;              // 开启总中断
}

// 定时器0中断服务函数
void timer0_isr() interrupt 1
{
    g_key_timer++;
    g_temp_timer++;
    g_display_timer++;
    g_led_timer++;
}

// 主任务调度
void main_schedule(void)
{
    // 按键处理任务（100ms）
    if(g_key_timer >= 10)
    {
        g_key_timer = 0;
        key_process();
    }
    
    // 温度控制任务（500ms）
    if(g_temp_timer >= 50)
    {
        g_temp_timer = 0;
        g_current_temp = ds18b20_read_temp();
        temp_control();
    }
    
    // 显示更新任务（1000ms）
    if(g_display_timer >= 100)
    {
        g_display_timer = 0;
        display_update();
    }
    
    // LED更新任务（200ms）
    if(g_led_timer >= 20)
    {
        g_led_timer = 0;
        led_control();
    }
}

// 初始化函数
void system_init(void)
{
    // 设置IO模式
    P0M0 = 0x00; P0M1 = 0x00; // P0为准双向口
    P2M0 = 0x00; P2M1 = 0x00; // P2为准双向口
    P3M0 = 0x00; P3M1 = 0x00; // P3为准双向口
    P1M0 = 0x00; P1M1 = 0x00; // P1为准双向口
    P5M0 = 0x00; P5M1 = 0x00; // P5为准双向口
    
    // 初始化外设
    tm1650_init();
    timer0_init();
    
    // 初始状态
    fan_control(0); // 关闭风扇
    led_control();  // 更新LED状态
}

// 主函数
void main(void)
{
    system_init();
    
    while(1)
    {
        main_schedule();
    }
}
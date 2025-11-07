#include "Timer.h"


// 定时器0初始化（1ms中断）
void Timer0_Init(void)		//定时器0初始化，2秒@40.000MHz
{
	TM0PS = 0x0;			//设置定时器时钟预分频
    AUXR |= 0x80;            //定时器0工作在1T模式
    // 定时间隔 = 1 / SysCLK * (65536 - [TL0:TH0]) * (TMPOS+1) * 2 s
    // 假设 SysCLK = 24MHz, 则 1ms 时 [TL0:TH0] = 25536 = 0xD120
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
    EA = 1;
}

// 定时器0中断服务函数
void Timer0_ISR(void) interrupt TMR0_VECTOR
{
    // 清除中断标志
    TF0 = 0;

    // Task_1ms_Count();
}
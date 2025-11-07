/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include	"Soft_I2C.h"

//========================================================================
// 函数: void I2C_Delay(void)
// 描述: I2C延时函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void I2C_Delay(void) //for normal MCS51, delay (2 * dly + 4) T, for STC12Cxxxx delay (4 * dly + 10) T
{
	u8  dly;
	dly = MAIN_Fosc / 2000000UL;        //按2us计算
	while(--dly);
}

// IIC初始化函数
void I2C_Init(void)
{
	P1M1 |= 0x30;   // 设置P1.4和P1.5为开漏模式
    P1M0 |= 0x30;  // 清零对应位
    P1PU &= ~0x30;  // 清零对应位
	I2C_Delay();
}

//========================================================================
// I2C总线函数
//========================================================================
void I2C_Start(void)         //start the I2C, SDA High-to-low when SCL is high
{
	SDA = 1;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SDA = 0;
	I2C_Delay();
	SCL = 0;
	I2C_Delay();
}       


void I2C_Stop(void)           //STOP the I2C, SDA Low-to-high when SCL is high
{
	SDA = 0;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SDA = 1;
	I2C_Delay();
}

void S_ACK(void)              //Send ACK (LOW)
{
	SDA = 0;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SCL = 0;
	I2C_Delay();
}

void S_NoACK(void)           //Send No ACK (High)
{
	SDA = 1;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SCL = 0;
	I2C_Delay();
}
        
void I2C_Check_ACK(void)     //Check ACK, If F0=0, then right, if F0=1, then error
{
	SDA = 1;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	F0  = SDA;
	SCL = 0;
	I2C_Delay();
}

//========================================================================
// 函数: void I2C_WriteAbyte(u8 dat)
// 描述: I2C发送一个字节数据函数.
// 参数: 发送的数据.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void I2C_WriteAbyte(u8 dat)     //write a byte to I2C
{
	u8 i;
	i = 8;
	do
	{
		if(dat & 0x80)  SDA = 1;
		else            SDA = 0;
		dat <<= 1;
		I2C_Delay();
		SCL = 1;
		I2C_Delay();
		SCL = 0;
		I2C_Delay();
	}
	while(--i);
}

// IIC发送函数，无设备地址
int SI2C_WriteByteNoDev(u8 mem_addr, u8 dat) 
{
	int i = 0;
	I2C_Start();
	I2C_WriteAbyte(mem_addr);
	I2C_Check_ACK();
	if(!F0)
	{
		I2C_WriteAbyte(dat);
		I2C_Check_ACK();
		if(F0) i = -1;
	}
	I2C_Stop();
	return i;
}

#include "DS18B20.h"

static void _Delay(u16 n)
{
	u16 dly = 0;
    for (; n > 0; n--){
        dly = MAIN_Fosc / 4000000UL;
	    while(--dly);
    }
}

void DS18D20_PullUp(void) {
    P5PU |= 0x10; 
}

void DS18D20_OutPut(void) {
    P5M0 |= 0x10;
    P5M1 |= 0x10; 
}

void DS18D20_InPut(void) {
    P5M0 &= ~0x10;
    P5M1 |= 0x10; 
}

void DS18D20_Init(void) {
    DS18D20_PullUp();
    DS18D20_OutPut();

    DS18B20 = 1;

    _Delay(100);
    DS18B20 = 0;

    _Delay(800);
    DS18B20 = 1;

    while (DS18B20 == 1);
    while (DS18B20 == 0);
}

void DS18B20_Write(u8 dat) {
    u8 i = 0;
    for (i = 0; i < 8; i++) {
        DS18B20 = 0;
        _Delay(2);
        DS18B20 = (dat & 0x01);
        _Delay(25);
        DS18B20 = 1;
        dat >>= 1;
        _Delay(5);
    }
    _Delay(5);
}

u8 DS18B20_Read(void) {
    u8 i = 0, dat = 0;
    for (i = 0; i < 8; i++) {
        DS18B20 = 0;
        dat >>= 1;
        DS18B20 = 1;
        _Delay(2);
        if (DS18B20 == 1) {
            dat |= 0x80;
        }
    }
    return dat;
}

int16 DS18B20_GetTemp(void) {
    u8 a = 0, b = 0;
    u16 t = 0;
    int8 temp = 0;
    DS18B20_Write(0xCC);
    DS18B20_Write(0x44);

    DS18B20_Write(0xCC);
    DS18B20_Write(0xBE);
    a = DS18B20_Read();
    b = DS18B20_Read();

    t = (b << 8) | a;
    if (t & 0xF800 == 0)
    {
        temp = t & 0x07FF;
        temp = temp * 0.0625;
        return temp;
    }else{
        temp = ~(t & 0x07FF);
        temp = (temp + 1) * 0.0625;
        return temp;
    }
}
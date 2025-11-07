#include "DS18B20.h"

void Delay(u16 n)
{
	u16 dly = 0;
    for (; n > 0; n--){
        dly = MAIN_Fosc / 4000000UL;
	    while(--dly);
    }
}

void DS18D20_PullUp() {
    P5PU |= 0x10; 
}

void DS18D20_OutPut() {
    P5M0 |= 0x10;
    P5M1 |= 0x10; 
}

void DS18D20_InPut() {
    P5M0 &= ~0x10;
    P5M1 |= 0x10; 
}

void DS18D20_Init() {
    DS18D20_PullUp();
    DS18D20_OutPut();

    DS18B20 = 1;

    Delay(100);
    DS18B20 = 0;

    Delay(800);
    DS18B20 = 1;

    while (DS18B20 == 1);
    while (DS18B20 == 0);
}

void DS18B20_Write(u8 dat) {
    u8 i = 0;
    for (i = 0; i < 8; i++) {
        DS18B20 = 0;
        Delay(2);
        DS18B20 = (dat & 0x01);
        Delay(25);
        DS18B20 = 1;
        dat >>= 1;
        Delay(5);
    }
    Delay(5);
}

u8 DS18B20_Read(void) {
    u8 i = 0, dat = 0;
    for (i = 0; i < 8; i++) {
        DS18B20 = 0;
        dat >>= 1;
        DS18B20 = 1;
        Delay(2);
        if (DS18B20 == 1) {
            dat |= 0x80;
        }
    }
    return dat;
}
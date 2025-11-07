#ifndef _DS18D20_H_
#define _DS18D20_H_

#include "common.h"

void DS18B20_Write(u8 dat);
u8 DS18B20_Read(void);
int16 DS18B20_GetTemp(void);

#endif // !_DS18D20_H_
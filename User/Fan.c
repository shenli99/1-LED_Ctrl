#include "Fan.h"

void Fan_Init(void) {
    P2M0 |= 0x03;
    P2M1 = (P2M1 & ~0x01) | 0x02; 

    FAN_H = 0;
    FAN_L = 0;
}

void Fan_On(void) {
    FAN_H = 1;
    FAN_L = 0;
}

void Fan_Off(void) {
    FAN_H = 0;
    FAN_L = 0;
}
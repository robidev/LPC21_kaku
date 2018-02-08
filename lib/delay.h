/*
    ARM IP interface

    delay.h:
            vertragings functies testboard

*/

#ifndef DELAY_H
#define DELAY_H

#define DELAY_US    PLL_MUL/2
#define DELAY_MS    PLL_MUL*400
#define DELAY_S     PLL_MUL*400000

/*******************************************************************************
  Volgende code is niet bedoeld om aan te passen
*/

#include <config.h>

void delay_us(volatile unsigned int time);
void delay_ms(volatile unsigned int time);
void delay_s(volatile unsigned int time);

#endif /* DELAY_H */

/*
    ARM IP interface

    buzzer.h:
             piepjes testboard

*/

#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_PIN 8
#define BEEPPULS PLL_MUL*166
#define BEEPTIJD PLL_MUL*33

/*******************************************************************************
  Volgende code is niet bedoeld om aan te passen
*/

#include <config.h>

void buzzer_init(void);
void buzzer_put(unsigned int time);
void beep(void);

#endif /* BUZZER_H */

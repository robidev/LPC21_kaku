/*
    ARM IP interface

    timer.h:
            timer-code testboard

*/

#ifndef TIMER_H
#define TIMER_H

#include <config.h>

void timer0_init(unsigned int interval,unsigned int prescaler,unsigned int function);
void timer1_init(unsigned int interval,unsigned int prescaler,unsigned int function);


#endif /* TIMER_H */

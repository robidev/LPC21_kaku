/*
    ARM IP interface

    buzzer.c:
             piepjes testboard

    pin-info:
           P8 - output buzzer

    opmerkingen:
                -

*/

#include "buzzer.h"

void buzzer_init(void)
/*
  Stelt gebruikte I/O pinnen in.
*/
{
     PINSEL0 &= ~(3<<(BUZZER_PIN*2));
     IODIR0   |= 1<<BUZZER_PIN;
}

void beep(void)
/*
  Piepje zonder fratsen (dat scheelt)
  Is (min of meer) onafhankelijk van gekozen frequentie
*/
{
    volatile int i,j;

    for(i=0;i<BEEPTIJD;i++) {
        for(j=0; j<BEEPPULS; j++);
        IOSET0 = 1<<BUZZER_PIN;
        for(j=0; j<BEEPPULS; j++);
        IOCLR0 = 1<<BUZZER_PIN;
    }
}


void buzzer_put(unsigned int time)
/*
  Maak een piepje met meegegeven lengte
*/
{
    volatile unsigned int i;
    volatile unsigned int k=0;
    
	while (k++<100) {
     	IOSET0= 1<<BUZZER_PIN;
		for (i=0;i<(time+k);i++);
		IOCLR0 = 1<<BUZZER_PIN;
		for (i=0;(i<time+k);i++);	
	}
}

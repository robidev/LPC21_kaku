/*
    ARM IP interface

    timer.c:
            timer-code testboard

    opmerkingen:
                -De functies __enable_interrupts() en __disable_interrupts()
                 zijn in het bestand crt0.s te vinden

*/

#include "timer.h"
#include "printf_usb.h"
#include <debug.h>

static void Timer0_intHandler(void);
static void Timer1_intHandler(void);

extern unsigned int timerFlag;

void timer1_init(unsigned int interval,unsigned int prescaler,unsigned int function)
/*
  Initialiseert timer1
*/
{
    __disable_interrupts();            
    //__store_interrupts(&__interrupt_status);

    /* timer1 instellen */
    T1TCR = 0;                                      /* reset timer */
    T1PR  = prescaler;                              /* prescaler instellen */
    T1MR0 = interval;                               /* counter instellen */
    T1MCR = 1<<1 | 1<<0;;                           /* Interrupt op MR0, en reset */
    T1IR = 0xFFFFFFFF;                              /* wis interrupt-flags */

    /* VIC instellen */
    VICIntSelect &=~ (1<<5);                        /* als IRQ */
    VICVectCntl1 = 0x20 | 5;                       /* ingeschakeld, en als timer1 */
    if(function != 0) {
        VICVectAddr1 = (unsigned int)function;          /* interrupt-afhandeler */
    }
    else {
        VICVectAddr1 = (unsigned int)Timer1_intHandler; /* interrupt-afhandeler */
    }
    VICIntEnable = 1<<5;                            /* interruptbron aan */
    VICVectAddr = 0xFF;                             /* acties doorvoeren */

    /* start timer */
    T1TCR = 1<<0;

    __enable_interrupts();
    //__restore_interrupts(&__interrupt_status);
}


void timer0_init(unsigned int interval,unsigned int prescaler, unsigned int function)
/*
  Initialiseert timer0
*/
{
    __disable_interrupts();

    /* timer0 instellen */
    T0TCR = 0;                                      /* reset timer */
    T0PR  = prescaler;                              /* prescaler instellen */
    T0MR0 = interval;                               /* counter instellen */
    T0MCR = 1<<1 | 1<<0;                            /* Interrupt op MR0, en reset */
    T0IR = 0xFFFFFFFF;                              /* wis interrupt-flags */

    /* VIC instellen */
    VICIntSelect &=~ (1<<4);                        /* als IRQ */
    VICVectCntl0 = 0x20 | 4;                       /* ingeschakeld, en als timer0 */
    if(function != 0) {
        VICVectAddr0 = (unsigned int)function;          /* interrupt-afhandeler */
    }
    else {
        VICVectAddr0 = (unsigned int)Timer0_intHandler; /* interrupt-afhandeler */
    }
    VICIntEnable = 1<<4;                            /* interruptbron aan */
    VICVectAddr = 0xFF;                             /* acties doorvoeren */

    /* start timer */
    T0TCR = 1<<0;

     __enable_interrupts();
}

static void Timer0_intHandler()
/*
  Deze functie handelt de timer0 interrupts af. Wordt aangeroepen door de
  interrupthandler (staat in crt0.s)
*/
{
    //#if DEBUG D_XINFO
    //usb_printf("timer.c: interrupt timer0\n\r");
    //#endif
    DBG("0");
    //timerFlag++;

    /* wis interrupt-flags */
    T0IR = 0xFFFFFFFF;
}

static void Timer1_intHandler()
/*
  Deze functie handelt de timer1 interrupts af. Wordt aangeroepen door de
  interrupthandler (staat in crt0.s)
*/
{
    //#if DEBUG D_XINFO
    //usb_printf("timer.c: interrupt timer1\n\r");
    //#endif
    DBG("1");
    /* wis interrupt-flags */
    T1IR = 0xFF;
}

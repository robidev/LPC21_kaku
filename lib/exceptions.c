/*
    ARM IP interface

    exceptions.c:
                 exception-handlers testboard

    opmerkingen:
                -NOOIT stack-gerelateerde zaken in C afhandelen; zorgt 
                 gegarandeerd voor problemen, tenzij je precies weet wat de 
                 compiler doet....
                 En (van ARM zelf): "A re-entrant interrupt handler (at least 
                 its 'top-level') *must* be written in assembler."
                 En ja, wij zijn re-entrant...

*/

#include "exceptions.h"

#ifdef CRASHWARNING
#include "delay.h"                      /* vertragingsfuncties */
#include "buzzer.h"                     /* buzzer-aansturing */


#endif

void __HaltUndef()
/*
  Occurs if neither the processor, or any coprocessor, recognizes the currently
  executed instruction.
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: Undefined instruction exception\n\r");
    #endif
    __HaltExit(ERROR);
}

void __HaltSwi()
/*
  This is a user-defined exception. It allows a program running in User-mode, 
  for example, to request priveleged operations that run in Supervisor mode, 
  such as an RTOS function.
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: SWI exception\n\r");
    #endif
    __HaltExit(EXCEPTION);
}

void __HaltPabort()
/*
  Occurs when the processor attempts to execute an instruction that was not 
  fetched, because the address was illegal.
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: Prefetch abort exception\n\r");
    #endif
    __HaltExit(ERROR);
}

void __HaltDabort()
/*
  Occurs when a data transfer instruction attempts to load or store data at an 
  illegal address
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: Data abort exception\n\r");
    #endif
    __HaltExit(ERROR);
}

void __HaltFiq()
/*
  Occurs when the processor external interrupt request pin is asserted(LOW) and
  the I bit in the CPSR is clear
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: FIQ exception\n\r");
    #endif
    __HaltExit(EXCEPTION);
}

void __HaltIrq()
/*
  Occurs when the processor external fast interrupt request pin is asserted(LOW)
  and the F bit in the CPSR is clear.
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: IRQ exception\n\r");
    #endif
    __HaltExit(EXCEPTION);
}

/*
  De volgende zijn niet voor echte exceptions:
*/

void __HaltVICerr()
/*
  ongeldig adres in VICVectAddr
*/
{
    #if DEBUG D_ERROR
    print("exceptions.c: ongeldig adres in VICVectAddr, van interruptbron ");
    switch(VICIRQStatus)
    {
        case (1<<0):
            print("VIC_0\n\r");
            break;
        case (1<<1):
            print("VIC_1\n\r");
            break;
        case (1<<2):
            print("VIC_2\n\r");
            break;
        case (1<<3):
            print("VIC_3\n\r");
            break;
        case (1<<4):
            print("VIC_4\n\r");
            break;
        case (1<<5):
            print("VIC_5\n\r");
            break;
        case (1<<6):
            print("VIC_6\n\r");
            break;
        case (1<<7):
            print("VIC_7\n\r");
            break;
        case (1<<8):
            print("VIC_8\n\r");
            break;
        case (1<<9):
            print("VIC_9\n\r");
            break;
        case (1<<10):
            print("VIC_10\n\r");
            break;
        case (1<<11):
            print("VIC_11\n\r");
            break;
        case (1<<12):
            print("VIC_12\n\r");
            break;
        case (1<<13):
            print("VIC_13\n\r");
            break;
        case (1<<14):
            print("VIC_14\n\r");
            break;
        case (1<<15):
            print("VIC_15\n\r");
            break;
        case (1<<16):
            print("VIC_16\n\r");
            break;
        default:
            print("...eh, ja welke??\n\r");
    }
    #endif
    __HaltExit(ERROR);
}

void __HaltExit(int exitcode)
/*
  Programma stop
*/
{
    #if DEBUG D_ERROR
    //print("exceptions.c: program ended\n\r");
    print("              exitcode %i\n\r",exitcode);
    #endif

    #ifdef CRASHWARNING
    /* Als dit nog niet is gedaan, zal het piepje straks waarschijnlijk niet 
       werken.  Dus voor de zekerheid..
       En bovendien, we zijn toch al gecrashed, dus alle tijd van de wereld.. */
    buzzer_init();
    #endif

    while(1) {
        #ifdef CRASHWARNING
        /* Gooi een piepje naar buiten om de halve seconde
           Vooral bij prefetch en data-aborts is dit best handig;
           dan werkt de UART namelijk (vaak) niet */
        beep();
        delay_ms(500);
        #endif
    }
}

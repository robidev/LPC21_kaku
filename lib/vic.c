/*
    ARM IP interface

    vic.c:
          VIC initialisatie testboard

    opmerkingen:

*/

#include "vic.h"

#define VICVectCntlArray   (((volatile unsigned long *) VICVectCntl0))
#define VICVectAddrArray   (((volatile unsigned long *) VICVectAddr0))

void VIC_init(void)
/*
  Initialisatie VIC; Dit zet alleen een functie in alle vectors van de VIC.
  Als de VIC nu actief wordt terwijl er geen adres in de desbetreffende vector
  is geladen, dan gaat het systeem iniedergeval geen gekke dingen doen.
*/
{
    /* vectors vullen */
    VICVectAddr0 = (unsigned int)__HaltVICerr;
    VICVectAddr1 = (unsigned int)__HaltVICerr;
    VICVectAddr2 = (unsigned int)__HaltVICerr;
    VICVectAddr3 = (unsigned int)__HaltVICerr;
    VICVectAddr4 = (unsigned int)__HaltVICerr;
    VICVectAddr5 = (unsigned int)__HaltVICerr;
    VICVectAddr6 = (unsigned int)__HaltVICerr;
    VICVectAddr7 = (unsigned int)__HaltVICerr;
    VICVectAddr8 = (unsigned int)__HaltVICerr;
    VICVectAddr9 = (unsigned int)__HaltVICerr;
    VICVectAddr10 = (unsigned int)__HaltVICerr;
    VICVectAddr11 = (unsigned int)__HaltVICerr;
    VICVectAddr12 = (unsigned int)__HaltVICerr;
    VICVectAddr13 = (unsigned int)__HaltVICerr;
    VICVectAddr14 = (unsigned int)__HaltVICerr;
    VICVectAddr15 = (unsigned int)__HaltVICerr;
    /* alle interrupts uitgeschakeld */
    VICIntEnClr = 0xFFFFFFFF;
    /* En VIC-instellingen doorvoeren */
    VICVectAddr = 0xFF;
}

void VIC_setup(unsigned char mode, unsigned char priority, unsigned char channel, unsigned int handler)
/*
  Een poging om VIC-initialisatie wat eenvoudiger te maken...

  Let op: nog niet voldoende getest
*/
{
    if (mode == IRQ) {
        VICIntSelect &=~ (1<<priority);                 /* als IRQ */
    }
    else {
        VICIntSelect |=  (1<<priority);                 /* als FIQ */
    }
    VICVectCntlArray[priority] = 0x20 | channel;        /* ingeschakeld, en op aangegeven bron*/
    VICVectAddrArray[priority] = (unsigned int)handler; /* interrupt-afhandeler */
    VICIntEnable = 1<<priority;                         /* interruptbron aan */
    VICVectAddr = 0xFF;                                 /* acties doorvoeren */
}

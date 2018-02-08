/*
    Template LPC2148

    config.h:
             configuratie en algemene definities drivers testboard

    opmerkingen:
                -Wanneer DEBUG aanstaat, moet de UART-code mee worden gecompileerd
                -Voor CRASHWARNING moet zowel de buzzer- als de delay-code mee worden gecompileerd

*/

#ifndef CONFIG_H
#define CONFIG_H

#define FOSC        12000000            /* Frequentie kristal */
#define PLL_MUL     5                   /* PLL vermenigvuldiger. Waarde van 1 tot 6 */
#define PBSD        1                   /* Pheripheral Bus Speed Divider. Is 1, 2, of 4 */

#define BAUDRATE    9600                /* baudrate UART */

/* nog wat 'handige' definities */
#define FALSE       0
#define TRUE        1

#ifndef NULL
#define NULL        ((void *)0)
#endif

/*
  als deze aanstaat wordt er debug-info over de UART verstuurt
  1:    enkel foutmeldingen
  2:    foutmeldingen en waarschuwingen
  3:    ook extra info (debug)
  4:    heel veel extra info
*/
#define DEBUG 3

/* Deze worden gebruikt om debug-niveau te testen, Niet mee pielen, anders klopt bovenstaande lijstje niet meer */
#define D_ERROR > 0
#define D_WARN  > 1
#define D_INFO  > 2
#define D_XINFO > 3

/*
  als deze aanstaat wordt er een waarschuwing gegeven als de ARM vastloopt in een
  exception. Let op, dit werkt uiteraard niet op het oude bordje zonder uitbreidingsbord!
*/
#define CRASHWARNING

/*
  standaard includes voor het arm ontwikkelbord
*/
#include <armboard3.h>
#include <lpc214x.h>                    /* definieert alle registers e.d. van de LPC2148 */
#include <err.h>                /* definieert algemene foutcodes */
#include <exceptions.h>         /* bevat de nodige exception-handlers */
#ifdef DEBUG
#include <uart.h>               /* UART-initialistie en drivers */
#endif

/* deze twee zijn in crt0.s te vinden */
extern void __enable_interrupts(void);
extern void __disable_interrupts(void);

#endif /* CONFIG_H */

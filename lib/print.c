/*
    ARM IP interface

    print.c:
             Minimale implementatie printf

    opmerkingen:
                -Deze printf levert de standaard s(string), c(char) en
                 i/d(integer).
                 Daarnaast zijn ook b(8 bits binair) en h(32 bits hexdecimaal)
                 beschikbaar.

*/

#include "print.h"
#include <stdarg.h>

void print(char *fmt, ...)
/*
  Eenvoudige printf-implementatie. Gebaseerd op code uit het C-handboek, blz. 212
*/
{
    char *string;
    char *i;

    /* Maakt een object aan met alle meegegeven argumenten */
    va_list argpointer;

    /* Initieer argpointer, zodat ie naar het eerste argument wijst */
    va_start(argpointer, fmt);


    /* Loop door alle argumenten */
    for(i=fmt;*i;i++) {
        /* Is dit een variable? */
        if(*i != '%') {
            /* Nee, gewoon printen dus */
            PRINT_CHAR(*i);
            //putchar(*i);
        }
        else {
            /* Ja, maar watvoor? */
            switch(*++i) {
                case 's':
                    /* string */
                    for(string=va_arg(argpointer, char *); *string; string++) {
                        /* we lopen hier door de string heen, zodat ze elk weer
                           als aparte char kunnen worden behandelt */
                        PRINT_CHAR(*string);
                    }
                    break;
                case 'd':
                case 'i':
                    /* int */
                    PRINT_INT(va_arg(argpointer, int));
                    break;
                case 'c':
                    /* char */
                    PRINT_CHAR((char)va_arg(argpointer, int));
                    break;
                case 'b':
                    /* binair */
                    PRINT_BIN((char)va_arg(argpointer, int));
                    break;
                case 'x':
                    /* hex */
                    PRINT_HEX(va_arg(argpointer, int));
                    break;
            }
        }
    }

    /* geheugen vrijgeven enzo.. */
    va_end(argpointer);
}

void print_init()
/*
  Bedoeld om eventuele initialisatie uit te voeren voor het door printf
  gebruikte apparaat
*/
{
    UART_init();
}

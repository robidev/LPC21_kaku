/*
    ARM IP interface

    uart.h:
           UART-driver testboard

*/

#ifndef UART_H
#define UART_H

#include <config.h>

#ifndef BAUDRATE
#error BAUDRATE is not defined!
#endif

#define baud (unsigned short)((((FOSC*PLL_MUL)/PBSD) / ((BAUDRATE) *16.0) + 0.5)) // note: wordt er hier nu naar beneden afgerond of?

  
void UART_init(void);
void UART_put(unsigned char *c);
void UART_putchar(unsigned char c);
void UART_putbin(unsigned char num);
void UART_putint(unsigned int num);
void UART_puthex(unsigned int num);
signed char UART_get(void);

#endif /* UART_H */

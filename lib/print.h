/*
    ARM IP interface

    print.h:
             minimale implementatie printf

*/

#ifndef PRINT_H
#define PRINT_H

#include <config.h>

/* definieer hier waar printf zijn data kwijtmoet; gaat nu naar de UART 
   Als je dit wilt veranderen, verander dan onderstaande definities van
   de output-functies.
   Let op: printf_init() moet ook aan worden gepast. */
#include "uart.h"

#define PRINT_INT(value)    UART_putint(value)
#define PRINT_CHAR(value)   UART_putchar(value)
#define PRINT_BIN(value)    UART_putbin(value)
#define PRINT_HEX(value)    UART_puthex(value)

void print(char *fmt, ...);
void print_init(void);

#endif /* PRINT_H */

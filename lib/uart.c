/*
    ARM IP interface

    uart.c:
           UART-driver testboard

    pin-info:
           P0 - TX
           P1 - RX

    opmerkingen:
                -De UART-interrupts worden niet gebruikt, om (gebruik van) de 
                 code eenvoudig te houden.
                -UART_puthex en UART_putint gebruiken beiden dezelfde functie,
                 omdat het principe exact  gelijk is (UART_putbin zou hier ook 
                 nog bijkunnen...).
                 Dit vergroot de stack wat, maar maakt de code wel kleiner.

*/

#include "uart.h"

static void UART_putnum(unsigned int num, unsigned char deel);

void UART_init(void)
/*
  Stelt gebruikte I/O pinnen en baudrate in.
*/
{
	/* Initialize Pin Select Block for Tx and Rx */
    PINSEL0 = (PINSEL0 & ~MSK_PINSEL0_U0) | VAL_PINSEL0_U0;
    /* Enable FIFO's and reset them */
    U0FCR = 0x7;
    /* Set DLAB and word length set to 8bits */
    U0LCR = 0x83;
    /* Set baudrate */
    U0DLL = (unsigned char)baud;
    U0DLM = baud>>8;
 	/*add fractional prescaler data? -> (5/(5+7)) for 9600 baud, add formula for calculation in 214x.h/uart.h?*/
    /* Clear DLAB */
    U0LCR = 0x3;
    /* Clear interrupt bits */
    U0IER = 0x00;
}

void UART_put(unsigned char *c)
/*
  Stuurt meegegeven string uit op de UART
*/
{
    unsigned int FiFoCount=0;
    signed int i=0;

    /* Wacht tot de buffer leeg is */
    while(!(U0LSR & (1<<6)));

    /* Ga door tot het eind van de string ('\0') is berijkt */
    while(c[i]) {
        U0THR=c[i];
        i++;
        FiFoCount++;
        if(FiFoCount==16) {
            FiFoCount=0;
            /* Buffer vol. Wacht tot ie leeg is */
            while(!(U0LSR & (1<<5)));
        }
    }
}

void UART_putchar(unsigned char c)
/*
  Stuurt meegegeven karakter uit op de UART
*/
{
    /* Wacht tot de buffer leeg is */
    while(!(U0LSR & (1<<6)));

    /* Karakter doorsturen */
    U0THR=c;
}

void UART_putbin(unsigned char num)
/*
  Stuurt meegegeven waarde binair uit op de UART
*/
{
    unsigned char c[9];
    unsigned char waarde = 128;
    unsigned int i=0;

    /* omzetten naar binair */
    while(i<8) {
        if(num >= waarde) {
            num = num - waarde;
            c[i]='1';
        }
        else {
            c[i]='0';
        }
        waarde >>= 1;
        i++;
    }

    /* string afsluiten */
    c[8]='\0';

    /* en doorsturen */
    UART_put(c);
}


void UART_putint(unsigned int num)
/*
  Stuurt meegegeven getal uit op de UART
*/
{
    UART_putnum(num, 10);
}

void UART_puthex(unsigned int num)
/*
  Stuurt meegegeven waarde hexdecimaal uit op de UART
*/
{
    /* Is niet echt nodig, maar staat wel interresant.... */
    #if 0
    UART_put("0x");
    #endif
    UART_putnum(num, 16);
}

signed char UART_get(void)
/*
  Ontvang ? karakter via de UART
*/
{
    /* Kijk of er wat in de buffer staat */
    if (U0LSR & 0x01) {
        /* return karakter */
        return U0RBR;
    }
    return -1;
}

static void UART_putnum(unsigned int num, unsigned char deel)
/*
  Stuurt meegegeven getal uit op de UART in het aangegeven getallenstelsel
*/
{
    static unsigned char chars[16] = "0123456789ABCDEF";
    unsigned int rest;
    signed char c[16];
    signed int i=15;

    /* Zet de integer om naar een string */
    if(num==0) {
        c[i]='0';
        i--;
    }
    else {
        while(num>0) {
            rest=num%deel;
            num/=deel;
            c[i]=chars[rest];
            i--;

            if(i==0) {
                /* It ends here */
                num=0;
            }
        }
    }

    /* Wacht tot de buffer leeg is */
    while(!(U0LSR & (1<<6)));

    /* Stuur de string uit */
    while(i<15) {
        i++;
        U0THR=c[i];
    }
}

/*
    ARM IP interface

    lcd.c:
          LCD-driver testboard

    pin-info:
           P9 - DB4
           P10- DB5
           P11- DB6
           P12- DB7
           P13- E:  start data read/write
           P14- RS: register-select

    opmerkingen:
                -Let op: deze code werkt niet als optimalisatie aan staat!!!!
                -Alle instructies voor het display staan achterstevoren! (maakt 
                 de code wat eenvoudiger)
                -Zorg ervoor dat de delay-code ook wordt meegecompileerd, anders
                 gaat het hier mis...
                -Deze code is gebaseerd op
                 "LCD routines for OLIMEX LPC-MT-2106,16x2 Character LCD 1602K,
                  4-bit mode operation",
                 Copyright (c) 2004 senz at arm.dreamislife.com

*/

#include "lcd.h"
#include "delay.h"

volatile static void LCD_writenibble(unsigned char data);
volatile static void LCD_writebyte(unsigned char data);

volatile void LCD_init(void)
/*
  Initialiseert LCD

  Display wordt gebruikt in 4bits modus,1 regel, 5x7 dots font.
*/
{
    /* standaard I/O */
    PINSEL0 &= LCD_GPIOMASK;
    
    /* Alles output */
    IODIR0 |= LCD_IOALL;
    
    IOCLR0 |= LCD_RS;
    IOCLR0 |= LCD_EN;

    delay_ms(15);

    /* function-set opstart */
    LCD_writebyte(0x30);
    delay_ms(50);
    LCD_writebyte(0x30);
    LCD_writebyte(0x30);

    /* interface op 4 bits */
    IOSET0 |= LCD_EN;
    LCD_writebyte(0x02);
    IOCLR0 |= LCD_EN;
    delay_ms(5);

    /* Function Set */
    LCD_writebyte(0x28);
    /* LCD aan */
    LCD_writebyte(0x0E);
    /* wis scherm */
    LCD_writebyte(0x01);
    /* Entry mode set */
    LCD_writebyte(0x06);
}

volatile void LCD_put(unsigned char *c)
/*
  Zet meegegeven string op het scherm
*/
{
    volatile unsigned char i=0;
    volatile unsigned char j;

    while(c[i]) {
        if(i==8) {
            /* nogal een vaag display; teken 8 tot 31 is niet uitgevoerd op het 
               scherm */
            for (j=0; j<32; j++) {
                LCD_cursor_right();
                delay_ms(50);
            }
        }
           
        LCD_putchar(c[i]);
        i++;
    }
}

volatile void LCD_putchar(unsigned char c)
/*
  Zet meegegeven karakter op het scherm
*/
{
    IOSET0 |= LCD_RS;
    LCD_writebyte(c);
}

volatile void LCD_putint(unsigned int num)
/*
  Zet het meegegeven getal op het scherm
*/
{
    static unsigned char chars[10] = "0123456789";
    unsigned int rest;
    char c[16];
    int i=15;
    volatile int j,k;

    /* Zet de integer om naar een string */
    if(num==0) {
        c[i]='0';
        i--;
    }
    else {
        while(num>0) {
            rest=num%10;
            num/=10;
            c[i]=chars[rest];
            i--;

            if(i==0) {
                /* It ends here */
                num=0;
            }
        }
    }

    /* string uitsturen. Deze code mag niet worden geoptimaliseerd door de
       compiler, dus een aparte volatile kopie van i (zodat de code hierboven
       wel kan worden geoptimaliseerd) */
    k=i;
    while(k<15) {
        if(k==8) {
            /* nogal een vaag display; teken 8 tot 31 is niet uitgevoerd op het 
               scherm */
            for (j=0; j<32; j++) {
                LCD_cursor_right();
                delay_us(500);
            }
        }
        k++;
        LCD_putchar(c[k]);
    }
}

volatile static void LCD_writenibble(unsigned char data)
/*
  Stuurt een 4-bits commando naar het display
*/
{
    IOCLR0 |= LCD_DATA;
    IOSET0 |= (data<<STARTBIT);
}

volatile static void LCD_writebyte(unsigned char data)
/*
  Stuurt een 8-bits commando naar het display
*/
{
    /* hoogste 4 bits */
    IOSET0 |= LCD_EN;
    LCD_writenibble((data>>4)&0x0F);
    IOCLR0 |= LCD_EN;

    delay_ms(5);

    /* laagste 4 bits */
    IOSET0 |= LCD_EN;
    LCD_writenibble(data&0x0F);
    IOCLR0 |= LCD_EN;

    delay_ms(5);
}

volatile void LCD_writecontrol(unsigned char data)
/*
  Stuurt een commando naar het display
*/
{
    IOCLR0 |= LCD_RS;
    LCD_writebyte(data);
}

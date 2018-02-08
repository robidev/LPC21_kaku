/*
    ARM IP interface

    lcd.h:
          LCD-driver testboard

*/

#ifndef LCD_H
#define LCD_H

#include <config.h>

/* I/O-pinnen */
#define  STARTBIT       9
#define  LCD_D4         (1<<9)
#define  LCD_D5         (1<<10)
#define  LCD_D6         (1<<11)
#define  LCD_D7         (1<<12)
#define  LCD_EN         (1<<13)
#define  LCD_RS         (1<<14)
#define  LCD_GPIOMASK   (~(3<<(9*2)))& (~(3<<(10*2))) & (~(3<<(11*2))) \
                         & (~(3<<(12*2))) & (~(3<<(13*2))) & (~(3<<(14*2)))

#define  LCD_DATA       (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define  LCD_IOALL      (LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_EN|LCD_RS)

/* functie-macro's */
#define LCD_clear()          LCD_writecontrol(0x01)
#define LCD_cursor_home()    LCD_writecontrol(0x02)

#define LCD_display_on()     LCD_writecontrol(0x0E)
#define LCD_display_off()    LCD_writecontrol(0x08)

#define LCD_cursor_blink()   LCD_writecontrol(0x0F)
#define LCD_cursor_on()      LCD_writecontrol(0x0E)
#define LCD_cursor_off()     LCD_writecontrol(0x0C)

#define LCD_cursor_left()    LCD_writecontrol(0x10)
#define LCD_cursor_right()   LCD_writecontrol(0x14)
#define LCD_display_sleft()  LCD_writecontrol(0x18)
#define LCD_display_sright() LCD_writecontrol(0x1C)

volatile void LCD_init(void);
volatile void LCD_writecontrol(unsigned char data);
volatile void LCD_put(unsigned char *c);
volatile void LCD_putchar(unsigned char c);
volatile void LCD_putint(unsigned int num);

#endif /*LCD_H*/

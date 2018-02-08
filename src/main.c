//////////////////////////////////////////////////////////////////////////////
// prog: main.c
// comm:
// auth: Robin
//////////////////////////////////////////////////////////////////////////////

#include <config.h>
#include <main.h>

#include <pll.h>
#include <buzzer.h>
#include <uart.h>
#include <delay.h>
#include <uart_usb.h>
#include <printf_usb.h>
#include <debug.h>
#include <kaku.h>
#include <RC5.h>






int main(void)
{
    int c;
    PLL_init();
    UART_init();
    USB_UART_Init();  
    kaku_init();
    buzzer_init();
    RC5_Init();
    

    /*while(VCOM_getchar() == EOF)
        ;//wait for keystroke
    usb_printf("*** USB serial console ***\n\r\n\r");
    usb_printf("Processor: ARM LPC2148\n\r");
    usb_printf("processor speed at %d MHz, ", CCLK/1000000);
    usb_printf("with perhiperals on %d MHz\n\r\n\r", CCLK/PBSD/1000000);

    usb_printf(">");*/
    while(1)
    {
        c = VCOM_getchar();
        if (c != EOF) 
        {
            switch(c)
            {
                case 0x01:
                {
                    int device = VCOM_getchar();
                    int command = VCOM_getchar();
                    if(device != EOF && command != EOF)
                        RC5_TX(device,command); 
                    else
                        VCOM_putchar(0xFF);
                    break;
                }
                case 0x02:
                {
                    int channel = VCOM_getchar();
                    int light = VCOM_getchar();
                    if(channel != EOF && light != EOF)
                        outputKakuRFSignal2(light, channel, 1);
                    else
                        VCOM_putchar(0xFF);
                    break;
                }
                case 0x03:
                {
                     int channel = VCOM_getchar();
                     int light = VCOM_getchar();
                     if(channel != EOF && light != EOF)
                         outputKakuRFSignal2(light, channel, 0);
                     else
                         VCOM_putchar(0xFF);
                     break;
                }
            }
            c=0;
        }
        if (RC5_flag) // wait for RC5 code
        {
            RC5_flag = 0;
            VCOM_putchar(0xEF);
            VCOM_putchar(0xEF);
            VCOM_putchar(RC5_System);
            VCOM_putchar(RC5_Command); // and print it
            VCOM_putchar('\r');
        }             
/*            if ((c == 9) || (c == 10) || (c == 13) || ((c >= 32) && (c <= 126))) {
                DBG("%c", c);
            }
            else {
                DBG(".");
            }
            //VCOM_putchar(c);

            if(c==97)
            {
                usb_printf("\n\n\r>IR TX\n\r>");
                beep();
            }
            if(c==99 || c==100)
            {
                int command = 0;
                if(c==99)
                    command=1;
                else
                    command=0;
            
                usb_printf("\n\n\r> input light(1,2,3,4)\n\r");
                c=-1;
                while((c = VCOM_getchar())==EOF);
                int light=c;
                if(light==49)
                    light=0;
                else if(light==50)
                    light=1;
                else if(light==51)
                    light=4;
                else if(light==52)
                    light=5;
                else
                {
                    usb_printf("\n\r input invalid, default:1");
                    light=0;
                }
            
            
                usb_printf("\n\n\r> input channel(a,b,c,d)\n\r");
                c=EOF;
                while((c = VCOM_getchar())==EOF);
                int channel=c;
            
                if(channel==97)
                    channel=0;
                else if(channel==98)
                    channel=1;
                else if(channel==99)
                    channel=4;
                else if(channel==100)
                    channel=5;
                else
                {
                    usb_printf("\n\r input invalid, default:a");
                    channel=0;
                }
                outputKakuRFSignal2(light, channel, command) ;
            }
            if(c==49)
            {
                usb_printf("\n\n\r> input command\n\r");
                c=EOF;
                while((c = VCOM_getchar())==EOF);
                int command=(c-39);
                RC5_TX(0xc0,command);   
            }*/   
    }
    return 0;
}



#include <config.h>
#include <debug.h>
#include <RC5.h>

static void RC5_Isr(void);

static signed int low_time;
static signed int high_time;
static unsigned char half_bit;
static unsigned char sys; // temp system byte
static unsigned char cmd; // temp Command byte

unsigned char RC5_System; // Format 1 E/N t s4 s3 s3 s1 s0
unsigned char RC5_Command; // Format 0 0 c5 c4 c3 c2 c1 c0
unsigned char RC5_flag;
void Capture(char bit);

void IR_TX(int bit);

void RC5_Init(void)
{
 VICVectAddr0 = (unsigned int) &RC5_Isr;
 VICVectCntl0 = 0x24; // Channel0 on Source#4 ... enabled
 VICIntEnable |= 0x10; // Channel#4 is the Timer 0

 PINSEL1 |= 0x30000000; // P0.30 as CAP0.0

 T0PR = 60; // presc 60, timer runs at 1 MHz
 T0MR0 = 12000; // 12 ms High (idle) Timeout
 T0MCR = 3; // Int on Match0, reset timer on match
 T0CCR = 0x0007; // Capture and interrupt on both edges
 T0TC = 0; // Reset timer
 T0TCR = 1; // start timer
 
 //transmitter
 PINSEL0 &= ~(3<<(IR_TX_PIN*2));
 IODIR0   |= 1<<IR_TX_PIN;
 
}

#define IR_PULS 833

void RC5_TX(unsigned short system,unsigned short command)
{
     static unsigned short toggle = 0;
     int i;
     
     if(toggle)
         toggle=0;
     else
         toggle=0x20;    
     system |= toggle;
     
     for(i=0;i<8;i++)
     {
         if(system & 0x80)
         {
             IR_TX(1);
         }
         else
         {
             IR_TX(0);
         }
         system = system << 1;//next bit
     }
     for(i=0;i<6;i++)
     {
         if(command & 0x20)
         {
             IR_TX(1);
         }
         else
         {
             IR_TX(0);
         }
         command = command << 1;//next bit
     }
}

void IR_TX(int bit)
{
     int j;
     if(bit==1)
     {
         //low
         T1TC = 0;
         while(T1TC<(32*(IR_PULS*2))) ;
         //high               
         for(j=0; j<32; j++)
         {
             IOSET0 = 1<<IR_TX_PIN;
             T1TC = 0;
             while(T1TC<IR_PULS);//bittiming) ;

             IOCLR0 = 1<<IR_TX_PIN;

             T1TC = 0;
             while(T1TC<IR_PULS);//bittiming) ;
         } 
     }
     else
     {
         //high
         for(j=0; j<32; j++)
         {
             IOSET0 = 1<<IR_TX_PIN;
             T1TC = 0;
             while(T1TC<IR_PULS);//bittiming) ;

             IOCLR0 = 1<<IR_TX_PIN;

             T1TC = 0;
             while(T1TC<IR_PULS);//bittiming) ;
         } 
         //low
         T1TC = 0;
         while(T1TC<(32*(IR_PULS*2))) ;
     }
}

/*
    unsigned int data[24] = {
       32,32,//888,888,
       64,32,//1777,888,
       32,32,//888,888,
       32,32,//888,888,
       32,32,//888,888,
       32,32,//888,888,
       32,32,//888,888,
       32,64,//888,1777,
       64,32,//1777,888,
       32,32,//888,888,
       32,32,//888,888,
       32,512};//888,12000};
    IR_TX_data(data,24,36000);
*/
void IR_TX_data(unsigned int * timingarray,unsigned int arraysize,unsigned int modulationfreq)
{
    unsigned int i=0, j=0;

    //unsigned int bittiming = 60000000/(modulationfreq*2);
    //unsigned int bitlength = 0;
    while(i<arraysize)
    {  
        //bitlength = ((timingarray[i]*60)/bittiming);
        for(j=0; j<timingarray[i];/*bitlength;*/ j++)
        {
            IOSET0 = 1<<IR_TX_PIN;
    
            T1TC = 0;
            while(T1TC<IR_PULS);//bittiming) ;

            IOCLR0 = 1<<IR_TX_PIN;

            T1TC = 0;
            while(T1TC<IR_PULS);//bittiming) ;
        } 

        i++;
        T1TC = 0;
        while(T1TC<(timingarray[i]*(IR_PULS*2))) ;
        i++;
    }
}


static void RC5_Shift_Bit(char val)
{
    if (sys & 0x80)
    {
        if (cmd & 0x80) // command full ?
        {
            sys = 0; // yes, ERROR
            cmd = 0;
        }
        else
            cmd = (cmd << 1) | val; // shift command
    }
    else
    sys = (sys << 1) | val; // shift system
}


static void RC5_Decode(void)
{
    unsigned char action;

    action = half_bit << 2;

    if ((high_time > MIN_FULL_BIT) && (high_time < MAX_FULL_BIT))
        action = action | 1; // high_time = long
    else if (!((high_time > MIN_HALF_BIT) && (high_time < MAX_HALF_BIT)))
    {
            sys = 0; // RC5 ERROR
            cmd = 0;
            //usb_printf("hE:%d.",high_time);
            return;
    }

    if ((low_time > MIN_FULL_BIT) && (low_time < MAX_FULL_BIT))
        action = action | 2; // low_time = long
    else if (!((low_time > MIN_HALF_BIT) && (low_time < MAX_HALF_BIT)))
    {
            sys = 0; // RC5 ERROR
            cmd = 0;
            //usb_printf("oE:%d.",low_time);
        return;
    }

    switch (action)
    {
        case 0: 
            RC5_Shift_Bit(0); // short low, short high, shift 0
        break;
        case 1: 
            RC5_Shift_Bit(1); // short low, long high, shift 1
            half_bit = 1; // new half bit is true
        break;
        case 2:
            sys = 0; // long low, short high, ERROR
            cmd = 0;
        case 3:
            RC5_Shift_Bit(1); // long low, long high, shift 1,0
            RC5_Shift_Bit(0);
        break;
        case 4:
            RC5_Shift_Bit(1); // short low, short high, shift 1
        break;
        case 5:
            sys = 0; // short low, long high, ERROR
            cmd = 0;
        break;
        case 6: 
            RC5_Shift_Bit(1); // long low, short high, shift 1,0
            RC5_Shift_Bit(0);
            half_bit = 0; // new half bit is false
        break;
        case 7: 
            sys = 0; // long low, long high, ERROR
            cmd = 0;
        default: break; // invalid
    }
}


static void RC5_Isr(void)
{
    T0TC = 0; // Reset timer

    if (T0IR & 1) // Timeout ? to guarantee a 12 msec
    { // idle time after last RC5 pulse
        if (cmd & 0x80) // command full ?
        {
            RC5_Command = cmd & 0x7F; // OK! Save command byte
            RC5_System = sys; // save system byte
            RC5_flag = 1; // set event to application
        }
        sys = 0;
        cmd = 0;
        T0IR = 0x01; // clear MR0 interrupt flag
    }
    else // capture interrupt
    {
        if (IO0PIN & 0x00010000) // check P0.16, rising or falling edge
        {
            if (sys == 0) // First pulse ?
            {
                low_time = T0CR0;//HALF_BIT_TIME; // assume short low time
                high_time = HALF_BIT_TIME; // assume short high time
                half_bit = 1; // assume half bit is true
                cmd = 0x02; // = 00000010, prepare command byte
            }
            else
                low_time = T0CR0; // rising, so capture low time
            RC5_Decode();
        }
        else
        {
            high_time = T0CR0; // falling, so capture high time
        }
        T0IR = 0x10; // reset interrupt flag
    }
    //VICVectAddr = 0; // Acknowledge interrupt by reseting VIC
}




/*
    ARM IP interface

   relays.c:
             Output Op SV3 Armboard V3.x

    pin-info:
           P1.17 - output relays_0
           P1.18 - output relays_1
           P1.19 - output relays_2
           P1.20 - output relays_3
           P1.21 - output relays_4
           P1.22 - output relays_5
           P1.23 - output relays_6
           P1.24 - output relays_7
           P1.25 - output relays_8
           

    opmerkingen:
                -

*/

#include "relays.h"

void relays_init(void)
/*
  Stelt gebruikte I/O pinnen in.
*/
{
     PINSEL2 &= ~(3<<((RELAYS_0-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_1-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_2-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_3-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_4-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_5-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_6-16)*2));  
     PINSEL2 &= ~(3<<((RELAYS_7-16)*2));  

     IODIR1   |= 1<<RELAYS_0;
     IODIR1   |= 1<<RELAYS_1;
     IODIR1   |= 1<<RELAYS_2;
     IODIR1   |= 1<<RELAYS_3;
     IODIR1   |= 1<<RELAYS_4;
     IODIR1   |= 1<<RELAYS_5;
     IODIR1   |= 1<<RELAYS_6;
     IODIR1   |= 1<<RELAYS_7;
    
}



void relays_set(unsigned char number)
/*
  set relays
  */
{
   
     	IOSET1= 1<<number;
	}
}
void relays_clr(unsigned char number)
/*
 clr relays
*/
{
   
    	IOCLR1 = 1<<number;
	}
}

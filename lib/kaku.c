#include <config.h>
#include <delay.h>
#include <kaku.h>



void kaku_init(void)
{
    PINSEL0 &= ~(3<<(PIN_O*2));
     IODIR0   |= 1<<PIN_O;
     
         /* start timer */
    //T0TCR = 1<<0;
    T1TCR = 1<<0;
}

void send_bit(unsigned int bit)
{
    IOSET0 = 1<<PIN_O;
    
    T1TC = 0;
    //while(T0TC<PULS*60) ;
    while(T1TC<PULS*60) ;

    if(bit==0)
        IOCLR0 = 1<<PIN_O;

    T1TC = 0;
    //while(T0TC<PULS*60) ;
    while(T1TC<PULS*60) ;

    IOCLR0 = 1<<PIN_O;

    T1TC = 0;
    //while(T0TC<PULS*60) ;
    while(T1TC<PULS*60) ;


}

void outputKakuRFSignalSeq(unsigned char channel, unsigned char command) 
{
	//first databyte, consist of 2 bytes. 
	//first(higher) 4 bytes are lights(4 in total),
	//second(lower) 4 bytes are channels(16 in total)
	send_bit(0);
	send_bit((channel & 0x80) >> 7);
	send_bit(0);
	send_bit((channel & 0x40) >> 6);
	send_bit(0);
	send_bit((channel & 0x20) >> 5);
	send_bit(0);
	send_bit((channel & 0x10) >> 4);
	send_bit(0);
	send_bit((channel & 0x08) >> 3);
	send_bit(0);
	send_bit((channel & 0x04) >> 2);
	send_bit(0);
	send_bit((channel & 0x02) >> 1);
	send_bit(0);
	send_bit((channel & 0x01));
	//?
	send_bit(0);
	send_bit(1);
	send_bit(0);
	send_bit(0);
	send_bit(0);
	send_bit(1);
	send_bit(0);
	send_bit(0);
	//1=on, 0=off
	send_bit(0);
	send_bit(1);
	send_bit(0);
	send_bit(command);
	send_bit(0);
	send_bit(1);
	send_bit(0);
	send_bit(!command);
	
}

void outputKakuRFSignal(unsigned char channel, unsigned char command) 
{
	unsigned char count = 0;

	while (count < 5) {
		outputKakuRFSignalSeq(channel, command);
        delay_ms(20);
		count++;
	}
}

void outputKakuRFSignal2(unsigned char light, unsigned char channel, unsigned char command) 
{
     int j;
     for(j=0;j<4;j++)
     {
	   //frame description
	   //startbit
       send_bit(0);							

	   //first byte, selects channel, a,b,c or d.
       //channel a=0x00
       //channel b=0x01
       //channel c=0x04
       //channel d=0x05                    
       send_bit((channel & 0x01));
       send_bit(0);
       send_bit((channel & 0x04) >> 2);
       send_bit(0);
       send_bit(0);
       send_bit(0);
       send_bit(0);
       send_bit(0);

	   //second byte, selects light, 1,2 or 3
       //1=0x00
       //2=0x01
       //3=0x04                    
       send_bit((light & 0x01));
       send_bit(0);
       send_bit((light & 0x04) >> 2);
       send_bit(0);
       send_bit(0);
       send_bit(0);
       send_bit(0);  
       send_bit(0);

	   //selects the command, on or off.
       //off=0x41
       //on=0x45
       send_bit(0);
       send_bit(0);
       send_bit(1);
       send_bit(0);
       send_bit(1);
       send_bit(0);
       send_bit(command & 0x01);
       send_bit(0);
       //delay before next frame can be send               
       delay_ms(10);                
     }
}

/*
    PWM servo software
    Ajay, robin, Reinier, Maarten
*/
#include "pwm.h"
#include "delay.h"

void pwm4_init(void)
{
//  PWMTCR = 0x0002;//timer counter en prescale counter reset voor
//  IODIR  |= 0x00202;
//	PINSEL0 |= 0xA000A; // select pin 0.08 PWM4 function
//  PWMMCR = 0x0002; //?
//	PWMMR0 =  7000; // Periodetijd 50 300 -
//	PWMLER = 0x01; 
//	PWMPCR = 0x4a00; //PWM4 enable, single edge
//	PWMPR  = 125;//prescale register, deelfactor  //1000  500 125


    PINSEL0 |= 0x00020000; // Change GPIO P0.8 to PWM4
    PWMPR = 0x00000000; // Set Pre Scale to 0
    IODIR0 |= 0x00000100; // Set Direction of PWM4 Pin to be output
    PWMPCR = 0x00001000; //Set PWM4 to enable with single edge
    PWMMR0 = 0x000001BF; // Max Value of Pulse Width                  447*60mhz ticks = 134,228 khz
    PWMMR4 = 0x000000DF; // PWM4 Match Value                          223*60mhz ticks = 50% duty cycle
    PWMMCR = 0x00000002; // Reset PWMMTC when Matched with PWMMR0
    PWMLER = 0x7F; //Enable latch in all match registers
    PWMTCR = 0x02; //Reset PWMMTC
    PWMTCR = 0x09; //Counter Enable, PWM Mode Enabled(bit0, bit3)
}
void pwm_1(int dutycycle)
{
	//PWMTCR = 0x0002;//timer counter en prescale counter reset voor
	
	PWMMR1 = dutycycle; // duty cycle
	PWMLER = 0x02;
	PWMTCR = COUNTERenable; // PWM enable en counter enabled voor counting
	delay_s(1);
}
void pwm_2(int dutycycle)
{
	//PWMTCR = 0x0002;//timer counter en prescale counter reset voor
	
	PWMMR3 = dutycycle; // duty cycle
	PWMLER = 0x8;
	PWMTCR = COUNTERenable; // PWM enable en counteru enabled voor counting
	delay_s(1);
}
void pwm_3(int dutycycle)
{
	//PWMTCR = 0x0002;//timer counter en prescale counter reset voor
	
	PWMMR6 = dutycycle; // duty cycle
	PWMLER = 0x40;
	PWMTCR = COUNTERenable; // PWM enable en counter enabled voor counting
	delay_s(1);
}

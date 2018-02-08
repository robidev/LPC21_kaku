

#ifndef PWM_H
#define PWM_H


#include <config.h>



/* DEFINES VOOR PWMM:*/
//#define PWM136			0xA8000
//#define PWM136enable 	0x5400
#define COUNTERenable 	0x09
#define	MRenable		0x01


void pwm_1(int dutycycle);
void pwm_2(int dutycycle);
void pwm_3(int dutycycle);
void pwm4_init(void);


#endif /* PWM_H */

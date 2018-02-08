#ifndef RC5_H
#define RC5_H

#define MIN_HALF_BIT 640 // 640 us
#define HALF_BIT_TIME 889 // 889 us
#define MAX_HALF_BIT 1140 // 1140 us
#define MIN_FULL_BIT 1340 // 1340 us
#define FULL_BIT_TIME 1778 // 1778 us
#define MAX_FULL_BIT 2220 // 2220 us

#define IR_TX_PIN 8

void RC5_Init(void);

void IR_TX_data(unsigned int * timingarray,unsigned int arraysize,unsigned int modulationfreq);
void RC5_TX(unsigned short system,unsigned short command);

extern unsigned char RC5_System; // Format 1 E/N t s4 s3 s3 s1 s0
extern unsigned char RC5_Command; // Format 0 0 c5 c4 c3 c2 c1 c0
extern unsigned char RC5_flag;

#endif

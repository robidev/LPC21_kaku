#ifndef KAKU_H
#define KAKU_H

#define PULS 425
//#define PIN_02 2
#define PIN_O 3


void kaku_init(void);
void send_bit(unsigned int bit);
void outputKakuRFSignalSeq(unsigned char channel, unsigned char command);
void outputKakuRFSignal(unsigned char channel, unsigned char command);
void outputKakuRFSignal2(unsigned char light, unsigned char channel, unsigned char command); 

#endif

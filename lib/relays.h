/*
    ARM common interface

    buzzer.h:
             piepjes testboard

*/

#ifndef RELAYS_H
#define RELAYS_H

#define relays_0 17
#define relays_1 18
#define relays_2 19
#define relays_3 20
#define relays_4 21
#define relays_5 22
#define relays_6 23
#define relays_7 24


/*******************************************************************************
  Volgende code is niet bedoeld om aan te passen
*/

//#include <config.h>

void relays_init(void);
void relays_set(unsigned char number );
void relays_clr(unsigned char number );

#endif /* RELAYS_H */

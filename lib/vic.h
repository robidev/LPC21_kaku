/*
    ARM IP interface

    vic.h:
          VIC initialisatie testboard

*/

#ifndef VIC_H
#define VIC_H

#include <config.h>

#define IRQ 0
#define FIQ 1

void VIC_init(void);
void VIC_setup(unsigned char mode, unsigned char priority, unsigned char channel, unsigned int handler);

#endif /* VIC_H */

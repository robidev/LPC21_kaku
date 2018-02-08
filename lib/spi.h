/*
    ARM IP interface

    spi.h:
          SPI-driver testboard

*/

#ifndef SPI_H
#define SPI_H

#include <config.h>

void SPI_init(unsigned char config);
error_t SPI_put(unsigned char data);
unsigned char SPI_get(void);

#endif /* SPI_H */

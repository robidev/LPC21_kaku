/*
    ARM IP interface

    err.h:
          definitie foutcodes

    opmerkingen:
                -

*/

#ifndef ERR_H
#define ERR_H

/* error-type */
typedef int error_t;

/* algemeen */
#define GOOD        1
#define ERROR       11
#define EXCEPTION   12
#define TERM        13
#define TIMEOUT     14
#define IOERROR     15

/* SPI interface */
#define SPI_GOOD    0x80
#define SPI_ABRT    0x08
#define SPI_MODF    0x10
#define SPI_ROVR    0x20
#define SPI_WCOL    0x40

#endif /* ERR_H */

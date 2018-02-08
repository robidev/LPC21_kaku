/*
    ARM IP interface

    spi.c:
          SPI-driver testboard

    pin-info:
           P4 - SCK:  klok
           P5 - MISO: Data-uit
           P6 - MOSI: Data-in
           P7 - SSEL: Chip-select

    opmerkingen:
                -Pin 4 en 6 worden zowel door de LED's als door de SPI gebruikt
                -SSEL moet via een weerstand(1 tot 10k) met de positieve
                 voedingsspanning worden verbonden
                -Nadat een timeout is opgetreden bij het verzenden doordat SSEL
                 loszit, zal de SPI-interface soms van mening zijn dat de data
                 goed is verzonden, terwijl er niets wordt ingelezen.
                 Extra controle op deze returncode(0x07) van SPI_put is dus
                 raadzaam.
*/

#include "spi.h"

/* Er staan in dit bestand twee functies om data naar de SPI te sturen; een met
   foutcontrole, en een zonder. Volgende define uitzetten, en je krijgt de
   functie zonder foutcontrole. */
#define SPI_ERR_TEST

unsigned char SPIconfig;

void SPI_init(unsigned char config)
/*
  SPI initialisatie
      configuratie:
        0:2 niet gebruikt
        3   0: data op eerste klokflank van SCK gesampled
            1: data op tweede klokflank van SCK gesampled
        4   0: SCK is actief laag
            1: SCK is actief hoog
        5   0: slave mode
            1: master mode
        6   0: data MSB eerst
            1: data LSB eerst
        7   0: SPI interrupt uitgeschakeld
            1: SPI interrupt aan
*/
{
    /* pinnen instellen(alle 4 op SPI-functie) */
    PINSEL0 |= 0x5500;
    /* SPI op maximale snelheid. Snelheid is dan VPB clock/SPCCR value (MHz) */
    SPCCR = 8;
    /* SPI configuratie doorvoeren */
    SPIconfig = config;
    SPCR = config;
}

unsigned char SPI_get()
/*
  Ontvang één karakter via de SPI. Als er niets in de buffer staat word er een 
  nul (0x00) ingelezen.
*/
{
    return SPDR;
}

#ifdef SPI_ERR_TEST
/* SPI_put met foutcontrole */

error_t SPI_put(unsigned char data)
/*
  Stuur meegegeven data uit op de SPI.
  Statusbits van SPI worden teruggegeven.
*/
{
    unsigned int status;
    unsigned int timeout=0;

    /* Data naar SPI */
    SPDR = data;

    /* Wacht tot ie klaar is */
    while(!(status = (SPSR & 0xF8))) {
        timeout++;
        if (timeout==99999) {
            #if DEBUG D_ERROR
            print("SPI.c: Timeout!\n\r");
            #endif
            /* Na een timeout werkt de SPI niet meer(blijft hangen).
               Opnieuw starten dus */
            SPI_init(SPIconfig);
            return TIMEOUT;
        }
    }

    /* controleer foutcodes */
    #if DEBUG D_ERROR
    if (status&SPI_ABRT) {
        print("SPI.c: Slave abort\n\r");
    }
    #endif
    if (status&SPI_MODF) {
        #if DEBUG D_ERROR
        print("SPI.c: Mode fault\n\r");
        #endif
        /* schrijf naar control-register om statusbit te wissen */
        SPCR=SPCR;
    }
    #if DEBUG D_ERROR
    if (status&SPI_ROVR) {
        print("SPI.c: Read overrun\n\r");
    }
    #endif
    if (status&SPI_WCOL) {
        #if DEBUG D_ERROR
        print("SPI.c: Write collison\n\r");
        #endif
        /* data-register uitlezen om statusbit te wissen */
        timeout=SPDR;
    }

    return status;
}

#else
/* SPI_put zonder foutcontrole */

error_t SPI_put(unsigned char data)
/*
  Stuur meegegeven data uit op de SPI.
  Statusbits van SPI worden teruggegeven.
*/
{
    /* Data naar SPI */
    SPDR = data;

    /* Wacht tot ie klaar is */
    while(!(SPSR & SPI_GOOD));

    return SPI_GOOD;
}

#endif /* SPI_ERR_TEST */

/*
    ARM IP interface

    pll.c:
          PLL-initialisatie testboard

    opmerkingen:
                -Als de PLL niet locked, blijft de processor hier hangen

*/

#include "pll.h"

void PLL_init(void)
/*
  Initialiseert de PLL met de gedefineerde snelheden
*/
{
    /* Stel PLL in*/
    PLL0CFG  = VAL_PLLCFG_MSEL | VAL_PLLCFG_PSEL;
    /* PLL enable */
    PLL0CON  = BIT_PLLCON_PLLE;
    /* PLL feed sequence */
    PLL0FEED = 0x000000AA;
    PLL0FEED = 0x00000055;
    /* Wacht tot PLL is gelocked */
    while(!(PLL0STAT & BIT_PLLSTAT_LOCK));
    /* PLL enable EN connect */
    PLL0CON  = BIT_PLLCON_PLLE | BIT_PLLCON_PLLC;
    /* PLL feed sequence */
    PLL0FEED = 0x000000AA;
    PLL0FEED = 0x00000055;
    /* Fully enable MAM */
    MAMCR   = VAL_MAMCR_FULL;
    /* Set VPB clock */
    VPBDIV  = VAL_VPBDIV;
}

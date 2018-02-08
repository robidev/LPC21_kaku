/***********************************************************************/
/*  Dit is een onderdeel van het GNU armboard V3 Hogeschool Utrecht    */
/*  Apptec Systems sept 2006					                       */
/***********************************************************************/
/*                                                                     */         
/*  armboerd3.h: Header file voor het Armboard V3 Hogeschool Utrecht   */
/*  Philips LPC2141/42/44/46/48                                        */                            
/***********************************************************************/

#define txd		1<<0
#define rtx		1<<1

#define scl0	1<<2
#define sda0	1<<3

#define sck0	1<<4
#define miso0	1<<5
#define mosi0	1<<6
#define ssel0	1<<7

#define spkr	1<<8
#define lcd4	1<<9
#define lcd5	1<<10
#define lcd6	1<<11
#define lcd7	1<<12
#define lcd_e	1<<13
#define lcd_rs	1<<14

#define sd_det	1<<15
#define sd_prot	1<<24
#define sdclock	1<<17
#define sdmiso	1<<18
#define sdmosi	1<<19
#define sdsel	1<<20

#define rck		1<<16  			// load input van de HC595
#define sck		1<<4			// clock ingang van HC595
#define ser		1<<6			// data ingang van de HC595

#define key_dat	1<<21
#define key_clk	1<<22
#define key_req	1<<25			// port 1

#define aout	1<<25
#define ain		1<<28


/* overige defines 		*/


#define ramstart   ((volatile unsigned long *) 0x40000000)
/*	unsigned long *p=ramstart;		// ramstart pointer direct read	*/

#define MEMMAP (*((volatile unsigned int *) 0xE01FC040))
#define IODIR0 (*((volatile unsigned int *) 0xE0028008))
#define IOCLR0 (*((volatile unsigned int *) 0xE002800C))
#define PINSEL0 (*((volatile unsigned int *) 0xE002C000))
#define VPBDIV (*((volatile unsigned int *) 0xE01FC100))
#define PLLCON (*((volatile unsigned int *) 0xE01FC080))
#define PLLFEED (*((volatile unsigned int *) 0xE01FC08C))
#define VICINTENCLR (*((volatile unsigned int *) 0Xfffff014))
#define TIMER1_PR (*((volatile unsigned int *) 0xE000800C))
#define TIMER1_MCR (*((volatile unsigned int *) 0xE0008014))
#define TIMER1_CCR (*((volatile unsigned int *) 0xE0008028))

void (*bootloader_entry)(void);

unsigned long temp;

void enter_ISP(void)
{
  temp = PINSEL0;
  /* Connect RXD0 & TXD0 pins to GPIO */
  PINSEL0 = temp & 0xFFFFFFF3;
  /* Select P0.14 as an output and P0.1 as an input */
  temp = IODIR0;
  temp = temp | 0x4000;
  temp = temp & 0xFFFFFFFD;
  IODIR0 = temp;

  /* Clear P0.14 */
  IOCLR0 = 0x4000;

  /* Disable Interrupts in the VIC*/
  VICINTENCLR=0x0;

  /*
  Disconnect PLL if you want to do ISP at crystal frequency.
  Otherwise you need to pass the PLL freq when bootloader goes in
  ISP mode.
  cclk = crystal when PLL is disconnected
  cclk = PLL freq when PLL is connected.
  Disconnecting the PLL is recommended. */
  PLLCON = 0x0;
  PLLFEED = 0xAA;
  PLLFEED= 0x55;

  /*
  Set the VPB divider to 1/4 if your application changes the VPBDIV value.
  The bootloader is hard-coded to use the reset value of VPBDIV register
  VPBDIV = 0x0;
  */
  
  /* Restore reset state of Timer1 */
  TIMER1_PR=0x0;
  TIMER1_MCR=0x0;
  TIMER1_CCR=0x0;

  /* Map bootloader vectors */
  MEMMAP = 0x0;

  /* Point to bootloader entry point i.e. reset vector 0x0 */
  bootloader_entry = (void (*)(void))(0x0);
  bootloader_entry();
}

/*
Invoke the bootloader
The bootloader will read pin P0.14 to detect if ISP is forced
Since P0.14 is configured as an output and set to 0, the bootloader
will go in ISP mode.
*/

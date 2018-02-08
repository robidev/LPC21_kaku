#ifndef UART_USB_H
#define UART_USB_H

#define BAUD_RATE	115200

#define INT_IN_EP		0x81
#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		((x)&0xFF),((x)>>8)

// CDC definitions
#define CS_INTERFACE			0x24
#define CS_ENDPOINT				0x25

#define	SET_LINE_CODING			0x20
#define	GET_LINE_CODING			0x21
#define	SET_CONTROL_LINE_STATE	0x22

#define EOF (-1)

#define	INT_VECT_NUM	2

#define IRQ_MASK 0x00000080


int USB_UART_Init(void);
int VCOM_putchar(int c);
int VCOM_getchar(void);

#endif

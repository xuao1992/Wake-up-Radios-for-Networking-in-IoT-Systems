/*
 * uart.c
 *
 *  Created on: 9 mars 2016
 *      Author: faitaoudia
 */

#include <msp430.h>
#include <misc.h>

#define UART_TX_READY	(UCA1IFG & UCTXIFG)
#define UART_RX_READY	(UCA1IFG & UCRXIFG)
#define UART_TX_DONE	(UCA1IFG & UCTXCPTIFG)
#define UART_RESET_TX_DONE	(UCA1IFG &= ~UCTXCPTIFG)


/*
 * Initialize UART subsystem
 */
void uart_init(void)
{
	// UART (UCA1) port on ETHZ LoRa/WuRx platfrom (June 2016)
	// P2.5 : TX
	// P2.6 : RX
	P2SEL1 |= (BIT5 + BIT6);
	P2SEL0 &= ~(BIT5 + BIT6);

	// - No parity
	// - LSB first
	// - One stop bit
	// - 8 bits data
	// Baudrate:
	UCA1CTLW0 = UCSWRST;
	UCA1CTLW0 = UCSSEL__SMCLK;
	UCA1BR0 = 6;
	UCA1BR1 = 0;
	UCA1MCTLW |= UCOS16 + UCBRF_8;
	UCA1CTLW0 &= ~UCSWRST;
}

/*
 * Send a character
 */
void uart_send_char(char c)
{
	while (!(UART_TX_READY));
	UCA1TXBUF = c;
	while (!(UART_TX_DONE));
	UART_RESET_TX_DONE;
}

/*
 * Send a string
 */
void uart_send_str(char str[])
{
	while (*str != '\0')
	{
		uart_send_char(*str);
		str++;
	}
}

/*
 * Send an intefer
 */
void uart_send_int(int x)
{
	char res[16];
	itoa(x, res, 10);
	uart_send_str(res);
}

/*
 * New line
 */
void uart_new_line(void)
{
	uart_send_char('\n');
	uart_send_char(0xD);
}

/*
 * spi.c
 *
 *  Created on: 29 févr. 2016
 *      Author: faitaoudia
 */

#include <msp430.h>
#include "spi.h"
//#include <stdio.h>

#define BIT_SPI_CS	BIT0
#define BIT_RESET	BIT4
#define BIT_DIO_0	BIT3

void init_spi(void) {
//	Mapping MSP430FR5969 <-> SX1276 on ETH Zurich platform (June 2016)
//                   MSP430FR5969
//                 -----------------
//                |                 |
//                |             P2.0|-> Data Out (UCA0SIMO)
//                |                 |
//                |             P2.1|<- Data In (UCA0SOMI)
//                |                 |
//                |             P1.5|-> Serial Clock Out (UCA0CLK)
//                |                 |
//                |             P3.0|-> CS
//                |                 |
//                |             P1.4|-> RESET
//                |                 |
//                |             P1.3|<-> DIO_0
//                |                 |


	// Configure GPIO
	P1DIR |= BIT_RESET;
	P3DIR |= BIT_SPI_CS;

	P1OUT &= BIT_RESET;

	// TODO
//	P2SEL0 &= ~BIT2;
//	P1SEL1 &= ~(BIT6 | BIT7);
//	P2SEL1 |= BIT2;
//	P1SEL1 |= BIT6 | BIT7;

	P1SEL0 &= ~BIT5;
	P2SEL1 |= (BIT0 | BIT1); // TODO ??
	P1SEL1 |= BIT5;
	P2SEL1 |= (BIT0 | BIT1);

	// Configure SPI
	// Reset mode
	UCA0CTLW0 = UCSWRST;

	// 3 pin
	// SPI master
	// High clock polarity
	// MSB first
	UCA0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB;
	// Sourcing SPI clock from SMCLK (should be at 1 MHz)
	UCA0CTLW0 |= UCSSEL__SMCLK;
	UCA0BR0 = 2;
	UCA0BR1 = 0;
	// Initializing USCI
	UCA0CTLW0 &= ~UCSWRST;

	P3OUT |= BIT_SPI_CS;

	//UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

/*
 * Set a register over the SPI interface
 */
void spi_snd_data(uint8_t reg_add, uint8_t data)
{
	P3OUT &= ~BIT_SPI_CS;

	__delay_cycles(20);

	// But the MSB bit at 1 for write access
	UCA0TXBUF = 0x80u | reg_add;
	while (UCA0STATW & UCBUSY);

	UCA0TXBUF = data;
	while (UCA0STATW & UCBUSY);

	P3OUT |= BIT_SPI_CS;

//	printf("        SPI_SND_DATA: Address: %x , Data: %x\n", reg_add, data);
}

/*
 * Read a register value over the SPI interface
 */
uint8_t spi_rcv_data(uint8_t reg_add)
{
	P3OUT &= ~BIT_SPI_CS;

	__delay_cycles(20);

	UCA0TXBUF = reg_add;
	while (UCA0STATW & UCBUSY);

	UCA0TXBUF = 0x00;
	while (UCA0STATW & UCBUSY);

	P3OUT |= BIT_SPI_CS;

//	printf("SPI_RCV_DATA: Address: %x, Data: %x\n", reg_add, UCA0RXBUF);
	return UCA0RXBUF;

}

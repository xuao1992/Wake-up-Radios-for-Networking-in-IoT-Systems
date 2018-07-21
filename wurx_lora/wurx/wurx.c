/*
 * wurx.c
 *
 *  Created on: 9 mars 2016
 *      Author: faitaoudia
 */

#include <wurx.h>
#include <stddef.h>
#include <msp430.h>

/*
 * Event ID associated to a WuRx interrupt
 */
static uint16_t _wurx_ev;

/*
 * Initialize WuRx communication.
 */
void wurx_init(event_handler_t handler)
{
	// The WuRx is assumed to be connected on P4.5 (ETHZ node, June 2016)
	P4DIR &= ~BIT5;
	P4OUT &= ~BIT5;
	P4IES &= ~BIT5;

	_wurx_ev = event_add(handler);

	wurx_enable_it();
}

// Enable/Disable WuRx interrupts
void wurx_enable_it(void)
{
	P4IE |= BIT5;
}

void wurx_disable_it(void)
{
	P4IE &= ~BIT5;
}

#pragma vector=PORT4_VECTOR
__interrupt void port4_interrupt_handler(void)
{
	if (P4IFG & BIT5)
	{
		EVENT_SIGNAL_ISR(_wurx_ev);

		P4IFG &= ~BIT5;
	}
}

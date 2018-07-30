/*
 * misc.c
 *
 *  Created on: 10 mars 2016
 *      Author: faitaoudia
 */

#include <msp430.h>
#include <misc.h>
#include <stdint.h>

// -----------------------------------------------------------------------
// Implementation of some useful functions
// -----------------------------------------------------------------------
/*
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 *
 * Convert a int to a string.
 */
char* itoa(int value, char* result, int base)
{

        // check that the base if valid
        if (base < 2 || base > 36) { *result = '\0'; return result; }
        char* ptr = result, *ptr1 = result, tmp_char;
        int tmp_value, init_value = value;

        do {
                tmp_value = value;
                value /= base;
                *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
        } while ( value );

        if(init_value<10) {
			*ptr++ = '0';
		}

        // Apply negative sign
        if (tmp_value < 0) *ptr++ = '-';
        *ptr-- = '\0';
        while(ptr1 < ptr) {
                tmp_char = *ptr;
                *ptr--= *ptr1;
                *ptr1++ = tmp_char;
        }

        return result;
}

// --------------------------------------------------------------------------
// LEDs manipulations
// LED 1 : P4.6
// LED 2 : P1.0

// LED 1 : P3.3
// LED 2 : P3.4   //Author: Xu Ao
// --------------------------------------------------------------------------
void leds_init(void)
{
//	P4DIR |= BIT6;
//	P1DIR |= BIT0;
//	P4OUT &= ~BIT6;
//	P1OUT &= ~BIT0;

	P3DIR |= BIT3;
	P3DIR |= BIT4;
	P3OUT &= ~BIT3;
	P3OUT &= ~BIT4;
}

void led1_on(void)
{
//	P4OUT |= BIT6;
    P3OUT |= BIT3;
}

void led1_off(void)
{
//	P4OUT &= ~BIT6;
    P3OUT &= ~BIT3;
}

void led1_long_blink(void)
{
	led1_on();
	__delay_cycles(1000000); // 1 s delay
	led1_off();
}

void led1_fast_double_blink(void)
{
	led1_on();
	__delay_cycles(100000); // 0.2 s delay
	led1_off();
	__delay_cycles(100000); // 0.2 s delay
	led1_on();
	__delay_cycles(100000); // 0.2 s delay
	led1_off();
}

void led2_on(void)
{
//	P1OUT |= BIT0;
    P3OUT |= BIT4;
}

void led2_off(void)
{
//	P1OUT &= ~BIT0;
    P3OUT &= ~BIT4;
}

void led2_blink(void)
{
	led2_on();
	__delay_cycles(1000000); // 1 s delay
	led2_off();
}

// -----------------------------------------------------------------------------
// Button SW1 : P4.5
// Button SW1 : P3.1  Author: Xu Ao
// When it is pressed, the LPM3 mode is left.
// -----------------------------------------------------------------------------
// Event id associated with the button
static uint16_t _sw1_event;

void sw1_init(event_handler_t sw1_handler)
{
//	P4DIR &= ~BIT5;	// Input
//	P4OUT |= BIT5;	// Pull up
//	P4REN |= BIT5;	// Pull-up enabled
//	P4IFG &= ~BIT5;	// Clear interrupts
//	P4IES |= BIT5;	// On a high to low transition...
//	P4IE |= BIT5;	// ...generate an interrupt

    P3DIR &= ~BIT1; // Input
    P3OUT |= BIT1;  // Pull up
    P3REN |= BIT1;  // Pull-up enabled
    P3IFG &= ~BIT1; // Clear interrupts
    P3IES |= BIT1;  // On a high to low transition...
    P3IE |= BIT1;   // ...generate an interrupt


	_sw1_event = event_add(sw1_handler);
}

#define SW1	(!(P3IN & BIT1))

#pragma vector=PORT3_VECTOR
__interrupt void sw1_interrupt_handler(void)
{
	if (P3IFG & BIT1)
	{
		// Wait for the button to be released
		while (SW1);

		// Reset the interrupt flag
		P3IFG &= ~BIT1;

		// Signal the event associated with the button
		EVENT_SIGNAL_ISR(_sw1_event);
	}
}

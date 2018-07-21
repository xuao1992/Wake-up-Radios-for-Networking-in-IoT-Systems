/*
 * misc.h
 *
 *  Created on: 10 mars 2016
 *      Author: faitaoudia
 */

#ifndef MISC_MISC_H_
#define MISC_MISC_H_

#include <event.h>

// --------------------------------------------------------------------------
// LED manipulations
// The used LED is the LED2 (P1.0)
// --------------------------------------------------------------------------
void leds_init(void);

void led1_on(void);

void led1_off(void);

// Make the led blink for 1s
void led1_long_blink(void);
// 2 quick blinks of 0.2s
void led1_fast_double_blink(void);

void led2_on(void);

void led2_off(void);

// Make the led blink for 1/2s
void led2_blink(void);

// -----------------------------------------------------------------------------
// Button SW1
// When it is pressed, the LPM3 mode is left.
// -----------------------------------------------------------------------------
void sw1_init(event_handler_t sw1_handler);

// -----------------------------------------------------------------------
// Implementation of some useful functions
// -----------------------------------------------------------------------
/*
 * Convert a int to a string.
 */
char* itoa(int value, char* result, int base);

// -----------------------------------------------------------------------------
// Some useful tool for manipulating circular buffer as FIFOs
// -----------------------------------------------------------------------------
#define FIFO_FULL(first, last, size)	( ((last + 1) == full) || ((first == 0) && (last == (size-1))) )
#define FIFO_EMPTY(first, last)			(first == last)
#define FIFO_INCR(p, size)				(p = ((p == (size-1)) ? 0 : p + 1))


#endif /* MISC_MISC_H_ */

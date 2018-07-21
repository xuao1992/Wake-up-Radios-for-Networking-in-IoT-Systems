/*
 * uart.h
 *
 *  Created on: 9 mars 2016
 *      Author: faitaoudia
 */

#ifndef UART_UART_H_
#define UART_UART_H_

/*
 * Initialize UART subsystem
 */
void uart_init(void);

/*
 * Send a character
 */
void uart_send_char(char c);

/*
 * Send a string
 */
void uart_send_str(char str[]);

/*
 * Send an intefer
 */
void uart_send_int(int x);

/*
 * New line
 */
void uart_new_line(void);



#endif /* UART_UART_H_ */

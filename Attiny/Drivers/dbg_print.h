/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42, by Joerg Wunsch):
 * <dinuxbg .at. gmail.com> wrote this file.  As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you 
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                             Dimitar Dimitrov
 * ----------------------------------------------------------------------------
 */

#ifndef DBG_PRINT_H
#define DBG_PRINT_H

#include <stdint.h>

#define DBG_UART_TX_PORT	PORTB
#define DBG_UART_TX_DDR		DDRB
#define DBG_UART_TX_PIN		2
#define DBG_UART_BAUDRATE	9600

void dbg_tx_init();
void dbg_putchar(uint8_t c);
void _printf(const char * format, ...);

#define dbg_printf(...) _printf(__VA_ARGS__)

#endif


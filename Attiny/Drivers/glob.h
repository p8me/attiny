
#ifndef GLOB_H
#define GLOB_H

#include "setts.h"
#include "inttypes.h"
#include <stdbool.h>

#define CPU_PRESC	0
#define F_CPU		(8000000 / (1 << CPU_PRESC))

void delay_ms(uint16_t ms);

#define set_bit_to(REG, BIT, TO)	REG ^= (-(TO) ^ (REG)) & _BV(BIT)
#define set_bit(REG, BIT)			REG |= _BV(BIT)
#define clr_bit(REG, BIT)			REG &= ~_BV(BIT)
#define tgl_bit(REG, BIT)			REG ^= _BV(BIT)
#define is_bit_set(REG, BIT)		(REG & _BV(BIT))
#define is_bit_clr(REG, BIT)		(!is_bit_set(REG, BIT))

#define set_pin_to(PIN, TO)			set_bit_to(PORTB, PIN, TO)
#define set_pin(PIN)				set_bit(PORTB, PIN)
#define clr_pin(PIN)				clr_bit(PORTB, PIN)
#define tgl_pin(PIN)				tgl_bit(PORTB, PIN)
#define is_pin_set(PIN)				is_bit_set(PINB, PIN)
#define is_pin_clr(PIN)				is_bit_clr(PINB, PIN)

#define  BLINK_MS(pin, time_ms) {set_bit(PORTB, pin); delay_ms(time_ms); clr_bit(PORTB, pin); delay_ms(time_ms);};
#define _BLINK(pin, time_ms) {clr_bit(PORTB, pin); delay_ms(time_ms); set_bit(PORTB, pin); delay_ms(time_ms);};

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define TINY13 (defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__))

#endif

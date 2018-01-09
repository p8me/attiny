
#include "glob.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void (*pc_int_cb)(bool);

volatile uint8_t int_pin;

void enable_pc_int(uint8_t pin, void (*cb)(bool)){
	PCMSK = (1<<pin);
	GIMSK |= (1<<PCIE);
	sei();
	pc_int_cb = cb;
	int_pin = pin;
}

#if PCINT

ISR(PCINT0_vect){
	pc_int_cb(PINB & (1<<int_pin));
	GIFR |= (1<<PCIF);
}

#endif

// ============= EXT INT0 ================
//set_bit(GIMSK, INT0);
//set_bit(MCUCR, ISC01);


//ISR(INT0_vect){
//set_bit(GIFR, INTF0);
//}

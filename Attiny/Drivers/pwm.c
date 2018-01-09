
#include <avr/io.h>
#include <avr/common.h>
#include "glob.h"
#include <util/delay.h>
#include "wdt.h"

#if OC0AB > 0

void pwm_init(void){
	TCCR0A =
#if OC0AB >= 1
		_BV(COM0A1) +
#endif
#if OC0AB >= 2
		_BV(COM0B1) +
#endif
		_BV(WGM00);  // phase correct PWM mode

	DDRB |=
#if OC0AB >= 1
		_BV(0) +
#endif
#if OC0AB >= 2
		_BV(1) +
#endif
	0 ;

	TCCR0B = _BV(CS01);   // clock source = CLK/1, start PWM
	disable_deep_sleep();
}

void pwm_disable(void){
	TCCR0A = 0;
}

bool is_pwm_disabled(void){
	return TCCR0A == 0;
}

// requires OC0AB = 2 
void num_to_light(uint8_t num){
	if (num < 128){ OCR0A = num; OCR0B = 0;}
	else          { OCR0A = 128; OCR0B = num - 128;}
}

void soft_blink(void){
	for (uint8_t i = 0; i < 150; i++){
		OCR0A  = i;
		OCR0B  = 150 - i;
		_delay_ms(20);
	}
	for (uint8_t i = 0; i < 150; i++){
		OCR0A  = 150 - i;
		OCR0B  = i;
		_delay_ms(20);
	}
}

#endif

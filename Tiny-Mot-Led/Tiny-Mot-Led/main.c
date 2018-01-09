
#include "main.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/common.h>
#include <avr/cpufunc.h>
#include <avr/fuse.h>
#include <avr/eeprom.h>
#include <avr/signature.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <math.h>
#include <stdbool.h>
#include "dbg_print.h"
#include "utils.h"
#include <stdlib.h>

void num2_light(uint8_t num);
void soft_blink(void);

#define TINY13 (defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__))

uint8_t read_adc(uint8_t ch){
	set_bit(ADCSRA, ADEN);
	ADMUX = (ADMUX & ~15) + ch;
	ADCSRA |= _BV(ADSC);         // start ADC measurement
	while (ADCSRA & _BV(ADSC) ); // wait till conversion complete
	clr_bit(ADCSRA, ADEN);
	#if(!TINY13)
	//set_bit(PRR, PRADC);
	#endif
	return ADCH;
}


uint16_t wdt_per;
uint32_t time = 0;

#if(TINY13)
	#define ADC_INT_1V1 REFS0
#else
	#define ADC_INT_1V1 REFS1
#endif

void adc_init(){
	ADMUX = _BV(ADLAR);// + _BV(ADC_INT_1V1); // MUX
	ADCSRA = 1; // 2^1 PRESC
}

bool active = false;

#define ADC_ACTIVATE_PIN	PB3

int main(void)
{
	PORTB = 0;
	#if(!TINY13)
	dbg_tx_init();
	#endif

	// ============= Clock ================
	CLKPR = _BV(CLKPCE); CLKPR = CPU_PRESC;

	// ============= I/O ================
	DDRB |= _BV(0) | _BV(1) | _BV(2) | _BV(3);

	set_bit(PORTB, ADC_ACTIVATE_PIN);

	// ============= Power ================
	//PRR |= _BV(PRADC) | _BV(PRUSI) | _BV(PRTIM0) | _BV(PRTIM1); // Power reduction: Trun everything off
	MCUCR |= _BV(SE); // Enable sleep mode
	POWER_DOWN_EN();

	// ============= EXT INT ================
	//set_bit(GIMSK, INT0);
	//set_bit(MCUCR, ISC01);

	// ============= Watchdog ================
	SET_NEXT_WAKE(2000);
	sei();
	
	// ============= PWM ================
	//TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);  // phase correct PWM mode
	//TCCR0B = _BV(CS00);   // clock source = CLK/1, start PWM

	// ============= ADC ================
	adc_init();
	
	clr_bit(PORTB, 0); clr_bit(PORTB, 1); clr_bit(PORTB, 2);

    while(1){
		sleep_cpu();
	}
}

uint8_t rnd = 0;

#define	MOT_LIM		500

uint8_t detect_motion(uint8_t ch){
	const uint32_t bayes_rat = 99;
	static uint16_t bayes_adc = 0;

	clr_bit(PORTB, ADC_ACTIVATE_PIN);
	uint16_t val = (uint16_t)read_adc(ch) * 256;
	set_bit(PORTB, ADC_ACTIVATE_PIN);

	bayes_adc = ((uint32_t)bayes_adc * bayes_rat + val) / (bayes_rat + 1);
	uint16_t dif = bayes_adc > val ? bayes_adc - val : val - bayes_adc;

	rnd = bayes_adc;

	//dbg_printf("%10u, %5u\n", bayes_adc, dif);

	if (dif > MOT_LIM){
		bayes_adc = val;
		return dif / MOT_LIM;
	}
	return 0;
}

uint8_t led = 0;
uint8_t cnt = 0;

//dbg_printf("%5d\n", read_adc(1));
//OCR0A = read_adc(1);

ISR(WDT_vect){
	time += wdt_per;
	
	#define MIN_IDLE_TIME_MS	500

	uint8_t mot = detect_motion(2);
	static uint16_t last_idle;
	if (cnt == 0 && mot > 0 && time > last_idle + (MIN_IDLE_TIME_MS/32)){
		cnt = min(mot, 3) * 10;
		SET_NEXT_WAKE(125);
		set_bit(WDTCR, 7);
	}
	if (cnt > 0){
		cnt--;
		set_bit(PORTB, led);
		_delay_ms(3);
		clr_bit(PORTB, led);
		if(++led == 3) led = 0;
		if (cnt == 0){
			last_idle = time;
			SET_NEXT_WAKE(250);
		}
	}
}

ISR(INT0_vect){
	// POWER_DOWN_EN();
	set_bit(GIFR, INTF0);
}

/*

void num2_light(uint8_t num){
	if (num < 128){ OCR0A = num; OCR0B = 0;}
	else          { OCR0A = 128; OCR0B = num;}
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

*/

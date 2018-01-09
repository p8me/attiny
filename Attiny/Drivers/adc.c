
#include <avr/common.h>
#include <stdlib.h>
#include <avr/io.h>
#include "glob.h"

#if(TINY13)
#define ADC_INT_1V1 REFS0
#else
#define ADC_INT_1V1 REFS1
#endif

/*     (PCINT5/RESET/ADC0/dW) PB5 -|1 --\/-- 8|- VCC
(PCINT3/XTAL1/CLKI/OC1B/ADC3) PB3 -|2        7|- PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
(PCINT4/XTAL2/CLKO/OC1B/ADC2) PB4 -|3        6|- PB1 (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
                              GND -|4--------5|- PB0 (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0)  */
const uint8_t adc_pin_2_ch[] = {-1, -1, 1, 3, 2, 0};
#define ADC_PIN_2_CH(x) adc_pin_2_ch[x]

void adc_init(void){
	ADMUX = _BV(ADLAR);// + _BV(ADC_INT_1V1); // MUX
	ADCSRA = 1; // 2^1 PRESC
}

// A light sensor must be connected between adc_pin and GND
uint8_t read_adc(uint8_t adc_pin){
	set_bit(ADCSRA, ADEN);
	ADMUX = (ADMUX & ~15) + ADC_PIN_2_CH(adc_pin);
	set_pin(adc_pin);
	ADCSRA |= _BV(ADSC);         // start ADC measurement
	while (ADCSRA & _BV(ADSC) ); // wait till conversion complete
	clr_bit(ADCSRA, ADEN);
	clr_pin(adc_pin);
	return ADCH;
}

// A light sensor must be connected between adc_pin and GND
uint8_t detect_motion(uint8_t adc_pin, const uint8_t mot_lim){
	static uint8_t old_val = 0;

	uint8_t val = read_adc(adc_pin);

	uint16_t dif = old_val > val ? old_val - val : val - old_val;

	if (dif > mot_lim){
		old_val = val;
		return dif;
	}
	return 0;
}

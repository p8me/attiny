
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
	//ADMUX =
	 //+ _BV(ADLAR) // ADCH will contain high 8 bits out of 10
	 //+ _BV(ADC_INT_1V1); // 1V1 REF
	ADCSRA = 1; // 2^1 PRESC
}

// A light sensor must be connected between adc_pin and GND
uint16_t read_adc(uint8_t adc_pin){
	set_bit(ADCSRA, ADEN);
	ADMUX = (ADMUX & 0xF0) + ADC_PIN_2_CH(adc_pin);
	set_pin(adc_pin);
	ADCSRA |= _BV(ADSC);         // start ADC measurement
	while (ADCSRA & _BV(ADSC) ); // wait till conversion complete
	clr_bit(ADCSRA, ADEN);
	clr_pin(adc_pin);
	uint16_t low = ADCL;
	return low + (ADCH << 8);
}


uint16_t read_capacitance(uint8_t adc_pin){
	set_bit(ADCSRA, ADEN);

	set_pin(adc_pin);
	ADMUX = (ADMUX & 0xF0) | 0b1101;
	ADCSRA |= _BV(ADSC);         // start ADC measurement
	while (ADCSRA & _BV(ADSC) ); // wait till conversion complete
	clr_pin(adc_pin);
	ADMUX = (ADMUX & 0xF0) + ADC_PIN_2_CH(adc_pin);
	ADCSRA |= _BV(ADSC);         // start ADC measurement
	while (ADCSRA & _BV(ADSC) ); // wait till conversion complete
	clr_bit(ADCSRA, ADEN);
	
	uint16_t low = ADCL;
	return low + (ADCH << 8);
}

#define NUM_ADCS	4

uint8_t old_val[NUM_ADCS];

// A light sensor must be connected between adc_pin and GND
// If mot_lim < 0, the change will be reset
uint8_t detect_adc_change(uint8_t adc_pin, const int8_t mot_lim){
	
	uint8_t val = read_adc(adc_pin);
	if (mot_lim < 0){
		old_val[adc_pin] = val;
		return 0;
	}
	uint16_t dif = old_val[adc_pin] > val ? old_val[adc_pin] - val : val - old_val[adc_pin];
	if (dif > mot_lim){
		old_val[adc_pin] = val;
		return dif;
	}
	return 0;
}


// If mot_lim < 0, the change will be reset
uint8_t detect_cap_change(uint8_t adc_pin, const int8_t mot_lim){
	
	uint8_t val = read_capacitance(adc_pin);
	if (mot_lim < 0){
		old_val[adc_pin] = val;
		return 0;
	}
	uint16_t dif = old_val[adc_pin] > val ? old_val[adc_pin] - val : val - old_val[adc_pin];
	if (dif > mot_lim){
		old_val[adc_pin] = val;
		return dif;
	}
	return 0;
}

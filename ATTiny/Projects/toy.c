
#include "main.h"

/* Description and Hardware

Mobile

setts:
#define FIXED_WDT_PER	WDT_1000ms

#define WDT			1
#define PCINT		0
#define OC0AB		0

*/

#define LED_CENT				3

#define MOT_VIB					2

#define ADC_PIN					4

#define DAY_LIM					80 // the higher longer the day

#define IS_DAY				(read_adc(ADC_PIN) < DAY_LIM)

void wdt_event(void)
{
	if (IS_DAY && detect_motion(ADC_PIN, 2)){
		cli();
		//set_pin(MOT_VIB);
		OCR0B = 90;
		pwm_init();
		for (uint8_t i = 0; i < 255; i++){
			OCR0A  = i;
			_delay_ms(2);
		}
		for (uint8_t i = 0; i < 255; i++){
			OCR0A  = 255 - i;
			_delay_ms(2);
		}
		OCR0B = 0;
		pwm_disable();
		clr_pin(MOT_VIB);
		sei();
		reset_motion(ADC_PIN);
	}
}


void loop(void){
	
}

void init(){
	OUT_PINS(LED_CENT, MOT_VIB);
	
	adc_init();
	wdt_setup();
	pwm_init();
}

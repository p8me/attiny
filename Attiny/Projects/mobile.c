
#include "main.h"

/* Description and Hardware

Mobile

setts:
#define FIXED_WDT_PER	WDT_1000ms

#define WDT			1
#define PCINT		0
#define OC0AB		0

*/

#define LED_CENT				0
#define LED_PERIPH				1
#define MOT_VIB					2
#define MOT_ROT					3


#define ADC_PIN					4

#define DAY_LIM					80 // the higher longer the day

void wdt_event(void)
{
	bool day = read_adc(ADC_PIN) < DAY_LIM;
	//if (read_adc(ADC_PIN) < DAY_LIM)
	//set_pin(VIB_MOT_PIN);
	//else
	//clr_pin(VIB_MOT_PIN);
	
	//set_pin_to(LED_CENT, day);

	REPEATE_EVERY_XSEC(4, BLINK_MS(LED_CENT, 10));
	REPEATE_EVERY_XSEC(5, BLINK_MS(LED_PERIPH, 10));
	if (day){
		REPEATE_EVERY_XSEC(10, BLINK_MS(MOT_ROT, 200));
		REPEATE_EVERY_XSEC(8, BLINK_MS(MOT_VIB, 2000));
	}
}


void loop(void){
	
}

void init(){
	OUT_PINS(LED_CENT, LED_PERIPH, MOT_ROT, MOT_VIB);
	
	adc_init();
	wdt_setup();
	//pwm_init();
}

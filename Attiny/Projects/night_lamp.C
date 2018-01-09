
#include "main.h"

/* Description and Hardware

Night Lamp

setts:
#define FIXED_WDT_PER	WDT_64ms

#define WDT			1
#define PCINT		0
#define OC0AB		1

*/

// PIN 0 will be OCR0A
#define ADC_PIN					4
#define TOUCH_PIN				3

#define NUM_STATS				6
uint8_t level[NUM_STATS] = {0, 10, 30, 100, 135, 255};
uint8_t stat = 0;

void wdt_event(void)
{
	
	if (read_adc(ADC_PIN) < 80) {
		stat = 0;
		OCR0A = 0;
		return;
	}

	set_bit(PORTB, TOUCH_PIN);
	_delay_us(1);
	bool touch = is_pin_clr(TOUCH_PIN);
	clr_bit(PORTB, TOUCH_PIN);

	if (touch) REPEAT_AFTER_AT_LEAST_MS(700, {
		if (++stat == NUM_STATS) stat = 0;
		OCR0A = level[stat];
	});
}


void loop(void){}

void init(){
	
	adc_init();
	wdt_setup();
#if (OC0AB > 0)
	pwm_init();
#endif
}

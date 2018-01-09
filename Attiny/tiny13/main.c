
#include "main.h"

/* Description and Hardware

Night Lamp 2

setts:
#define FIXED_WDT_PER	WDT_32ms

#define WDT			1
#define PCINT		0
#define OC0AB		1

*/

// PIN 0 will be OCR0A
#define TOUCH_PIN				3
uint8_t level = 0;

uint32_t count_on = 0;
uint16_t count_lock = 0;
#define CONN_TIMEOUT_SECS	9

uint32_t command_ts = 0;

bool incr = true;

void wdt_event(void)
{
	set_bit(PORTB, TOUCH_PIN);
	_delay_us(1);
	bool touch = is_pin_clr(TOUCH_PIN);
	clr_bit(PORTB, TOUCH_PIN);
	
	if (count_lock) count_lock--;
	else if (touch){
		if ((ts - command_ts) > MS_TO_TS(2000) && OCR0A > 0) {OCR0A = 0; incr = true; pwm_disable(); count_lock = 10;}
		else{
			if (incr) OCR0A += 10;
			else OCR0A -= 10;
			if (OCR0A == 250) {incr = false; count_lock = 10;}
			if (OCR0A == 0) {OCR0A = 0; incr = true; pwm_disable(); count_lock = 10;}
			else if (is_pwm_disabled()) pwm_init();
			count_on = CONN_TIMEOUT_SECS*wdt_frq_hz;
			command_ts = ts;
		}
	}
	
	if (count_on) count_on--;
	else {OCR0A = 0; incr = true; pwm_disable();}
}

void loop(void){}

void init(){
	
	adc_init();
	wdt_setup();
#if (OC0AB > 0)
	pwm_init();
#endif
}

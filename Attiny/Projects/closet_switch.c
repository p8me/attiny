#include "main.h"

/* Description and Hardware

Closet Switch

DRIVE_PIN is connected to N-CH MOSFET.
SW_PIN is low sensitive and is the switch pin

setts:
#define FIXED_WDT_PER	WDT_250ms

*/

#define DRIVE_PIN	0
#define SW_PIN		4

#define DISC_TIMEOUT_SECS	5
#define CONN_TIMEOUT_SECS	15
#define ERR_CLR_SECS		2

bool led_is_on = 0;
uint8_t count_off = 0; // when led_on and no sw counts up to 15 secs and turns off led and sets itself to 0.
uint16_t count_on = 0; // when led_on regardless of sw, counts up to a 15 and turns off the leds, it's set to 0 whenever leds are off

uint8_t sw_error = 0;

void led_off(){
	led_is_on = 0;
	count_on = 0;
	count_off = 0;
	uint8_t c;
	for (c = 50; c > 0; c -= 10) _BLINK(DRIVE_PIN, c);
	clr_pin(DRIVE_PIN);
}

void led_on(){
	led_is_on = 1;
	set_pin(DRIVE_PIN);
}

void wdt_event(void)
{
	bool sw = is_pin_clr(SW_PIN);
	
	if (led_is_on){
		if (++count_on == CONN_TIMEOUT_SECS*wdt_frq_hz){
			led_off();
			sw_error = ERR_CLR_SECS*wdt_frq_hz;
		}
	}
	if (sw){
		if (!sw_error){
			count_off = 0;
			if (!led_is_on) led_on();
		}
	}
	else{
		if (sw_error) sw_error--;
		if (led_is_on){
			if (++count_off == DISC_TIMEOUT_SECS*wdt_frq_hz) led_off();
		}
	}
}

void loop(void){}

void init(){
	OUT_PINS(DRIVE_PIN);
	HIGH_PINS(SW_PIN);

	wdt_setup();
}

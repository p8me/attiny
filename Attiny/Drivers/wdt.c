
#include "wdt.h"
#include "glob.h"
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef WDIE
#define WDIE 6
#endif

#if (!FIXED_WDT_PER)

volatile uint16_t wdt_per_ts = 0;
volatile uint8_t wdt_frq_hz = 0;

void set_wdt_period(uint8_t wk_ms){
	WDTCR = _BV(WDIE) + wk_ms;
	wdt_per_ts = (uint16_t)(1) << wk_ms;
	wdt_frq_hz = 64 >> wk_ms;
}

#endif

volatile uint32_t ts = 0; // timestamp

void wdt_setup(){
	MCUCR |= _BV(SE); // Enable sleep mode
	sei();
	#if FIXED_WDT_PER
	WDTCR = _BV(WDIE) + FIXED_WDT_PER;
	#endif
}


bool wdt_event_f = false;

bool wdt_timeout_happened(void){
	if (wdt_event_f){
		wdt_event_f = 0;
		return 1;
	}
	return 0;
}

#if WDT

ISR(WDT_vect){
	ts += wdt_per_ts;
	wdt_event_f = true;
}

void disable_deep_sleep(void){
	clr_bit(MCUCR, SM1); // Disable deep sleep (CPU usage jumps from 5uA tp 800uA)
}

void enable_deep_sleep(void){
	set_bit(MCUCR, SM1); // Enable deep sleep (CPU usage 5uA)
}

#endif
#include "main.h"

/* Description and Hardware

Curtains

M1_PIN and M2_PIN are Motor drive pins.
SW_PIN is low sensitive switch to detect end of drapes
ADC_DRIVE_LED_PIN is adc drive pin and LED indicator pin

setts:
#define FIXED_WDT_PER	WDT_250ms

#define WDT			1
#define PCINT		0
#define OC0AB		0

TODO:
	adc_read no longer needs an activation pin
	light sensor should be connected between GND and adc_pin
	ADC_CH is no longer used only ADC_PIN

*/

#define M1_PIN					0
#define M2_PIN					1
#define SW_PIN					2
#define ADC_DRIVE_LED_PIN		3
#define ADC_CH					2 // Ch2 on pin4

void close_drapes(bool is_night){
	if (is_night){
		set_bit(PORTB, M1_PIN);
		clr_bit(PORTB, M2_PIN);
	}
	else{
		clr_bit(PORTB, M1_PIN);
		set_bit(PORTB, M2_PIN);
	}
}

void error(){
	cli();
	set_bit(PORTB, ADC_DRIVE_LED_PIN);
	sleep_cpu();
}


#define MIN_TS_BET_DAY_NIGHT	MS_TO_TS(3000)

#define MAX_ACTION_TIME_TS		MS_TO_TS(120000) // 120 secs
#define MIN_ACTION_TIME_TS		MS_TO_TS(30000) // 30 secs

#define DAY_NIGHT_LIM	50 // 0-255, increase to have more day time

#define TEST_LIGHT		0

volatile int32_t last_motor_ds = -MIN_TS_BET_DAY_NIGHT;
volatile uint8_t bad_dir_debounce = 0, right_dir_debounce;
volatile bool is_night = false, motor_active_f = false;
volatile bool drapes_are_open = false;
volatile bool drapes_are_closed = false;
volatile uint8_t night_cnt, day_cnt;



void stop_motor(){
	clr_bit(PORTB, M1_PIN);
	clr_bit(PORTB, M2_PIN);
}

void stop_action(uint8_t n, uint8_t del){
	cli();
	uint16_t tmp = 5 * del;
	stop_motor();
	drapes_are_open = !is_night;
	drapes_are_closed = is_night;
	motor_active_f = false;
	for (uint8_t i = 0; i < n; i++){
		clr_bit(PORTB, ADC_DRIVE_LED_PIN);
		delay_ms(del);
		set_bit(PORTB, ADC_DRIVE_LED_PIN);
		delay_ms(tmp);
	}
	sei();
}

void wdt_event(void)
{
	#if TEST_LIGHT
	close_drapes(read_adc(ADC_CH, ADC_DRIVE_LED_PIN) > DAY_NIGHT_LIM);
	delay_ms(1);
	stop_motor();
	#endif
	if (motor_active_f || ts - last_motor_ds < MIN_TS_BET_DAY_NIGHT) return;
	if (read_adc(ADC_CH, ADC_DRIVE_LED_PIN) > DAY_NIGHT_LIM){
		day_cnt = 0;
		if (!drapes_are_closed){
			if (night_cnt == 2*wdt_frq_hz){
				motor_active_f = true;
				is_night = true;
			}
			else night_cnt++;
		}
	}
	else{
		night_cnt = 0;
		if (!drapes_are_open){
			if (day_cnt == 2*wdt_frq_hz){
				motor_active_f = true;
				is_night = false;
			}
			else day_cnt++;
		}
	}
}


void loop(void){
	if (motor_active_f){
		bad_dir_debounce = 0;
		right_dir_debounce = 0;
		last_motor_ds = ts;
		close_drapes(is_night);
		while(true){
			uint32_t motor_dur = ts - last_motor_ds;
			bool reached_end_f = is_bit_clr(PINB, SW_PIN);
			// Timeout Check
			if (motor_dur > MAX_ACTION_TIME_TS){
				// took too long, something is wrong with the hardware system
				stop_action(20, 10);
				error();
				return;
			}
			// Wrong dir Check
			if (!drapes_are_open && !drapes_are_closed && 
				motor_dur < MIN_ACTION_TIME_TS && reached_end_f){
				if (++bad_dir_debounce > 2*wdt_frq_hz){ // wrong direction
					// already at the end
					stop_action(5, 10);
					return;
				}
			}
			// Expected action
			if (motor_dur > MIN_ACTION_TIME_TS && reached_end_f){
				if (++right_dir_debounce > 1){
					// reached end
					stop_action(2, 200);
					return;
				}
			}
			sleep_cpu();
		}
	}
}


void init(){
	OUT_PINS(M1_PIN, M2_PIN, ADC_DRIVE_LED_PIN);
	HIGH_PINS(SW_PIN, ADC_DRIVE_LED_PIN);
	
	adc_init();
	wdt_setup();
}

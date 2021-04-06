
#include "main.h"

/* Description and Hardware

Timer-based Night Lamp

setts:
#define FIXED_WDT_PER	WDT_32ms

#define WDT			1
#define PCINT		0
#define OC0AB		1

*/

// PIN 0 will be OCR0A
#define TOUCH_PIN				3
#define TIMEOUT_SECS			(uint32_t)(30*60)//(30*60)
#define TIMEOUT_SECS_AT_MAX		(uint32_t)(10*60*60)
#define LOCK_SECS				1
#define TURNOFF_TOUCH_SECS		2

#define NIGHT_TIME_SECS			(uint32_t)(10)//(8*60*60)
#define DAY_TIME_SECS			(uint32_t)(5)//(1*60*60)

uint32_t turn_back_on_counter = 0, turn_back_off_counter = 0;

uint32_t command_ts = 0;
uint32_t count_on = 0;
uint16_t count_lock = 0;

bool turning_on = true;
bool timed_out = false;
bool turn_back_on = false;
bool turn_back_off = false;

#define TURN_BACK_ON_MORNING	false
#define STAY_ON_AT_MAX			true

#define		MAX_PWM		110

// Returns 1 when pwm is 0
// returns 2 when pwm is MAX_PWM
static uint8_t change_brightness(bool increase){
	int16_t pwm = OCR0A;
	uint8_t diff = max(pwm/8, 1);
	if(increase)
	pwm += diff;
	else
	pwm -= diff;
	if(pwm < 0){
		OCR0A = 0;
		return 1;
	}
	if(pwm > MAX_PWM){
		OCR0A = MAX_PWM;
		return 2;
	}
	OCR0A = pwm;
	return 0;
}

void wdt_event(void)
{
	set_bit(PORTB, TOUCH_PIN);
	_delay_us(1);
	bool touch = is_pin_clr(TOUCH_PIN);
	clr_bit(PORTB, TOUCH_PIN);
	
	if (count_lock) count_lock--;
	else if (touch){
		turn_back_on = false;
		turn_back_off = false;
		timed_out = false;
		if ((ts - command_ts) > MS_TO_TS(TURNOFF_TOUCH_SECS*1000) && OCR0A > 0) {
			OCR0A = 0;
			turning_on = true;
			pwm_disable();
			count_lock = LOCK_SECS*wdt_frq_hz;
			timed_out = true;
		}
		else{
			uint8_t r = change_brightness(turning_on);
			if (r == 2) { // Reached MAX
				turning_on = false;
				count_lock = LOCK_SECS*wdt_frq_hz;
				pwm_disable();
				delay_ms(200); // Notify the user it is max
				pwm_init();
			}
			else if (r == 1) {OCR0A = 0; turning_on = true; pwm_disable(); count_lock = LOCK_SECS*wdt_frq_hz;}
			else if (is_pwm_disabled()) pwm_init();
			
			count_on = r == 2 ? TIMEOUT_SECS_AT_MAX*wdt_frq_hz : TIMEOUT_SECS*wdt_frq_hz;
			
			command_ts = ts;
		}
	}
	
	if (!timed_out){
		if (count_on) count_on--;
		else {
			if(OCR0A == 0){
				turning_on = true;
				pwm_disable();
				timed_out = true;
				turn_back_on = true;
				turn_back_on_counter = NIGHT_TIME_SECS*wdt_frq_hz;
			}
			else OCR0A--;
		}
	}
#if (TURN_BACK_ON_MORNING)
	if (turn_back_on){
		if(turn_back_on_counter) turn_back_on_counter--;
		else{
			if (is_pwm_disabled()) pwm_init();
			if(OCR0A == MAX_PWM){
				turn_back_on = false;
				turn_back_off = true;
				turn_back_off_counter = DAY_TIME_SECS*wdt_frq_hz;
			}
			else OCR0A++;
		}
	}
	if(turn_back_off){
		if(turn_back_off_counter) turn_back_off_counter--;
		else{
			turn_back_off = false;
			OCR0A = 0;
			pwm_disable();
		}
	}
#endif
}

void loop(void){}

void init(){
	//adc_init();
	wdt_setup();
	#if (OC0AB > 0)
	pwm_init();
	#endif
}

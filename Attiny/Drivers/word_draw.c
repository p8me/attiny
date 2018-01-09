
#include "setts.h"
#ifdef WORD_DRAW
#include <avr/common.h>
#include <stdlib.h>
#include <avr/io.h>
#include "glob.h"
#include <stdbool.h>
#include "wdt.h"
#include <util/delay.h>
#include <avr/interrupt.h>


#if (!TINY13)

const uint8_t TONE_PIN[2] = {0, 1};

void start_tone(uint8_t idx){
	if (idx == 0) TCCR0B = 2; // presc=8 -> 1MHz
	else TCCR1 = 4; // presc=8 -> 1MHz
}

void stop_tone(uint8_t idx){
	if (idx == 0) TCCR0B = 0;
	else TCCR1 = 0;
}


void music_init(void){
	TIMSK = _BV(TOIE0) | _BV(TOIE1);
	DDRB |= _BV(TONE_PIN[0]) | _BV(TONE_PIN[1]);
	disable_deep_sleep();
}

float tone_on[2];
uint16_t goal_ovf[2] = {0};
float tone_slope[2] = {0};
bool tone_flag[2], dir[2];

static void Draw(uint8_t idx, uint16_t start, float slope){
	tone_on[idx] = start;
	tone_slope[idx] = slope;
}

void set_timer(uint8_t idx, uint16_t time){
	goal_ovf[idx] = (time >> 8);
	uint8_t tmp = 255 - (time % 256);
	if (idx == 0) TCNT0 = tmp;
	else TCNT1 = tmp;
}

inline void tone_isr(uint8_t idx){
	if (goal_ovf[idx] > 0) goal_ovf[idx]--;
	else{ // timer expired
		if (tone_flag[idx]){
			set_timer(idx, tone_on[idx]);
			set_pin_to(TONE_PIN[idx], 0);
		}
		else{
			set_timer(idx, tone_on[idx]);
			set_pin_to(TONE_PIN[idx], 1);
			//tone_on[idx] *= 1.001;
			tone_on[idx] += tone_on[idx]*tone_on[idx]*tone_slope[idx];
		}
		tone_flag[idx] = !tone_flag[idx];
	}
}

ISR(TIM0_OVF_vect){
	tone_isr(0);
}

ISR(TIM1_OVF_vect){
	tone_isr(1);
}

#define SL(diff,time)	(diff/time)/17000000.

// 833, 290

const uint16_t tone1[] = {			500,		600,		0,		500,		600,		0};
const float tone1_slope[] = {		SL(100,1),	0,			0,		SL(100,8),	SL(-100,8),	0};
const uint8_t tone1_duration[] = {	1,			13,			5,		8,			8,			10};
const uint16_t tone2[] = {			700,		500,		0,		500,		800,		0};
const float tone2_slope[] = {		SL(-400,2.),0,			0,		SL(400,12.),SL(-100,12.),0};
const uint8_t tone2_duration[] = {	2,			20,			10,		12,			12,			10};


uint16_t tone1_idx = 0, tone2_idx = 0, tone1_dur = 0, tone2_dur = 0;

#define DL(TIME)  {_delay_ms(TIME*100);}

void draw_sound_words(void){

	if (tone1_dur == 0){
		if (tone1_idx == sizeof(tone1)/sizeof(uint16_t))
		{tone1_idx = 0; tone2_idx = 0; tone1_dur = 0; tone2_dur = 0;}
		
		if (tone1[tone1_idx]) {
			Draw(0, tone1[tone1_idx], tone1_slope[tone1_idx]);
			start_tone(0);
		}
		else stop_tone(0);
		tone1_dur = tone1_duration[tone1_idx] - 1;
		tone1_idx++;
	}
	else tone1_dur--;
	return; // doesn't work for 2!
	
	if (tone2_dur == 0){
		if (tone2_idx == sizeof(tone2)/sizeof(uint16_t))
		{tone1_idx = 0; tone2_idx = 0; tone1_dur = 0; tone2_dur = 0;}
		if (tone2[tone2_idx]) {
			Draw(1, tone2[tone2_idx], tone2_slope[tone2_idx]);
			start_tone(1);
		}
		else stop_tone(1);
		tone2_dur = tone2_duration[tone2_idx] - 1;
		tone2_idx++;
	}
	else tone2_dur--;
}

#endif
#endif // WORD_DRAW
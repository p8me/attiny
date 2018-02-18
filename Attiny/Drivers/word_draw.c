
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
			//set_timer(idx, tone_on[idx]);
			set_timer(idx, 1);
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

//====================== MID LEVEL =========================

#define RES			3 // pixel-res = RES x RES

#define VERT_TIME	2	// for one pixel
#define HORZ_TIME	7	// for one pixel

#define	MX			400. // bottom
#define MN			250. // top

float vert_vals[RES] = {MN, (float)2/(1/MX+1/MN), MX};

// send 0 0 to get a space
// gets point and calculates period, slope and duration
void draw_coor(const uint8_t * point, uint16_t * period, float * slope, uint8_t * duration){
	if (point[0] == 0 && point[1] == 0){
		*period = 0;
		*duration = RES * HORZ_TIME; // Silence
	}
	else{
		int8_t s_col = point[0]%RES;
		int8_t e_col = point[1]%RES;
		if(s_col > e_col) {
			*period = 0;
			*duration = RES * HORZ_TIME;
		} // error
		else{
			int8_t s_row = point[0]/RES;
			int8_t e_row = point[1]/RES;
			if (s_col == e_col) *duration = VERT_TIME;
			else				*duration = (e_col - s_col) * HORZ_TIME;
			*period = vert_vals[s_row]; // starting period
			*slope = (vert_vals[e_row] - vert_vals[s_row]) / (float)*duration / 22000000.;
		}
	}
}

#define W	0,7, 7,2, 0,7, 7,2
#define V	0,7, 7,2
#define L	0,6, 6,8
#define S	0,0
#define H	0,6, 3,5, 0,6
#define Y	0,5, 4,7, 3,2

const uint8_t coords[] = {W,S,V,S,L,S,H,S,Y,S,S};

// Possible single speaker:
// English: LUVNMWJHTY , hij
// persian 2 6 7 8
// greek: ? ? ?

/*
0 1 2 // MN	 \/
3 4 5 // MD	 \/
6 7 8 // MX	 \/
*/

struct  
{
	uint16_t idx;
	uint16_t period;
	float slope;
	uint8_t duration;
} state[2];


void process_draw_sound_words(void){

	if (state[0].duration == 0){
		if (state[0].idx == (sizeof(coords)/sizeof(coords[0])))
		{state[0].idx = 0; state[0].duration = 0;}
		
		draw_coor(&coords[state[0].idx], &state[0].period, &state[0].slope, &state[0].duration);
		
		if (state[0].period) {
			Draw(0, state[0].period, state[0].slope);
			//Draw(0, 300, 200./170000000.);
			start_tone(0);
		}
		else stop_tone(0);
		
		state[0].idx += 2;
	}
	else state[0].duration--;
	
	//return; // doesn't work for 2!
	
/*
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
*/
}

#endif
#endif // WORD_DRAW
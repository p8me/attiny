
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

#define PWM_FRQ		((float)F_CPU/8.)

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
float K[2] = {0};
bool tone_flag[2], dir[2];

static void Draw(uint8_t idx, uint16_t start, float slope){
	tone_on[idx] = PWM_FRQ/start;
	K[idx] = slope/(PWM_FRQ*PWM_FRQ);
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
			//tone_on[idx] += tone_on[idx]*tone_on[idx]*tone_slope[idx];
			float p0 = tone_on[idx];
			tone_on[idx] = p0/(1. + K[idx]*p0*p0);
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

#define VERT_TIME	3	// for one pixel
#define HORZ_TIME	9	// for one pixel

#define	MX			3500. // bottom
#define MN			2500. // top

#define VERT_STEP	((MX - MN) / (RES - 1.))

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
			*period = VERT_STEP * (RES - 1 - s_row) + MN; // starting period
			*slope = (float)(s_row - e_row) * VERT_STEP / (float)*duration * wdt_frq_hz;
		}
	}
}

#define S	0,0
#define L	0,6, 6,8
#define U	0,6, 6,8, 8,2
#define V	0,7, 7,2
#define W	0,7, 7,2, 0,7, 7,2
#define M	6,1, 1,8, 6,1, 1,8
#define N	6,1, 1,8, 6,1
//#define J	3,6, 6,8, 8,2
#define T	0,2, 1,7, 0,2
#define H	0,6, 3,5, 0,6
#define I	1,7
#define Y	0,5, 4,7, 3,2
#define P2	6,0, 0,2
#define P6	0,2, 2,8
#define P7	V
#define P8	6,1, 1,8
#define PI	6,0, 0,2, 2,8
#define LAMBDA	6,5, 5,2, 3,8
#define DASH	3,4
#define HYPHEN	6,8

const uint8_t coords[] = {L,S,U,S,V,S,W,S,M,S,N,S,T,S,H,S,Y,S,P2,S,P6,S,P7,S,P8,S,PI,S,LAMBDA,S,DASH,S,HYPHEN,S,I};

// Possible single speaker:
// English: LUVNMWJHTYI
// persian 2 6 7 8
// greek: pi, lambda

/*
0 1 2 // MX	 \/
3 4 5 // 	 \/
6 7 8 // MN	 \/
*/

struct  
{
	uint16_t idx;
	uint16_t period;
	float slope;
	uint8_t duration;
} state[2];


void process_draw_sound_words(void){
	
	for (uint8_t i = 0; i < 1; i++)
	{
		if (state[i].duration == 0){
			if (state[i].idx == (sizeof(coords)/sizeof(coords[i])))
			{state[i].idx = 0; state[i].duration = 0;}
		
			draw_coor(&coords[state[i].idx], &state[i].period, &state[i].slope, &state[i].duration);
		
			if (state[i].period) {
				Draw(i, state[i].period, state[i].slope);
				start_tone(i);
			}
			else stop_tone(i);
		
			state[i].idx += 2;
		}
		else state[i].duration--;
	}
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
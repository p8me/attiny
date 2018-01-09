
#include "setts.h"
#ifdef MUSIC
#include <avr/common.h>
#include <stdlib.h>
#include <avr/io.h>
#include "glob.h"
#include <stdbool.h>
#include "wdt.h"
#include <util/delay.h>
#include <avr/interrupt.h>

// Periods

#define C 3000
#define D 2673
#define E 2381
#define F 2247
#define G 2002
#define A 1784
#define B 1589

#if (!TINY13)

const uint8_t TONE_PIN[2] = {0, 1};

void start_tone(uint8_t idx){
	if (idx == 0) TCCR0B = 2;
	else TCCR1 = 4;
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

uint16_t tone_on[2], tone_off[2], tone_vib_l[2], tone_end[2];
uint16_t goal_ovf[2] = {0}, vib_cnt[2] = {0}, vib_cnt_max[2] = {0};
bool tone_flag[2], dir[2];

#define		VIB_STEP		40
#define		VIB_MAG_DIV		200

void Play(uint8_t idx, uint16_t note){
	tone_on[idx] = note/2;
	tone_off[idx] = note - tone_on[idx];
	uint16_t tmp = note/VIB_MAG_DIV; // vibration magnitude.
	vib_cnt_max[idx] = (VIB_STEP * VIB_MAG_DIV)/note; // vibration step
	vib_cnt_max[idx] *= vib_cnt_max[idx] / 5;
	tone_vib_l[idx] = tone_on[idx] - tmp;
	tone_end[idx] = tone_on[idx] + tmp;
}

void set_timer(uint8_t idx, uint16_t time){
	goal_ovf[idx] = (time >> 8);
	uint8_t tmp = 255 - (time % 256);
	if (idx == 0) TCNT0 = tmp;
	else TCNT1 = tmp;
}

inline void mus_isr(uint8_t idx){
	if (goal_ovf[idx] > 0) goal_ovf[idx]--;
	else{ // timer expired
		if (tone_flag[idx]){
			set_timer(idx, tone_on[idx]);
			set_pin_to(TONE_PIN[idx], 0);
		}
		else{
			set_timer(idx, tone_off[idx]);
			set_pin_to(TONE_PIN[idx], 1);
			if (++vib_cnt[idx] >= vib_cnt_max[idx]){
				vib_cnt[idx] = 0;
				if(dir[idx]) tone_on[idx]--;
				else tone_on[idx]++;
				
				if (tone_on[idx] == tone_vib_l[idx]) dir[idx] = false;
				if (tone_on[idx] == tone_end[idx]) dir[idx] = true;
			}
		}
		tone_flag[idx] = !tone_flag[idx];
	}
}

ISR(TIM0_OVF_vect){
	mus_isr(0);
}

ISR(TIM1_OVF_vect){
	mus_isr(1);
}


#define CC11 C, E, G, C/2, G, E
#define CC12 2*C, 2*G, C, E, C, 2*G

#define GC11 G, B, D/2, G/2, D/2, B
#define GC12 2*G, D, G, B, G, D

#define FC11 F, A, C/2, F/2, C/2, A
#define FC12 2*F, C, F, A, F, C

#define CT11 10,10,10,10,10,10

// ======================================

#define CC21 C, 0, E, 0, G, 0, C/2, 0, G, 0, E, 0

#define CT21 5,5,5,5,5,5,5,5,5,5,5,5
#define CT22 2,8,2,8,2,8,2,8,2,8,2,8

const uint16_t melody[] = {			G, E/2,	D/2, E/2, F/2, D/2, C/2, B/2, A/2,  G/2, D/4, F/4, E/4, D/4, E/4, F/4, G/4, E/4, E/2, D/2, C/2, D/2, E/2, C/2,  G, E/2, D/2,  G, D/2, E/2, F/2, D/2, C/2,  B/2, A/2};
const uint8_t melody_dur[] = {		10, 50,	  2,   3,   5,  50,	 10,  60,  50,   10,  39,   7,   3,   2,   2,   3,   4,  60,   7,   3,   2,   2,   3,   4, 39,   7,   3,  2,   2,   3,   4,  50,  10,   60,  60};
const uint16_t accompany[] = {		0, CC12, 			  GC11,	    FC12,FC12,      GC12,							   CC11,   0,                        CC12,                             GC12,      FC12,FC12};
const uint8_t accompany_dur[] = {	10,CT11,			  CT11,     CT11,CT11,		CT11,                              CT11,  21,                        CT11,                             CT11,      CT11,CT11};

uint16_t mel_idx = 0, acc_idx = 0, mel_dur = 0, acc_dur = 0;

#define DL(TIME)  {_delay_ms(TIME*100);}

void la_bebe(void){

	if (mel_dur == 0){
		if (mel_idx == sizeof(melody)/sizeof(uint16_t))
			{mel_idx = 0; acc_idx = 0; mel_dur = 0; acc_dur = 0;}
		uint16_t note = melody[mel_idx];
		if (note) {
			Play(0, note);
			start_tone(0);
		}
		else stop_tone(0);
		mel_dur = melody_dur[mel_idx] - 1;
		mel_idx++;
	}
	else mel_dur--;

	if (acc_dur == 0){
		if (acc_idx == sizeof(accompany)/sizeof(uint16_t))
			{mel_idx = 0; acc_idx = 0; mel_dur = 0; acc_dur = 0;}
		uint16_t note = accompany[acc_idx];
		if (note) {
			Play(1, note);
			start_tone(1);
		}
		else stop_tone(1);
		acc_dur = accompany_dur[acc_idx] - 1;
		acc_idx++;
	}
	else acc_dur--;
}

#endif

#endif // MUSIC
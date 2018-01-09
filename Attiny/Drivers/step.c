
#include <avr/io.h>
#include "step.h"
#include "glob.h"
#include <util/delay.h>
#include <stdbool.h>

void set_pins(bool p1, bool p2, bool p3, bool p4){
	if(p1)	set_bit(STEP_PORT, STEP_1);
	else	clr_bit(STEP_PORT, STEP_1);
	
	if(p2)	set_bit(STEP_PORT, STEP_2);
	else	clr_bit(STEP_PORT, STEP_2);

	if(p3)	set_bit(STEP_PORT, STEP_3);
	else	clr_bit(STEP_PORT, STEP_3);

	if(p4)	set_bit(STEP_PORT, STEP_4);
	else	clr_bit(STEP_PORT, STEP_4);
}

void goto_step_8(int16_t p){
	p %= 8;
	if(p < 0) p += 8;
	switch(p){
		case 0: set_pins(1,0,1,0); break; // 1, 1
		case 1: set_pins(0,0,1,0); break; // 0, 1
		case 2: set_pins(0,1,1,0); break; //-1, 1
		case 3: set_pins(0,1,0,0); break; //-1, 0
		case 4: set_pins(0,1,0,1); break; //-1,-1
		case 5: set_pins(0,0,0,1); break; // 0,-1
		case 6: set_pins(1,0,0,1); break; // 1,-1
		case 7: set_pins(1,0,0,0); break; // 1, 0
	}
}

int16_t pos = 0;

void activate_pins() {STEP_DDR |= _BV(STEP_1) | _BV(STEP_2) | _BV(STEP_3) | _BV(STEP_4);}
void deactivate_pins() {
	clr_bit(STEP_DDR, STEP_1);
	clr_bit(STEP_DDR, STEP_2);
	clr_bit(STEP_DDR, STEP_3);
	clr_bit(STEP_DDR, STEP_4);
}

void stepper_go(int16_t p, uint8_t spd){
	int16_t sig = p > pos ? 1 : -1;

	activate_pins();

	while(pos != p){
		goto_step_8(pos);
		for(uint8_t i = 0; i < spd; i++) _delay_ms(5);
		pos += sig;
	}

	deactivate_pins();
}

void stepper_forward(){
	goto_step_8(++pos);
	activate_pins();
	_delay_ms(50);
	deactivate_pins();
}

void stepper_backward(){
	activate_pins();
	goto_step_8(--pos);
	_delay_ms(10);
	deactivate_pins();
}

int16_t stepper_get_position(void){
	return pos;
}



#ifndef PCINT_H
#define PCINT_H

/* Sample:

enable_pc_int(INPUT_PIN, ext_int);

void ext_int(bool pin_high){
	if (!pin_high)
	REPEAT_AFTER_AT_LEAST(5000, {
		
	});
}

*/

// only 1 interrupt is implemented
void enable_pc_int(uint8_t pin, void (*cb)(bool));

#endif

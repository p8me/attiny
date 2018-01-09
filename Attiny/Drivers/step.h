#ifndef STEP_H
#define STEP_H

/*

stepper_go(200, 20);
_delay_ms(1000);
stepper_go(0, 20);
_delay_ms(1000);

*/

#define STEP_PORT	PORTB
#define STEP_DDR	DDRB

#define STEP_1	0
#define STEP_2	2
#define STEP_3	3
#define STEP_4	1

void stepper_go(int16_t p, uint8_t spd);

void stepper_forward();
void stepper_backward();

int16_t stepper_get_position(void);

#endif
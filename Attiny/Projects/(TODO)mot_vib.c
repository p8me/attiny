
#include "main.h"

/* Description and Hardware

???

setts:
#define FIXED_WDT_PER	WDT_250ms

#define WDT			1
#define PCINT		0
#define OC0AB		0

*/

#define VIB_MOT_PIN				0
#define ADC_PIN					4

#define TOUCH_PIN				2

uint8_t led = 0;
bool minor = 0;

void wdt_event(void)
{
	//tgl_pin(2);
	//num_to_light(read_adc(ADC_CH));
	
	//uint16_t mot = detect_motion(ADC);

	//set_pin_to(VIB_MOT_PIN, is_pin_clr(TOUCH_PIN));

	if (read_adc(ADC_PIN) < 40)
		set_pin(VIB_MOT_PIN);
	else
		clr_pin(VIB_MOT_PIN);

	//if (is_pin_set(TOUCH_PIN))
	//set_pin(VIB_MOT_PIN);
	//else
	//clr_pin(VIB_MOT_PIN);

	//DO_ON_MOTION(ADC_CH, 3, 1500,
	//cnt = 2 * min(mot, 5) - 1,
	//{
	//set_pin(VIB_MOT_PIN);
	//},
	//{
	//clr_pin(VIB_MOT_PIN);
	//});
}


void loop(void){
	
}

void init(){
	OUT_PINS(VIB_MOT_PIN);
	
	adc_init();
	wdt_setup();
	//pwm_init();
}

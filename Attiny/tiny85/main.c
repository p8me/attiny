
#include "main.h"

/* Description and Hardware

setts:
#define FIXED_WDT_PER	WDT_250ms

*/

#define VIB_MOT					0
#define ADC_DRIVE_PIN			3
#define ADC_CH					2 // ch2 on pin4

uint8_t led = 0;
bool minor = 0;

void wdt_event(void)
{
	//tgl_pin(2);
	//num_to_light(read_adc(ADC_CH, ADC_DRIVE_PIN));
	
	//uint16_t mot = detect_motion(ADC, ADC_DRIVE_PIN);

	//DO_ON_MOTION(ADC_CH, ADC_DRIVE_PIN, 3, 1500,
	//cnt = 2 * min(mot, 5) - 1,
	//{
		//set_pin(VIB_MOT);
	//},
	//{
		//clr_pin(VIB_MOT);
	//});
	process_draw_sound_words();
}


void loop(void){
	
}

void init(){
	OUT_PINS(ADC_DRIVE_PIN, 0);
	HIGH_PINS(ADC_DRIVE_PIN);
	
	adc_init();
	wdt_setup();
	music_init();
	//for(int i = 0; i < 180; i++) la_bebe();
	//pwm_init();
}

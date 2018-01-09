//#include "motion.h"
//#include "adc.h"
//#include <avr/wdt.h>
//#include "glob.h"
//#include "delay.h"
//
////#include "act_tone.h"
//
//// to be called in ISR(WDT_vect)
//void react_to_motion(uint32_t tim){
	//uint8_t mot = detect_motion(2, 3);
	//static uint16_t last_idle;
	//
	//if (cnt == 0 && mot > 0 && tim > last_idle + (MIN_IDLE_TIME_MS/32)){
		//init_act(mot);
		//set_bit(WDTCR, 7);
	//}
	//if (cnt > 0){
		//cnt--;
		//act();
		//if (cnt == 0){
			//last_idle = tim;
			//end_act();
		//}
	//}
//}
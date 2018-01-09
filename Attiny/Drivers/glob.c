
#include "glob.h"
#include <util\delay.h>

void delay_ms(uint16_t n){
	while(n--)_delay_ms(1);
}


/*


uint32_t tmp = ((uint32_t)bayes_adc * bayes_rat + val);
bayes_adc = tmp / (bayes_rat + 1);
uint16_t dif = bayes_adc > val ? bayes_adc - val : val - bayes_adc;

//dbg_printf("%7ul, %7ul, ", val, bayes_adc);
//dbg_printf("%4d, %5d\n", val);

*/

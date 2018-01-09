
#include "main.h"

void init(void);
void loop(void);
void wdt_event(void);


int main(void)
{
	#if(!TINY13)
	dbg_tx_init();
	#endif

	// ============= Clock ================
	CLKPR = _BV(CLKPCE); CLKPR = CPU_PRESC;

	// ========== Enable Sleep ============
#if WDT
	set_bit(MCUCR, SM1); // incompatible with PWM
#endif
	init();

	while(1){
#if WDT
		sleep_cpu();
		if (wdt_timeout_happened()) wdt_event();
#endif
		loop();
	}
	return 0;
}

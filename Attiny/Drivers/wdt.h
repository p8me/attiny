
#ifndef WDT_H
#define WDT_H

#include "glob.h"
#include <stdbool.h>

void wdt_setup();
bool wdt_timeout_happened(void);
void enable_deep_sleep(void);
void disable_deep_sleep(void);

extern volatile uint32_t ts;

#define	WDT_16ms	0
#define	WDT_32ms	1
#define	WDT_64ms	2
#define	WDT_125ms	3
#define	WDT_250ms	4
#define	WDT_500ms	5
#define	WDT_1000ms	6
#define	WDT_2000ms	7
#define	WDT_4000ms	8
#define	WDT_8000ms	9

#if (!FIXED_WDT_PER)

void set_wdt_period(uint8_t wk_ms);
extern volatile uint8_t wdt_frq_hz;
extern volatile uint16_t wdt_per_ts; // not in milliseconds

#else

#define wdt_per_ts	((uint16_t)(1) << FIXED_WDT_PER) // not in milliseconds
#define wdt_frq_hz	(64 >> FIXED_WDT_PER)

#endif

#define MS_TO_TS(ms)	((ms)/16)

#define REPEAT_AFTER_AT_LEAST_MS(AT_LEAST_MS, ACT){\
	static uint32_t last_ts = 0;\
	if (ts > (last_ts + MS_TO_TS(AT_LEAST_MS))){\
		last_ts = ts;\
		ACT;\
	}\
}

#define REPEATE_EVERY_XSEC(SECS, ACT){	\
	static cnt = 0;						\
	if (++cnt == SECS*wdt_frq_hz){		\
		cnt = 0;						\
		ACT;							\
	}									\
}

#endif

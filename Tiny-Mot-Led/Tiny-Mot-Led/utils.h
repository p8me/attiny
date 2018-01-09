

#define set_bit(REG, BIT) REG |= _BV(BIT)
#define clr_bit(REG, BIT) REG &= ~_BV(BIT)
#define tgl_bit(REG, BIT) REG ^= _BV(BIT)
#define is_bit_set(REG, BIT) (REG & _BV(BIT))
#define is_bit_clr(REG, BIT) !is_bit_set(REG, BIT)

enum {
	WDT_16ms = 0,
	WDT_32ms,
	WDT_64ms,
	WDT_125ms,
	WDT_250ms,
	WDT_500ms,
	WDT_1000ms,
	WDT_2000ms,
	WDT_4000ms,
	WDT_8000ms
};


#ifndef WDIE
	#define WDIE 6
#endif

#define SET_NEXT_WAKE(wk_ms)	WDTCR = _BV(WDIE) + WDT_##wk_ms##ms;\
								wdt_per = wk_ms;
								//wdt_per = (uint16_t)((wk_ms * 104UL) / 100UL);

#define POWER_DOWN_EN()  set_bit(MCUCR, SM1); //TCCR0B = 0;
#define POWER_DOWN_DIS() clr_bit(MCUCR, SM1); //TCCR0B = _BV(CS00);

#define min(a,b) ((a) < (b) ? (a) : (b))


//dbg_printf("%5d\n", read_adc(1));
//OCR0A = read_adc(1);

/* Sample: in wdt_event

DO_ON_MOTION(ADC_CH, 3, 200,
		cnt = 12 * min(mot, 5) - 1,
		{set_bit(PORTB, led);
		_delay_ms(3);
		clr_bit(PORTB, led);
		if(++led == 3) led = 0;
		}, {});


DO_ON_MOTION(ADC_CH, 3, 200,
	{
		minor = rand()%2;
		if(minor){
			cnt = 12 * min(mot, 5) - 1;
			set_wdt_period(WDT_250ms);
		}
		else{
			cnt = 24 * min(mot, 5) - 1;
			set_wdt_period(WDT_125ms);
		}
		start_tone();
	},
	{
		if (minor)	RandTone_minor((cnt/24));
		else		RandTone_major((cnt/24));
	},
	{
		set_wdt_period(WDT_250ms);
		stop_tone();
	});

*/



#define DO_ON_MOTION(ADC, MOT_LIM, MIN_CLEARANCE, INIT, ACT, END){	\
	uint8_t mot = detect_motion(ADC, MOT_LIM);				\
	static uint8_t cnt;									\
	static uint32_t last_idle;							\
	REPEAT_AFTER_AT_LEAST(MIN_CLEARANCE,				\
		if (cnt == 0 && mot > 0){						\
			INIT;										\
		}												\
	);													\
	if (cnt > 0){			\
		ACT;				\
		if (--cnt == 0){	\
			last_idle = ts;	\
			END;			\
		}					\
	}						\
}							\


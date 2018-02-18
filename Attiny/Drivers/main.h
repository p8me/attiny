/* avrdude arguments:
>> attiny85:
 -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -V -pattiny85 -cstk500v1 -PCOM3 -b1000000 -Uflash:w:"$(ProjectDir)Debug\$(TargetName).hex":i
>>  attiny13A:
 -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -V -pattiny13 -cstk500v1 -PCOM3 -b1000000 -Uflash:w:"$(ProjectDir)Debug\$(TargetName).hex":i

       (PCINT5/RESET/ADC0/dW) PB5 -|1 --\/-- 8|- VCC
(PCINT3/XTAL1/CLKI/OC1B/ADC3) PB3 -|2        7|- PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
(PCINT4/XTAL2/CLKO/OC1B/ADC2) PB4 -|3        6|- PB1 (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
                              GND -|4--------5|- PB0 (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0) 

*/

#ifndef MAIN_H
#define MAIN_H

#include "glob.h"
#include "step.h"
#include "adc.h"
#include "pwm.h"
#include "wdt.h"
#include "pcint.h"
#include "motion.h"
#include "dbg_print.h"
#include "music.h"
#include "word_draw.h"
#include "setts.h"



#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/common.h>
#include <avr/cpufunc.h>
#include <avr/fuse.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <math.h>
#include <stdlib.h>
//#include <avr/signature.h> // includes a const var, can't be included in more than 1 source file


#include "macro_utils.h"
#define SET_HIGH(x)		_BV(x) +
#define OUT_PINS(...)	DDRB |= FOR_EACH(SET_HIGH, __VA_ARGS__) 0;
#define HIGH_PINS(...)	PORTB |= FOR_EACH(SET_HIGH, __VA_ARGS__) 0;

#endif

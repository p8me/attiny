// avrdude command:
// attiny85:
// -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -pattiny85 -cstk500v1 -PCOM3 -b19200 -Uflash:w:"$(ProjectDir)Debug\$(TargetName).hex":i
// attiny13A:
// -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -pattiny13 -cstk500v1 -PCOM3 -b19200 -Uflash:w:"$(ProjectDir)Debug\$(TargetName).hex":i

/*     (PCINT5/RESET/ADC0/dW) PB5 -|1 --\/-- 8|- VCC
(PCINT3/XTAL1/CLKI/OC1B/ADC3) PB3 -|2        7|- PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
(PCINT4/XTAL2/CLKO/OC1B/ADC2) PB4 -|3        6|- PB1 (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
                              GND -|4--------5|- PB0 (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0)  */

#define CPU_PRESC 1
#define F_CPU (8000000 / (1 << CPU_PRESC))


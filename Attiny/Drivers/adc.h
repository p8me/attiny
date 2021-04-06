
#ifndef ADC_H
#define ADC_H

void adc_init(void);
// A light sensor must be connected between adc_pin and GND
uint16_t read_adc(uint8_t adc_pin); // pin is PORTx
uint8_t detect_adc_change(uint8_t adc_pin, const uint8_t mot_lim);
uint16_t read_capacitance(uint8_t adc_pin);
uint8_t detect_cap_change(uint8_t adc_pin, const int8_t mot_lim);

#endif

#ifndef ADC_H
#define ADC_H

void adc_init(void);
uint8_t read_adc(uint8_t adc_pin);
uint8_t detect_motion(uint8_t adc_pin, const uint8_t mot_lim);

#endif
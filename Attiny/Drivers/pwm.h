
#ifndef PWM_H
#define PWM_H

 void pwm_init(void);
 void pwm_disable(void);
 bool is_pwm_disabled(void);
 void num_to_light(uint8_t num);
 void soft_blink(void);

#endif

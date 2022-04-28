#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f0xx_tim.h"

void TIM1_Config(void);
void TIM1_PWM_Config(uint16_t fpwm);
void TIM3_Config(void);
void TIM3_PWM_Config(uint16_t fpwm);

void M1A_pwm(uint16_t value);
void M2A_pwm(uint16_t value);
void M3A_pwm(uint16_t value);
void M4A_pwm(uint16_t value);
void M1B_pwm(uint16_t value);
void M2B_pwm(uint16_t value);
void M3B_pwm(uint16_t value);
void M4B_pwm(uint16_t value);

#endif

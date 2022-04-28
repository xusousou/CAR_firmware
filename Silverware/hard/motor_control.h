#ifndef __MOTOR_CONTROL_H_
#include <math.h>
#include "stm32f0xx.h"
#include "config.h"

void forward(void);
void backward(void);

void pan_right(void);
void pan_left(void);

void up_right(void);
void up_left(void);
void down_right(void);
void down_left(void);

void turn_up_left(void);
void turn_up_right(void);
void turn_down_left(void);
void turn_down_right(void);

void slid(void);
void brush(void);

void rx_value(void);
void Motor_Control(void);

#endif

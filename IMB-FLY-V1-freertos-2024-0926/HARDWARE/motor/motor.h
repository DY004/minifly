#ifndef   _MOTOR_H
#define   _MOTOR_H
#include "stdint.h"

void MOTOR_Init(void);
void Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM);

#endif


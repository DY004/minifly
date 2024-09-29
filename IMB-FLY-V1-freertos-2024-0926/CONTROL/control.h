#ifndef   _CONTROL_H
#define   _CONTROL_H

//#include "stm32f10x.h"
#include "imu.h"

void Control(FLOAT_ANGLE *att_in,FLOAT_XYZ *gyr_in, RC_TYPE *rc_in, u8 armed);
int16_t Yaw_Control(float TARGET_YAW);
void Yaw_Carefree(FLOAT_ANGLE *Target_Angle, const FLOAT_ANGLE *Measure_Angle);
void Safety_Check(void);
#endif


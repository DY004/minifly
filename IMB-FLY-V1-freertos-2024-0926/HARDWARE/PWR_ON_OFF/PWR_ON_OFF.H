#ifndef _PWR_ON_OFF_H
#define _PWR_ON_OFF_H
//#include "sys.h"

#define KEY_ON_OFF    HAL_GPIO_ReadPin(PW_ON_OFF_GPIO_Port,PW_ON_OFF_Pin)

void PWR_ON_OFF(void);








#endif



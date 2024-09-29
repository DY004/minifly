#include "PWR_ON.h"
#include "gpio.h"
#include "delay.h"

void PWR_ON(void)
{
	HAL_GPIO_WritePin(PW_EN_GPIO_Port,PW_EN_Pin,GPIO_PIN_SET);
	HAL_Delay(200);
	
}



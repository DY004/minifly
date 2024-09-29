#include "PWR_ON_OFF.h"
#include "gpio.h"
//#include "key.h"
#include "delay.h"



void PWR_ON_OFF(void)
{
	if(KEY_ON_OFF == GPIO_PIN_RESET)//检测关机按键的按下
	{
		HAL_Delay(50);
		while(!KEY_ON_OFF);//等待按键松开
		HAL_GPIO_WritePin(PW_EN_GPIO_Port,PW_EN_Pin, GPIO_PIN_RESET);//把开机的IO
	
	}


}



#include "Control_fan.h"
#include "gpio.h"
#include "key.h"
#include "delay.h"


uint8_t fan_a_on_flag = 0;
uint8_t fan_a_off_flag = 0;
uint8_t fan_b_on_flag = 0;
uint8_t fan_b_off_flag = 0;

void Control_fan(void)
{
	if(fan_a_on_flag == 1)//检测从上位机接受的控制风扇的命令
	{
		HAL_GPIO_WritePin(AC_contactor_en_GPIO_Port,AC_contactor_en_Pin,GPIO_PIN_SET);//开启风扇A
		fan_a_on_flag = 0;//清零，等待下一次的命令到来。
	
	}
	
	else if(fan_a_off_flag == 1)//检测从上位机接受的控制风扇的命令
	{
		HAL_GPIO_WritePin(AC_contactor_en_GPIO_Port,AC_contactor_en_Pin,GPIO_PIN_RESET);//开启风扇A
		fan_a_off_flag = 0;//清零，等待下一次的命令到来。
	
	}
	
	else if(fan_b_on_flag == 1)//检测从上位机接受的控制风扇的命令
	{
		HAL_GPIO_WritePin(FAN1_GPIO_Port,FAN1_Pin,GPIO_PIN_SET);//开启风扇A
		fan_b_on_flag = 0;//清零，等待下一次的命令到来。
	
	}
	else if(fan_b_off_flag == 1)//检测从上位机接受的控制风扇的命令
	{
		HAL_GPIO_WritePin(FAN1_GPIO_Port,FAN1_Pin,GPIO_PIN_RESET);//开启风扇A
		fan_b_off_flag = 0;//清零，等待下一次的命令到来。
	
	}


}



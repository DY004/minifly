#include "Display_temperature.h"
#include "gpio.h"
#include "key.h"
#include "delay.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "usart.h"

char page_str[100];  //发送字符串缓存
float T = 1.0;
uint8_t T_Flag = 0;//达到一定值后，达到了标志位
void Display_temperature(void)
{
		//设置WENDU值
	sprintf(page_str,"wset numTf.valf %.2f\r\n",T);
	u1_SendArray((uint8_t*)page_str,strlen(page_str));
	
//		USART_OUT(USART1, (uint8_t*)page_str,strlen(page_str));	
	HAL_Delay(1); //命令之间需要加上小延时分开
	T+=2.3;
	
	if(T > 30.6)
	{
		T_Flag = 1;//触发了标志位的到达
		T = 1.0;//T温度回归到初始值。
		T_Flag = 0;//清除标志位，等待下一次触发的到来。
	}
	
	



}



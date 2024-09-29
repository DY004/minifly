/*******************************************************************************************
										    声 明
    本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他
	
不可估量的BUG，天际智联不负任何责任。请勿商用！

* 程序版本：V1.01
* 程序日期：2018-8-18
* 程序作者：愤怒的小孩
* 版权所有：西安天际智联信息技术有限公司
*******************************************************************************************/
//#include "stm32f10x.h"
#include "stdlib.h"
#include "delay.h"
//#include "structconfig.h"
#include "gpio.h"
#include "WS2812B_led.h"
#include "structconfig.h"
//#include "intrins.h"

//#include "stm32f10x.h"

/***
#define   RGB_H     GPIOB->BSRR |= GPIO_Pin_9   //配置对应RGB引脚为 高电平
#define   RGB_L     GPIOB->BRR  |= GPIO_Pin_9	//配置对应RGB引脚为 低电平
***/
//上面的适配成hal库的形式


#define   RGB_H     HAL_GPIO_WritePin(RGBLED_GPIO_Port,RGBLED_Pin,GPIO_PIN_SET) //配置对应RGB引脚为 高电平
#define   RGB_L     HAL_GPIO_WritePin(RGBLED_GPIO_Port,RGBLED_Pin,GPIO_PIN_RESET)	//配置对应RGB引脚为 低电平

//跑马灯RGB三元色配出七彩跑马灯
static u32 Run_buf[][16] = 
{
	{0xFFA500,0,0,0,0xFFA500,0,0,0,0xFFA500,0,0,0,0xFFA500,0,0,0,},//橙色
	{0x00FF00,0,0,0,0x00FF00,0,0,0,0x00FF00,0,0,0,0x00FF00,0,0,0,},//绿色
	{0xFF00FF,0,0,0,0xFF00FF,0,0,0,0xFF00FF,0,0,0,0xFF00FF,0,0,0,},//紫色
	{0x00FFFF,0,0,0,0x00FFFF,0,0,0,0x00FFFF,0,0,0,0x00FFFF,0,0,0,},//青色
	{0x0000FF,0,0,0,0x0000FF,0,0,0,0x0000FF,0,0,0,0x0000FF,0,0,0,},//蓝色
	{0xFFFF00,0,0,0,0xFFFF00,0,0,0,0xFFFF00,0,0,0,0xFFFF00,0,0,0,},//黄色
	{0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,0xFFFFFF,0,0,0,},//白色
	
};

uint8_t Run_flag=1;//跑马灯标志

/***************************************************************************
* 函  数：void LED_Init(void)
* 功  能：用户指示灯引脚初始化
* 参  数：无
* 返回值：无
* 备  注: RGB引脚初始化和MCU指示灯引脚初始化
***************************************************************************/
//void LED_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;   
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
//	
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9; 
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_8);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
//}

/***************************************************************************
* 函  数：void LED_Run(void)
* 功  能：指示MCU是否工作
* 参  数：无
* 返回值：无
* 备  注: LED闪烁MCU工作正常
***************************************************************************/
void LED_Run(void)
{
//	static uint8_t flag = 1;
//	if(flag)
//	{
//		flag = 0;
//		GPIO_SetBits(GPIOB,GPIO_Pin_8);
//	}
//	else
//	{
//		flag = 1;
//		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
//	}
}

/********************************************************************************
* 函  数：void Write0(void)
* 功  能：写 0码 函数
* 参  数：无
* 返回值：无
* 备  注: 根据RGB灯手册查得 ：RGB_H 延时 300ns，RGB_L 延时 900ns
*		  RGB_H，RGB_L之间的延时，根据不同芯片的主频实际调节__nop()函数的个数	  
*********************************************************************************/
void Write0(void)
{
	RGB_H;
	__nop();__nop();__nop();__nop();__nop();__nop();
	
	RGB_L;
	
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}


/********************************************************************************
* 函  数：void Write1(void)
* 功  能：写 1码 函数
* 参  数：无
* 返回值：无
* 备  注: 根据RGB灯手册查得 ：RGB_H 延时 600ns，RGB_L 延时 600ns
*		  RGB_H，RGB_L之间的延时，根据不同芯片的主频实际调节__nop()函数的个数	  
*********************************************************************************/
void Write1(void)
{
	RGB_H;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	
	RGB_L;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}

/***************************************************************************
* 函  数：void RGB_WByte(uint8_t byte)
* 功  能：写一个字节（8bit）数据
* 参  数：byte 
* 返回值: 无
* 备  注: RGB的亮度通过更改 0x01 处的值进行调节
***************************************************************************/
void RGB_WByte(uint8_t byte)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
	  if((byte<<i)&0x01)
		  Write1();
	  else
		  Write0();
	}
}

/***************************************************************************
* 函  数：void Write24Bit(uint8_t green, uint8_t red, uint8_t blue)
* 功  能：设置一个RGB灯的色彩
* 参  数：green red blue，红绿蓝光所占比例大小,范围0~255
* 返回值: 无
* 备  注: 灯亮的顺序为GRB，每种颜色占8位数据，因此一个灯需要写24位数据
***************************************************************************/
void Write24Bit(uint8_t green, uint8_t red, uint8_t blue)
{
	RGB_WByte(green);
	RGB_WByte(red);
	RGB_WByte(blue);
}

/**************************************************************************
* 函  数：void RGB_LED_Rand(void)
* 功  能：随机变换颜色
* 参  数：无
* 返回值：无
* 备  注: 取模范围越大，色彩种类越多
**************************************************************************/
void RGB_LED_Rand(void)
{
	uint8_t i,red=0,green=0,blue=0;;
	for(i=0;i<4;i++)
	{
		green = rand()%18+2;//产生一个0~20的随机数
		red   = rand()%18+2;
		blue  = rand()%18+2;
		Write24Bit(green,red,blue);//合成颜色
	}  
}

/**************************************************************************
* 函  数：void RGB_LED_Runing(void)
* 功  能：跑马灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Runing(void)
{
	uint8_t i,red=0,green=0,blue=0;
	static uint8_t cnt = 0,wcnt = 0,times = 0;
	if(times++ >= 16)
	{
	 times = 0; 
	 wcnt++;
	}
	for(i=0;i<4;i++)
	{
		if(cnt>4) cnt = 0;
		red   = ((Run_buf[wcnt][cnt]>>16)&0xff);
		green = ((Run_buf[wcnt][cnt]>>8)&0xff); 
		blue  = ((Run_buf[wcnt][cnt]>>0)&0xff);
		Write24Bit(green,red,blue);//合成颜色
		cnt++;
	}
	if(wcnt==7) wcnt = 0;  
}

/**************************************************************************
* 函  数：void RGB_LED_Red(void)
* 功  能：红灯
* 参  数：无
* 返回值：无
* 备  注：还有很多种颜色可以自己慢慢调配
**************************************************************************/
void RGB_LED_Red(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0,0xff,0);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_Orange(void)
* 功  能：橙灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Orange(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0xa5,0xff,0x00);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_Yellow(void)
* 功  能：黄灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Yellow(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0xff,0xff,0);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_green(void)
* 功  能：绿灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_green(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0xff,0,0);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_Cyan(void)
* 功  能：青灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Cyan(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0xff,0,0xff);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_Blue(void)
* 功  能：蓝灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Blue(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0,0,0xff);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_Violet(void)
* 功  能：紫灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Violet(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0x00,0xcd,0xcd);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_FLY(void)
* 功  能：两红两绿
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_FLY(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		if(i<2)
			Write24Bit(0xff,0,0);
		else
			Write24Bit(0,0xff,0);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_White(void)
* 功  能：白灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_White(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0xff,0xff,0xff);
	}
}

/**************************************************************************
* 函  数：void RGB_LED_Off(void)
* 功  能：蓝灯
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void RGB_LED_Off(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		Write24Bit(0,0,0);
	}
}

/**************************************************************************
* 函  数：void GYRO_Offset_LED(void)
* 功  能：陀螺仪校准完成蓝灯闪烁
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void GYRO_Offset_LED(void)
{
	 RGB_LED_Off();
	 RGB_LED_Blue();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Blue();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Blue();
	 delay_ms(100);
	 RGB_LED_Off();
}

/**************************************************************************
* 函  数：void ACC_Offset_LED(void)
* 功  能：加速度校准完成绿灯闪烁
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void ACC_Offset_LED(void)
{
	 RGB_LED_Off();
	 RGB_LED_green();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_green();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_green();
	 delay_ms(100);
	 RGB_LED_Off();
}

/**************************************************************************
* 函  数：void BAR_Offset_LED(void)
* 功  能：气压计校准完成紫灯闪烁
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void BAR_Offset_LED(void)
{
	 RGB_LED_Off();
	 RGB_LED_Violet();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Violet();
	 delay_ms(100);
	 RGB_LED_Off();
	 delay_ms(100);
	 RGB_LED_Violet();
	 delay_ms(100);
	 RGB_LED_Off();
}

/**************************************************************************
* 函  数：void BATT_Alarm_LED(void)
* 功  能：低电量红灯快闪
* 参  数：无
* 返回值：无
* 备  注: 无
**************************************************************************/
void BATT_Alarm_LED(void)
{
	static uint8_t flag = 0;
	if(BATT_LEDflag)
	{
		if(flag)
		{
			flag = 0;
			RGB_LED_Red();
		}
		else
		{
			flag = 1;
			RGB_LED_Off();
		}
	}
}



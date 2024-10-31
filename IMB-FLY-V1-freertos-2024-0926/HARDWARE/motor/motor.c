/*******************************************************************************************
										    声 明
    本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他
	
不可估量的BUG，天际智联不负任何责任。请勿商用！

* 程序版本：V1.01
* 程序日期：2018-8-18
* 程序作者：愤怒的小孩
* 版权所有：西安天际智联信息技术有限公司
*******************************************************************************************/
#include "stm32f1xx_hal.h"
#include "motor.h"


#define Moto_PwmMax 1000//需要适配自己的动力。
int16_t MOTO1_PWM = 0;
int16_t MOTO2_PWM = 0;
int16_t MOTO3_PWM = 0;
int16_t MOTO4_PWM = 0;


/******************************************************************************************
* 函  数：void MOTOR_Init(void)
* 功  能：电机引脚初始化 以及TIM3 配置输出PWM
* 参  数：无
* 返回值：无
* 备  注：TIM3 CH1(PWM1) -> PA6
*         TIM3 CH2(PWM2) -> PA7
*         TIM3 CH3(PWM3) -> PB0
*         TIM3 CH4(PWM4) -> PB1
*******************************************************************************************/
void MOTOR_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //定义定时器结构体变量
//	TIM_OCInitTypeDef TIM_OCInitStruct;   //定义输出比较结构体变量
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //GPIOA、B、复用时钟使能
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //使能TIM3的时钟
//	
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;   //配置GPIO第6、7引脚
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //配置GPIO为复用推挽输出
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  	  //配置GPIO速率
//	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
//	
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;   //配置GPIO第0、1引脚
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //配置GPIO为复用推挽输出
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   	  //配置GPIO速率
//	GPIO_Init(GPIOB,&GPIO_InitStruct);   //GPIO初始化函数
//	
//	TIM_TimeBaseInitStruct.TIM_Period=1000-1;   	  //设置自动重装载的周期值
//	TIM_TimeBaseInitStruct.TIM_Prescaler=100;   	  //设置预分频值
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //设置时钟分割
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);   		   //定时器初始化函数
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
//	TIM_OCInitStruct.TIM_Pulse=0;   						   //初始化占空比为0
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   	   //输出比较极性低
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //比较输出使能
//	TIM_OC1Init(TIM3,&TIM_OCInitStruct);   					   //比较输出初始化函数
//	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较1预装载寄存器使能
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
//	TIM_OCInitStruct.TIM_Pulse=0;   					   		//初始化占空比为0
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//输出比较极性低
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//比较输出使能
//	TIM_OC2Init(TIM3,&TIM_OCInitStruct);   						//比较输出初始化函数
//	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较2预装载寄存器使能
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
//	TIM_OCInitStruct.TIM_Pulse=0;   							//初始化占空比为0
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//输出比较极性低
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//比较输出使能
//	TIM_OC3Init(TIM3,&TIM_OCInitStruct);   						//比较输出初始化函数
//	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较3预装载寄存器使能
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
//	TIM_OCInitStruct.TIM_Pulse=0;   							//初始化占空比为0
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   		//输出比较极性低
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   	//比较输出使能
//	TIM_OC4Init(TIM3,&TIM_OCInitStruct);   						//比较输出初始化函数
//	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);   //输出比较4预装载寄存器使能
//	
//	TIM_Cmd(TIM3,ENABLE);   //TIM3使能
}

/************************************************************************************************
* 函  数：void Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
* 功  能：电机要输出数值转换成PWM波形输出
* 参  数：MOTO1_PWM 电机1
*         MOTO2_PWM 电机2
*         MOTO3_PWM 电机3
*         MOTO3_PWM 电机4
* 返回值：无 
* 备  注：无
************************************************************************************************/
void Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
{		
	if(MOTO1_PWM>Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	if(MOTO2_PWM>Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	if(MOTO3_PWM>Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	if(MOTO4_PWM>Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
	if(MOTO1_PWM<0)	MOTO1_PWM = 0;
	if(MOTO2_PWM<0)	MOTO2_PWM = 0;
	if(MOTO3_PWM<0)	MOTO3_PWM = 0;
	if(MOTO4_PWM<0)	MOTO4_PWM = 0;
	
	TIM3->CCR1 = MOTO1_PWM;
	TIM3->CCR2 = MOTO2_PWM;
	TIM3->CCR3 = MOTO3_PWM;
	TIM3->CCR4 = MOTO4_PWM;
}

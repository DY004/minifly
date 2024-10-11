/*******************************************************************************************
										    声 明
    本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他
	
不可估量的BUG，天际智联不负任何责任。请勿商用！

* 程序版本：V1.01
* 程序日期：2018-8-18
* 程序作者：愤怒的小孩
* 版权所有：西安天际智联信息技术有限公司
*******************************************************************************************/
#include "structconfig.h"
#include "battery_adc.h"
#include "si24r1.h"
#include "stdio.h"
#include "filter.h"
#include "adc.h"


BATT_TYPE BAT=
{
	.BattAdc = 0,        //电池电压采集ADC值
	.BattRealV = 3.31f,  //实际测量的飞机供电电压 (注意此电压必须亲测否则测量的电压不准)
	.BattMeasureV = 0,   //程序测量的实际电池电压
	.BattAlarmV = 3.0f,  //电池低电压报警瞬时值 (这个值需要根据机身不同重量实测，实测380mh是2.8v)
	.BattFullV = 4.2f,   //电池充满电值 4.2V
};
uint8_t BATT_LEDflag = 0;

/******************************************************************************************
* 函  数：void BATT_Init(void)
* 功  能：电压检测引脚初始化 以及ADC1初始化
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************************/
void BATT_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	ADC_InitTypeDef   ADC_InitStruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); 
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//	
//	//模拟输入模式选择       
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN; //模拟输入
//	GPIO_Init(GPIOA, &GPIO_InitStruct);

//	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;						//独立模式
//	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;					//数据右对齐
//	ADC_InitStruct.ADC_NbrOfChannel=1;									//1个数据通道
//	ADC_InitStruct.ADC_ScanConvMode=DISABLE;							//扫描转换模式失能
//	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;		//外部触发失能
//	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;						//连续转换失能
//	ADC_Init(ADC1,&ADC_InitStruct);
//	
//	ADC_Cmd(ADC1, ENABLE); //使能ADC1
//	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);	//规则组转换通道
}

/******************************************************************************************
* 函  数：uint16_t Get_BatteryAdc(uint8_t ch)
* 功  能：获取电池采样点电压的ADC值
* 参  数：ch  ADC采样通道
* 返回值：返回通道AD值
* 备  注：电池电压采样点的ADC值，电池电压采样电路见原理图
*******************************************************************************************/
uint16_t Get_BatteryAdc(uint8_t ch)
{
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	//软件触发转换使能
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	//等待转换结束
//	return ADC_GetConversionValue(ADC1);	//返回转换结果的值
	
//	HAL_ADC_ConfigChannel(&hadc1, &sConfig);         /* 通道配置 */
    HAL_ADC_Start(&hadc1);                               /* 开启ADC */
    HAL_ADC_PollForConversion(&hadc1, 10);               /* 轮询转换 */
    return (uint16_t)HAL_ADC_GetValue(&hadc1);           /* 返回最近一次ADC1规则组的转换结果 */

}		

/******************************************************************************************
* 函  数：void BATT_GetVoltage(void)
* 功  能：获取电池电压
* 参  数：无
* 返回值：无
* 备  注：电池电压 = ADC检测电压*2 具体原理可看原理图
*******************************************************************************************/
void BATT_GetVoltage(void)
{
	float V;
	Aver_Filter((float)Get_BatteryAdc(ADC_CHANNEL_1),&BAT.BattAdc,6); //滑动滤波电压值，提高精度
	if(BAT.BattAdc)
	V = BAT.BattAdc * BAT.BattRealV / 4096.0f;
	BAT.BattMeasureV = 2*V; //根据原理电阻分压，可知 电池实际电压 = ADC侧量电压 * 2
//	printf("Test Voltage :%0.2f   temp:%0.0f \r\n ",BAT.BattMeasureV,BAT.BattAdc);
}

/******************************************************************************************
* 函  数：void LowVoltage_Alarm(void)
* 功  能：低电量报警
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************************/
void LowVoltage_Alarm(void)
{
	static uint8_t cnt=0,cnt1=0;
	BATT_GetVoltage();
	if(Airplane_Enable)
	{
		if(BAT.BattMeasureV < BAT.BattAlarmV)//飞行时测量
		{
			if(cnt1++>10)
			{
				cnt1 = 0;
				BATT_LEDflag = 1;
			}
		}
		else
		{
			cnt1 = 0;
			BATT_LEDflag = 0;
		}
	}else
	{
		if(BAT.BattMeasureV < 3.7f)//落地时测量（380mh时是3.5V）
		{
			if(cnt++>10)
			{
				Run_flag = 0;
				cnt = 0;
			  BATT_LEDflag = 1;
			}
		}
		else
		{
			Run_flag = 1;
			cnt = 0;
			BATT_LEDflag = 0;
		}
	}
}



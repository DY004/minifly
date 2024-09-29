#include "battery_adc.h"
#include "gpio.h"
//#include "key.h"
#include "delay.h"
#include "structconfig.h"

BATT_TYPE BAT=
{
	.BattAdc = 0,        //电池电压采集ADC值
	.BattRealV = 3.31f,  //实际测量的飞机供电电压 (注意此电压必须亲测否则测量的电压不准)
	.BattMeasureV = 0,   //程序测量的实际电池电压
	.BattAlarmV = 3.2f,  //电池低电压报警瞬时值 (这个值需要根据机身不同重量实测，实测380mh是2.8v)
	.BattFullV = 4.2f,   //电池充满电值 4.2V
};
uint8_t BATT_LEDflag = 0;


void battery_adc(void)
{



}



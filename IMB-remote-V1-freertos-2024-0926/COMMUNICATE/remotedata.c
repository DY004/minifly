/*******************************************************************************************
										    声 明
    本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他
	
不可估量的BUG，天际智联不负任何责任。请勿商用！

* 程序版本：V1.01
* 程序日期：2018-8-18
* 程序作者：愤怒的小孩
* 版权所有：西安天际智联信息技术有限公司
*******************************************************************************************/
#include "main.h"
#include "remotedata.h"
#include "usart.h"
#include "structconfig.h"
#include "WS2812B_led.h"
#include "battery_adc.h"

uint8_t DataID;	//数据包ID
RC_TYPE RC_Control;

void Button_command(uint8_t Button);

/***********************************************************************************************************************
* 函  数：void Remote_Data_ReceiveAnalysis(void)
* 功  能：遥控器数据包解析
* 参  数：无
* 返回值：无
* 备  注：通信协议：
         前导码-按键MASK--ADC1低8--ADC1高8--ADC2低8--ADC2高8--ADC3低8--ADC3高8--ADC4低8--ADC4高8--数据包标识--校验码0xa5;
         前导码只有0x01和0x08才表示有效的数据包，0x01表示此数据包是由ADC采样完成触发的，0x08表示此数据包是由遥控器上的按
         键触发的;
		 数据包标识用于识别是否是同一数据包的作用（这在飞机上主要用于当遥控信号中断时，自动开始降落。）
************************************************************************************************************************/
void Remote_Data_ReceiveAnalysis(void)
{
	SI24R1_Controlflag = 1;
	if(SI24R1_RX_DATA[11]!=0xa5)	//验证校验码是否为0xa5
		return;
	
	if(SI24R1_RX_DATA[0] & 0x01) //当数据包是由遥控器的ADC采样完成时触发发送时
	{								       
		RC_Control.YAW      = SI24R1_RX_DATA[3]<<8|SI24R1_RX_DATA[2];		//ADC2
		RC_Control.THROTTLE = SI24R1_RX_DATA[5]<<8|SI24R1_RX_DATA[4]; 		//ADC1
		RC_Control.ROLL     = SI24R1_RX_DATA[7]<<8|SI24R1_RX_DATA[6];	  	//ADC4
		RC_Control.PITCH    = SI24R1_RX_DATA[9]<<8|SI24R1_RX_DATA[8];		//ADC3

	}
	else if(SI24R1_RX_DATA[0] & 0x08) //当数据包是由遥控器的按键触发发送时
	{							
		Button_command(SI24R1_RX_DATA[1]); //ButtonMask按键命令解析
	}
	DataID = SI24R1_RX_DATA[10];//将数据包识别PID值取出，覆盖之前的值，以表示信号链接正常
}

/**************************************************************************************************************
* 函  数：void Button_command(uint8_t Button)
* 功  能：按键命令解析
* 参  数：Button 按键命令数值
* 返回值：无
* 备  注：遥控器共有四个按键 K1 ,K2 ,K3 ,K4
* K1 	：解锁上锁按键
* K2 	: 用户自定义功能按键
* K3 	: 传感器校准按键
* K4 	: 模式选择按键
* 其中K1,K2,K3,K4 对应ButtonMask的低4位，K1对应0位 K2对应1位 K3对应2位 K4对应3位
* 0x00的时候表示4个按键都没有被触发，0000 1111 表示KEY1~KEY4按键都被按下
* 按键每按一次对应位取反一次(详细操作请参考遥控器源码 senddata.c )      
***************************************************************************************************************/
void Button_command(uint8_t Button)
{
	static uint8_t PreButton = 0;//前一个按键值

	//遥控器K1按键
	if((PreButton&0x01) != (Button&0x01)) 
	{
		if(Button&0x01)//飞机解锁
		{
			Airplane_Enable = 1;
			RGB_LED_FLY();
			RGB_LED_Off();
		}else //飞机上锁
		{
			Airplane_Enable = 0;
			RGB_LED_Off();
		}
	}
	//遥控器K2按键
	if((PreButton&0x02) != (Button&0x02)) 
	{
		/*
			用户自行定义
		*/
	}
	//遥控器K3按键
	if(Button&0x04) //陀螺仪加速度计校准
	{
		SENSER_FLAG_SET(GYRO_OFFSET);
	}
	//遥控器K4按键
	if((PreButton&0x08) != (Button&0x08))
	{
		if(Button&0x08)
		{
			SENSER_FLAG_SET(FLY_MODE); //无头模式
		}else 
		{
			SENSER_FLAG_RESET(FLY_MODE); //有头模式
		}
	}
	PreButton = Button;
}

/**************************************************************************************************************
* 函  数：void UnControl_Land(void)
* 功  能：信号中断紧急降落
* 参  数：无
* 返回值：无
* 备  注：粗略处理，有待完善
***************************************************************************************************************/
void UnControl_Land(void)       
{
	RC_Control.THROTTLE -= 6;
	if(RC_Control.THROTTLE <= 150)
		RC_Control.THROTTLE = 150;
}

/**************************************************************************************************************
* 函  数：void SI24R1_SingalCheck(void)
* 功  能：信号中断检测
* 参  数：无
* 返回值：无
* 备  注：如果飞机处于解锁状态但是,当前数据包的ID等于前一个数据包的ID，这就说明遥控器与飞机断开连接
***************************************************************************************************************/
void SI24R1_SingalCheck(void)
{
	static uint8_t PreDataID = 250; 
	
	if(SI24R1_Controlflag)
	{
		if(Airplane_Enable && DataID == PreDataID)//飞机与遥控断开连接
		{
			UnControl_Land(); //紧急降落处理
			RGB_LED_Red(); 	  //红灯常亮报警
		}else if(Airplane_Enable && !BATT_LEDflag)//飞机遥控连接正常
		{
			RGB_LED_FLY(); 	  //飞行指示灯
		}
		PreDataID = DataID;
	}
}

/**************************************************************************************************************
* 函  数：void SendToRemote(void)
* 功  能：飞机状态数据发送给遥控器
* 参  数：无
* 返回值：无
* 备  注：注意：SI24R1单次发送最大32个字节，请勿越界
***************************************************************************************************************/
void SendToRemote(void)
{
	int16_t temp; 	
	if(Airplane_Enable)
	{
		SENSER_FLAG_SET(FLY_ENABLE); //解锁模式置位
	}
	else
	{
		SENSER_FLAG_RESET(FLY_ENABLE); //上锁模式复位
	}
	SI24R1_TX_DATA[0] = 0xFF;//帧头
	
	SI24R1_TX_DATA[1] = SENSER_OFFSET_FLAG; //标志位组
	
	temp = (u16)RC_Control.THROTTLE; //油门
	SI24R1_TX_DATA[2] = Byte1(temp);
	SI24R1_TX_DATA[3] = Byte0(temp);
	temp = (int)(Att_Angle.yaw*100); //航向
	SI24R1_TX_DATA[4] = Byte1(temp);
	SI24R1_TX_DATA[5] = Byte0(temp);
	temp = (int)(Att_Angle.pit*100); //俯仰
	SI24R1_TX_DATA[6] = Byte1(temp);
	SI24R1_TX_DATA[7] = Byte0(temp);
	temp = (int)(Att_Angle.rol*100); //横滚
	SI24R1_TX_DATA[8] = Byte1(temp);
	SI24R1_TX_DATA[9] = Byte0(temp);
	temp = (int)(FBM.AltitudeFilter*100); //高度留待
	SI24R1_TX_DATA[10] = Byte1(temp);
	SI24R1_TX_DATA[11] = Byte0(temp);
	temp = (int)(BAT.BattMeasureV*100); //飞机电池电压
	SI24R1_TX_DATA[12] = Byte1(temp);
	SI24R1_TX_DATA[13] = Byte0(temp);
	
	SI24R1_TxPacket(SI24R1_TX_DATA); //SI24R1发送函数
}



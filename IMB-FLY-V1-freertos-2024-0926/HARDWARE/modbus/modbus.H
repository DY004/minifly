#ifndef _modbus_H
#define _modbus_H
#include "sys.h"



//void modbus(void);

void Host_Func3(void);
void Modbus_Init(void);

void Modbus_Init(void);
void Modbus_Func3(void);//读寄存器数据
void Modbus_Func6(void);//往1个寄存器中写入数据
void Modbus_Func16(void);//往多个寄存器中写入数据
void Host_Func6(void);
void Modbus_Event(void);
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);
void HOST_ModbusRX(void);
void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num);
void Host_Func10(void);
void Host_write10_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t reg_num,uint8_t Bbyte_NUM,uint16_t num1,uint16_t num2);



/***定义一个结构体：（设备作为主机或从机时均会用到）***/

typedef struct
{
    //作为从机时使用
    uint8_t  myadd;        //本设备从机地址
    u8  rcbuf[100];   //modbus接受缓冲区
    u8  timout;       //modbus数据持续时间
    u8  recount;      //modbus端口接收到的数据个数
    u8  timrun;       //modbus定时器是否计时标志
    u8  reflag;       //modbus一帧数据接受完成标志位
    u8  sendbuf[100]; //modbus接发送缓冲区

    //作为主机添加部分
    u8 Host_Txbuf[20];	//modbus发送数组
    uint8_t slave_add;		//要匹配的从机设备地址（做主机实验时使用）
    u8 Host_send_flag;//主机设备发送数据完毕标志位
    int Host_Sendtime;//发送完一帧数据后时间计数
    u8 Host_time_flag;//发送时间到标志位，=1表示到发送数据时间了
    u8 Host_End;//接收数据后处理完毕
} MODBUS;



extern MODBUS modbus;

extern uint8_t RES;//串口接收缓冲区









#endif



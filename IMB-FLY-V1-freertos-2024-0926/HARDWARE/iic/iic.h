#ifndef   _IIC_H
#define   _IIC_H
#include "stdint.h"
#include "gpio.h"

#define SCL_H         HAL_GPIO_WritePin(MPU6050_SCL_GPIO_Port,MPU6050_SCL_Pin,GPIO_PIN_SET)
#define SCL_L         HAL_GPIO_WritePin(MPU6050_SCL_GPIO_Port,MPU6050_SCL_Pin,GPIO_PIN_RESET)
#define SDA_H         HAL_GPIO_WritePin(MPU6050_SDA_GPIO_Port,MPU6050_SDA_Pin,GPIO_PIN_SET) 
#define SDA_L         HAL_GPIO_WritePin(MPU6050_SDA_GPIO_Port,MPU6050_SDA_Pin,GPIO_PIN_RESET)
#define SDA_read      (HAL_GPIO_ReadPin(MPU6050_SDA_GPIO_Port,MPU6050_SDA_Pin)!=0)?1:0
        

void IIC_GPIO_Init(void);        //初始化IIC的IO口				 
void IIC_Start(void);			 //发送IIC开始信号
void IIC_Stop(void);	  	  	 //发送IIC停止信号
void IIC_Ack(void);				 //IIC发送ACK信号
void IIC_NAck(void);			 //IIC不发送ACK信号
uint8_t IIC_WaitAck(void); 		 //IIC等待ACK信号

void IIC_SendByte(uint8_t data);  //IIC发送一个字节
uint8_t IIC_ReadByte(uint8_t ack);//IIC读取一个字节

uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t buf);
uint8_t IIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);


#endif


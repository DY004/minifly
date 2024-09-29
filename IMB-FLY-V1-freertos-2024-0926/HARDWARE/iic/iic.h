#ifndef   _IIC_H
#define   _IIC_H
#include "stdint.h"
#include "gpio.h"

#define SCL_H         HAL_GPIO_WritePin(MPU6050_SCL_GPIO_Port,MPU6050_SCL_Pin,GPIO_PIN_SET)
#define SCL_L         HAL_GPIO_WritePin(MPU6050_SCL_GPIO_Port,MPU6050_SCL_Pin,GPIO_PIN_RESET)
#define SDA_H         HAL_GPIO_WritePin(MPU6050_SDA_GPIO_Port,MPU6050_SDA_Pin,GPIO_PIN_SET) 
#define SDA_L         HAL_GPIO_WritePin(MPU6050_SDA_GPIO_Port,MPU6050_SDA_Pin,GPIO_PIN_RESET)
#define SDA_read      (HAL_GPIO_ReadPin(MPU6050_SDA_GPIO_Port,MPU6050_SDA_Pin)!=0)?1:0
        

void IIC_GPIO_Init(void);        //��ʼ��IIC��IO��				 
void IIC_Start(void);			 //����IIC��ʼ�ź�
void IIC_Stop(void);	  	  	 //����IICֹͣ�ź�
void IIC_Ack(void);				 //IIC����ACK�ź�
void IIC_NAck(void);			 //IIC������ACK�ź�
uint8_t IIC_WaitAck(void); 		 //IIC�ȴ�ACK�ź�

void IIC_SendByte(uint8_t data);  //IIC����һ���ֽ�
uint8_t IIC_ReadByte(uint8_t ack);//IIC��ȡһ���ֽ�

uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t buf);
uint8_t IIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);


#endif


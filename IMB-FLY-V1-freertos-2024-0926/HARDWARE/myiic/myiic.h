#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "bme280.h"

//IO������
#define SDA_IN()  {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=8<<12;}
#define SDA_OUT() {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=3<<12;}

//IO��������	 
#define IIC_SCL    PAout(15)                                                   //SCL
#define IIC_SDA    PBout(3)                                                   //SDA	 
#define READ_SDA   PBin(3)                                                    //����SDA 

//IIC���в�������
void IIC_Init(void);                                                           //��ʼ��IIC��IO��				 
void MY_IIC_Start(void);				                                           //����IIC��ʼ�ź�
void MY_IIC_Stop(void);	  			                                           //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			                                           //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);                                           //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				                                           //IIC�ȴ�ACK�ź�
void MY_IIC_Ack(void);					                                           //IIC����ACK�ź�
void MY_IIC_NAck(void);				                                           //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8 iicDevReadByte(u8 devaddr,u8 addr);	                                       /*��һ�ֽ�*/
void iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*дһ�ֽ�*/
void iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*������ȡ����ֽ�*/
void iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*����д�����ֽ�*/
void iicDevReadCal(u8 devaddr,u8 addr,u8 len,bme280Calib *bme280Ctype);  
void iicDevReadCal1(u8 devaddr,u8 addr,u8 len,u8 *rbuf);
#endif

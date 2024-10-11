#include "myiic.h"
#include "delay.h"

//初始化IIC
void IIC_Init(void)
{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
//	   
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;                            //推挽输出
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;

}
//产生IIC起始信号
void MY_IIC_Start(void)
{
	SDA_OUT();                                                                 //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;                                                                 //START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;                                                                 //钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void MY_IIC_Stop(void)
{
	SDA_OUT();                                                                 //sda线输出
	IIC_SCL=0;
	IIC_SDA=0;                                                                 //STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;                                                                 //发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();                                                                  //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MY_IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;                                                                 //时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void MY_IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void MY_IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;                                                                 //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);                                                           //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();                                                                  //SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MY_IIC_NAck();                                                            //发送nACK
    else
        MY_IIC_Ack();                                                             //发送ACK   
    return receive;
}

//从指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 iicDevReadByte(u8 devaddr,u8 addr)
{				  
	u8 temp=0;		  	    																 
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);                                                    //发送器件写命令 	   
	IIC_Wait_Ack(); 
	IIC_Send_Byte(addr);                                                       //发送低地址
	IIC_Wait_Ack();	

	MY_IIC_Start();  	 	   
	IIC_Send_Byte(devaddr|1);                                                  //发送器件读命令			   
	IIC_Wait_Ack();	 
	temp=IIC_Read_Byte(0);			   
	MY_IIC_Stop();                                                                //产生一个停止条件	    
	return temp;
}

//连续读多个字节
//addr:起始地址
//rbuf:读数据缓存
//len:数据长度
void iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(addr);                                                       //地址自增  
	IIC_Wait_Ack();	

	MY_IIC_Start();  	
	IIC_Send_Byte(devaddr|1);  	
	IIC_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte(0);                                          //最后一个字节不应答
		}
		else
			rbuf[i]=IIC_Read_Byte(1);
	}
	MY_IIC_Stop( );	
}


void iicDevReadCal1(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(0x88);                                                       //地址自增  
	IIC_Wait_Ack();	
	len=24;
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte(0);                                          //最后一个字节不应答
		}
		else
			rbuf[i]=IIC_Read_Byte(1);
	}
	MY_IIC_Stop( );	
	delay_us(100);
	
	MY_IIC_Start();  
	IIC_Send_Byte(0x76);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(0xA1);                                                       //地址自增  
	IIC_Wait_Ack();	
	rbuf[i++]=IIC_Read_Byte(0); 
	MY_IIC_Stop( );
	delay_us(100);
	
	MY_IIC_Start();  
	IIC_Send_Byte(0x76);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(0xE1);                                                       //地址自增  
	IIC_Wait_Ack();	
	len=32;
	for(; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte(0);                                          //最后一个字节不应答
		}
		else
			rbuf[i]=IIC_Read_Byte(1);
	}
	MY_IIC_Stop( );	
}

//连续读多个字节
//addr:起始地址
//rbuf:读数据缓存
//len:数据长度
void iicDevReadCal(u8 devaddr,u8 addr,u8 len,bme280Calib *bme280Ctype)
{
	int i=0;
	u8 rbuf[32];
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(0x88);                                                       //地址自增  
	IIC_Wait_Ack();	
	len=24;
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte(0);                                          //最后一个字节不应答
		}
		else
			rbuf[i]=IIC_Read_Byte(1);
	}
	MY_IIC_Stop( );	
	delay_us(100);
	
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(0xA1);                                                       //地址自增  
	IIC_Wait_Ack();	
	rbuf[i++]=IIC_Read_Byte(0); 
	MY_IIC_Stop( );
	delay_us(100);
	
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);  
	IIC_Wait_Ack();	
	IIC_Send_Byte(0xE1);                                                       //地址自增  
	IIC_Wait_Ack();	
	len=32;
	for(; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte(0);                                          //最后一个字节不应答
		}
		else
			rbuf[i]=IIC_Read_Byte(1);
	}
	MY_IIC_Stop( );
	
	bme280Ctype->dig_T1=(rbuf[1] << 8) | rbuf[0];
	bme280Ctype->dig_T2=(rbuf[3] << 8) | rbuf[2];
	bme280Ctype->dig_T3=(rbuf[5] << 8) | rbuf[4];
	bme280Ctype->dig_P1=(rbuf[7] << 8) | rbuf[6];
	bme280Ctype->dig_P2=(rbuf[9] << 8) | rbuf[8];
	bme280Ctype->dig_P3=(rbuf[11]<< 8) | rbuf[10];
	bme280Ctype->dig_P4=(rbuf[13]<< 8) | rbuf[12];
	bme280Ctype->dig_P5=(rbuf[15]<< 8) | rbuf[14];
	bme280Ctype->dig_P6=(rbuf[17]<< 8) | rbuf[16];
	bme280Ctype->dig_P7=(rbuf[19]<< 8) | rbuf[18];
	bme280Ctype->dig_P8=(rbuf[21]<< 8) | rbuf[20];
	bme280Ctype->dig_P9=(rbuf[23]<< 8) | rbuf[22];
	bme280Ctype->dig_H1=rbuf[24];
	bme280Ctype->dig_H2=(rbuf[26]<< 8) | rbuf[25];
	bme280Ctype->dig_H3=rbuf[27];
	bme280Ctype->dig_H4=(rbuf[28]<< 4) | (0x0F & rbuf[29]);
	bme280Ctype->dig_H5=(rbuf[30] << 4) | ((rbuf[29] >> 4) & 0x0F);
	bme280Ctype->dig_H6=rbuf[31];  
}

//从指定地址写入一个数据
//WriteAddr :写入数据的目的地址    
//DataToWrite:要写入的数据
void iicDevWriteByte(u8 devaddr,u8 addr,u8 data)
{				   	  	    																 
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);                                                    //发送器件写命令 	 
	IIC_Wait_Ack();	   
	IIC_Send_Byte(addr);                                                       //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(data);                                                       //发送字节							   
	IIC_Wait_Ack();  		    	   
	MY_IIC_Stop();		                                                           //产生一个停止条件 	 
}

//连续写多个字节
//addr:起始地址
//wbuf:写数据缓存
//len:数据的长度
void iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf)
{
	int i=0;
	MY_IIC_Start();  
	IIC_Send_Byte(devaddr);  	
	IIC_Wait_Ack();	
	IIC_Send_Byte(addr);  //地址自增
	IIC_Wait_Ack();	
	for(i=0; i<len; i++)
	{
		IIC_Send_Byte(wbuf[i]);  
		IIC_Wait_Ack();		
	}
	MY_IIC_Stop( );	
}

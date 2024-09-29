#include "modbus.h"
#include "gpio.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

#include "string.h"
#include "stdio.h"
#include "modbus_crc.h"

uint8_t RES;//串口接收缓冲区

MODBUS modbus;//结构体变量







//作为从机部分的代码

uint16_t Reg[] = {0x0001,
                  0x0002,
                  0x0003,
                  0x0004,
                  0x0005,
                  0x0006,
                  0x0007,
                  0X0008,
                 };//reg是提前定义好的寄存器和寄存器数据，要读取和改写的部分内容


// Modbus初始化函数
void Modbus_Init()
{
    modbus.myadd = 0x01; //从机设备地址为1
    modbus.timrun = 0;    //modbus定时器停止计算
    modbus.slave_add=0x01;//主机要匹配的从机地址
}
// Modbus 3号功能码函数
// Modbus 主机读取寄存器值
void Modbus_Func3()
{
    uint16_t Regadd,Reglen,crc;
    uint8_t i,j;
    //得到要读取寄存器的首地址
    Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//读取的首地址
    //得到要读取寄存器的数据长度
    Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
    //发送回应数据包
    i = 0;
    modbus.sendbuf[i++] = modbus.myadd;      //ID号：发送本机设备地址
    modbus.sendbuf[i++] = 0x03;              //发送功能码
    modbus.sendbuf[i++] = ((Reglen*2)%256);   //返回字节个数
    for(j=0; j<Reglen; j++)                  //返回数据
    {
        //reg是提前定义好的16位数组（模仿寄存器）
        modbus.sendbuf[i++] = Reg[Regadd+j]/256;//高位数据
        modbus.sendbuf[i++] = Reg[Regadd+j]%256;//低位数据
    }
    crc = Modbus_CRC16(modbus.sendbuf,i);    //计算要返回数据的CRC
    modbus.sendbuf[i++] = crc/256;//校验位高位
    modbus.sendbuf[i++] = crc%256;//校验位低位
    //数据包打包完成
    // 开始返回Modbus数据

//    RS485_TX_ENABLE;//这是开启485发送

    for(j=0; j<i; j++) //发送数据
    {
        Modbus_Send_Byte(modbus.sendbuf[j]);
    }
//    RS485_RX_ENABLE;//这里是关闭485发送

}


// Modbus 6号功能码函数
// Modbus 主机写入寄存器值
void Modbus_Func6()
{
    uint16_t Regadd;//地址16位
    uint16_t val;//值
    uint16_t i,crc,j;
    i=0;
    Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //得到要修改的地址
    val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //修改后的值（要写入的数据）
    Reg[Regadd]=val;  //修改本设备相应的寄存器

    //以下为回应主机
    modbus.sendbuf[i++]=modbus.myadd;//本设备地址
    modbus.sendbuf[i++]=0x06;        //功能码
    modbus.sendbuf[i++]=Regadd/256;//写入的地址
    modbus.sendbuf[i++]=Regadd%256;
    modbus.sendbuf[i++]=val/256;//写入的数值
    modbus.sendbuf[i++]=val%256;
    crc=Modbus_CRC16(modbus.sendbuf,i);//获取crc校验位
    modbus.sendbuf[i++]=crc/256;  //crc校验位加入包中
    modbus.sendbuf[i++]=crc%256;
    //数据发送包打包完毕
//    RS485_TX_ENABLE;;//使能485控制端(启动发送)
    for(j=0; j<i; j++)
    {
        Modbus_Send_Byte(modbus.sendbuf[j]);
    }
//    RS485_RX_ENABLE;//失能485控制端（改为接收）

}

//这是往多个寄存器器中写入数据
//功能码0x10指令即十进制16
void Modbus_Func16()
{
    uint16_t Regadd;//地址16位
    uint16_t Reglen;
    uint16_t i,crc,j;

    Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //要修改内容的起始地址
    Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
    for(i=0; i<Reglen; i++) //往寄存器中写入数据
    {
        //接收数组的第七位开始是数据
        Reg[Regadd+i]=modbus.rcbuf[7+i*2]*256+modbus.rcbuf[8+i*2];//对寄存器一次写入数据
    }
    //写入数据完毕，接下来需要进行打包回复数据了

    //以下为回应主机内容
    //内容=接收数组的前6位+两位的校验位
    modbus.sendbuf[0]=modbus.rcbuf[0];//本设备地址
    modbus.sendbuf[1]=modbus.rcbuf[1];  //功能码
    modbus.sendbuf[2]=modbus.rcbuf[2];//写入的地址
    modbus.sendbuf[3]=modbus.rcbuf[3];
    modbus.sendbuf[4]=modbus.rcbuf[4];
    modbus.sendbuf[5]=modbus.rcbuf[5];
    crc=Modbus_CRC16(modbus.sendbuf,6);//获取crc校验位
    modbus.sendbuf[6]=crc/256;  //crc校验位加入包中
    modbus.sendbuf[7]=crc%256;
    //数据发送包打包完毕

//    RS485_TX_ENABLE;;//使能485控制端(启动发送)
    for(j=0; j<8; j++)
    {
        Modbus_Send_Byte(modbus.sendbuf[j]);
    }
//    RS485_RX_ENABLE;//失能485控制端（改为接收）

}


// Modbus事件处理函数
void Modbus_Event()
{
    uint16_t crc,rccrc;//crc和接收到的crc
    //没有收到数据包
    if(modbus.reflag == 0)  //如果接收未完成则返回空
    {
//		printf("没有接收到数据\r\n");
        return;
    }
//	printf("收到了数据\r\n");
    //收到数据包(接收完成)
    //通过读到的数据帧计算CRC
    //参数1是数组首地址，参数2是要计算的长度（除了CRC校验位其余全算）
    crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //获取CRC校验位
    // 读取数据帧的CRC
    rccrc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//计算读取的CRC校验位
    //等价于下面这条语句
    //rccrc=modbus.rcbuf[modbus.recount-1]|(((uint16_t)modbus.rcbuf[modbus.recount-2])<<8);//获取接收到的CRC
    if(crc == rccrc) //CRC检验成功 开始分析包
    {
		printf("CRC校验成功\r\n");
        if(modbus.rcbuf[0] == modbus.myadd)  // 检查地址是否时自己的地址
        {
            switch(modbus.rcbuf[1])   //分析modbus功能码
            {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
//				printf("执行到了追一部\r\n");
                Modbus_Func3();
                break;//这是读取寄存器的数据
            case 4:
                break;
            case 5:
                break;
            case 6:
                Modbus_Func6();
                break;//这是写入单个寄存器数据
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 16:
                Modbus_Func16();
                break;//写入多个寄存器数据
            }
        }
        else if(modbus.rcbuf[0] == 0) //广播地址不予回应
        {

        }
    }
    modbus.recount = 0;//接收计数清零
    modbus.reflag = 0; //接收标志清零
}
//作为从机部分内容结束









//下面时作为主机时内容

//主机选择从机
//参数1从机，参数2起始地址，参数3寄存器个数
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num)
{
    int j;
    uint16_t crc;//计算的CRC校验位
    modbus.slave_add=slave;
    modbus.Host_Txbuf[0]=slave;//这是要匹配的从机地址
    modbus.Host_Txbuf[1]=0x03;//功能码
    modbus.Host_Txbuf[2]=StartAddr/256;//起始地址高位
    modbus.Host_Txbuf[3]=StartAddr%256;//起始地址低位
    modbus.Host_Txbuf[4]=num/256;//寄存器个数高位
    modbus.Host_Txbuf[5]=num%256;//寄存器个数低位
    crc=Modbus_CRC16(&modbus.Host_Txbuf[0],6); //获取CRC校验位
    modbus.Host_Txbuf[6]=crc/256;//寄存器个数高位
    modbus.Host_Txbuf[7]=crc%256;//寄存器个数低位

    //发送数据包装完毕
//    RS485_TX_ENABLE;;//使能485控制端(启动发送)
//	HAL_Delay(100);
    for(j=0; j<8; j++)
    {
        Modbus_Send_Byte(modbus.Host_Txbuf[j]);
    }
    HAL_Delay(10);
//    RS485_RX_ENABLE;//失能485控制端（改为接收）
    modbus.Host_send_flag=1;//表示发送数据完毕
}


//第三个是字节个数
//主机处理接受到的数据
void Host_Func3()
{
    int i;
    int count=(int)modbus.rcbuf[2];//这是数据个数

    printf("从机返回 %d 个寄存器数据：\r\n",count/2);
    for(i=0; i<count; i=i+2)
    {
        printf("Temp_Hbit= %d Temp_Lbit= %d temp= %d\r\n",(int)modbus.rcbuf[3+i],(int)modbus.rcbuf[4+i],(int)modbus.rcbuf[4+i]+((int)modbus.rcbuf[3+i])*256);
    }
    modbus.Host_End=1;//接收的数据处理完毕
}

//主机接收从机的消息进行处理
void HOST_ModbusRX()
{
    uint16_t crc,rccrc;//计算crc和接收到的crc

//	printf("0000");
    if(modbus.reflag == 0)  //如果接收未完成则返回空
    {
//		printf("55555");
        return;
    }
    //接收数据结束

    //（数组中除了最后两位CRC校验位其余全算）
    crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //获取CRC校验位
    rccrc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//计算读取的CRC校验位
//	printf("111");
    if(crc == rccrc) //CRC检验成功 开始分析包
    {
        if(modbus.rcbuf[0] == modbus.slave_add)  // 检查地址是是对应从机发过来的
        {
            if(modbus.rcbuf[1]==3)//功能码时03
//				 printf("222");
                Host_Func3();//这是读取寄存器的有效数据位进行计算
        }

    }
    modbus.recount = 0;//接收计数清零
    modbus.reflag = 0; //接收标志清零

}





//向一个寄存器中写数据的参数设置
void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num)
{
    uint16_t crc,j;//计算的CRC校验位
    modbus.slave_add=slave;//从机地址赋值一下，后期有用
    modbus.Host_Txbuf[0]=slave;//这是要匹配的从机地址
    modbus.Host_Txbuf[1]=fun;//功能码
    modbus.Host_Txbuf[2]=StartAddr/256;//起始地址高位
    modbus.Host_Txbuf[3]=StartAddr%256;//起始地址低位
    modbus.Host_Txbuf[4]=num/256;
    modbus.Host_Txbuf[5]=num%256;
    crc=Modbus_CRC16(&modbus.Host_Txbuf[0],6); //获取CRC校验位
    modbus.Host_Txbuf[6]=crc/256;//寄存器个数高位
    modbus.Host_Txbuf[7]=crc%256;//寄存器个数低位

    //发送数据包装完毕
//    RS485_TX_ENABLE;;//使能485控制端(启动发送)
//	HAL_Delay(100);
    for(j=0; j<8; j++)
    {
        Modbus_Send_Byte(modbus.Host_Txbuf[j]);
    }
    HAL_Delay(10);
//    RS485_RX_ENABLE;//失能485控制端（改为接收）
    modbus.Host_send_flag=1;//表示发送数据完毕

}





//从机返回数据
void Host_Func6()
{
    int crc,rccrc;
    crc = Modbus_CRC16(&modbus.rcbuf[0],6); //获取CRC校验位
    rccrc = modbus.rcbuf[6]*256+modbus.rcbuf[7];//计算读取的CRC校验位
    if(crc == rccrc) //CRC检验成功 开始分析包
    {
        if(modbus.rcbuf[0] == modbus.slave_add)  // 检查地址是是对应从机发过来的
        {
            if(modbus.rcbuf[1]==6)//功能码时06
            {

                printf("地址为 %d 的从机寄存器 %d 中写入数据 %d \r\n ",(int)modbus.rcbuf[0],(int)modbus.rcbuf[3]+((int)modbus.rcbuf[2])*256,(int)modbus.rcbuf[5]+((int)modbus.rcbuf[4])*256);
                printf("Host_06 write data right!\r\n");

            }
        }

    }

    modbus.Host_End=1;//接收的数据处理完毕
}


//向多个个寄存器中写数据的参数设置
void Host_write10_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t reg_num,uint8_t Bbyte_NUM,uint16_t num1,uint16_t num2)
{
    uint16_t crc,j;//计算的CRC校验位
    modbus.slave_add=slave;//从机地址赋值一下，后期有用
    modbus.Host_Txbuf[0]=slave;//这是要匹配的从机地址
    modbus.Host_Txbuf[1]=fun;//功能码
    modbus.Host_Txbuf[2]=StartAddr/256;//起始地址高位
    modbus.Host_Txbuf[3]=StartAddr%256;//起始地址低位
	modbus.Host_Txbuf[4]=reg_num/256;//寄存器个数高位
	modbus.Host_Txbuf[5]=reg_num%256;//寄存器个数低位
	
	modbus.Host_Txbuf[6]=Bbyte_NUM;//字节个数
    modbus.Host_Txbuf[7]=num1/256;
    modbus.Host_Txbuf[8]=num1%256;
	modbus.Host_Txbuf[9]=num2/256;
    modbus.Host_Txbuf[10]=num2%256;
    crc=Modbus_CRC16(&modbus.Host_Txbuf[0],11); //获取CRC校验位
    modbus.Host_Txbuf[11]=crc/256;//寄存器个数高位
    modbus.Host_Txbuf[12]=crc%256;//寄存器个数低位

    //发送数据包装完毕
//    RS485_TX_ENABLE;;//使能485控制端(启动发送)
//	HAL_Delay(100);
    for(j=0; j<13; j++)
    {
        Modbus_Send_Byte(modbus.Host_Txbuf[j]);
    }
    HAL_Delay(10);
//    RS485_RX_ENABLE;//失能485控制端（改为接收）
    modbus.Host_send_flag=1;//表示发送数据完毕

}

//从机返回数据
void Host_Func10()
{
    int crc,rccrc;
    crc = Modbus_CRC16(&modbus.rcbuf[0],11); //获取CRC校验位
    rccrc = modbus.rcbuf[11]*256+modbus.rcbuf[12];//计算读取的CRC校验位
    if(crc == rccrc) //CRC检验成功 开始分析包
    {
        if(modbus.rcbuf[0] == modbus.slave_add)  // 检查地址是是对应从机发过来的
        {
            if(modbus.rcbuf[1]==0X10)//功能码时10,改成16进制这里，不然CRC校验不通过。
            {
				printf("已经通过CRC校验，确保了数据传输的正确性\r\n");
                printf("地址为 %d 的从机寄存器 %d 中写入数据 %d \r\n ",(int)modbus.rcbuf[0],(int)modbus.rcbuf[3]+((int)modbus.rcbuf[2])*256,(int)modbus.Host_Txbuf[8]+((int)modbus.Host_Txbuf[7])*256);
				printf("地址为 %d 的从机寄存器 %d 中写入数据 %d \r\n ",(int)modbus.rcbuf[0],((int)modbus.rcbuf[3]+((int)modbus.rcbuf[2])*256)+1,(int)modbus.Host_Txbuf[10]+((int)modbus.Host_Txbuf[9])*256);
                printf("Host_10 write data right!\r\n");
            }
        }

    }

    modbus.Host_End=1;//接收的数据处理完毕
}






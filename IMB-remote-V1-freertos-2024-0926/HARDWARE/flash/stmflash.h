#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "main.h"  
#include "sys.h"  
#include "stm32f1xx_hal.h"

//=========================数据类型宏定义
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define __IO    volatile 
typedef __IO uint16_t vu16;
 
//=========================用户根据自己的需要设置
#define STM32_FLASH_SIZE 	64 	 	//所选STM32的FLASH容量大小(单位为K)
    #if     STM32_FLASH_SIZE < 256      //设置扇区大小
    #define STM_SECTOR_SIZE     1024    //1K字节
    #else 
    #define STM_SECTOR_SIZE	    2048    //2K字节
    #endif	
#define STM32_FLASH_BASE    0x08000000 		//STM32 FLASH的起始地址
#define FLASH_SAVE_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*62	//写Flash的地址，这里从倒数第二页开始
#define STM32_FLASH_WREN 	1              	//使能FLASH写入(0，不是能;1，使能)
#define FLASH_WAITETIME  	50000          	//FLASH等待超时时间
 
 
 
 
u8 STMFLASH_GetStatus(void);				  //获得状态
u8 STMFLASH_WaitDone(u16 time);				  //等待操作结束
u8 STMFLASH_ErasePage(u32 paddr);			  //擦除页
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat);//写入半字
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
void Flash_PageErase(uint32_t PageAddress);     //扇区擦除
						   
#endif
 
 
 

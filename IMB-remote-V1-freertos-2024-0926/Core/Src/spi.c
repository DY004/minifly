/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
#include "si24r1.h"
#include "sys.h"
u8 status;			//记录从STATUS寄存器中返回的状态值

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief       SPI2速度设置函数
 * @note        SPI2时钟选择来自APB1, 即PCLK1, 为45Mhz
 *              SPI速度 = PCLK2 / 2^(speed + 1)
 * @param       speed   : SPI时钟分频系数
 * @retval      无
 */
void spi1_set_speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));          /* 判断有效性 */
    __HAL_SPI_DISABLE(&hspi1);                       /* 关闭SPI */
    hspi1.Instance->CR1 &= 0XFFC7;                   /* 位3-5清零，用来设置波特率 */
    hspi1.Instance->CR1 |= speed;                    /* 设置SPI速度 */
    __HAL_SPI_ENABLE(&hspi1);                        /* 使能SPI */
}

/**
 * @brief       SPI2 读写一个字节
 * @param       txdata:要写入的字节
 * @retval      读取到的字节
 */
uint8_t spi1_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;

    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 1000);

    return rxdata;                      /* 返回收到的数据 */
}



/**********************************************************************
* 用SPI给某器件的寄存器中写入一个字节的数据，即配置寄存器
* 输入参数reg：操作指令+寄存器地址
* 输入参数value:写入寄存器中的数据
***********************************************************************/
u8 SPI_Write_Byte(u8 reg, u8 value)
{
    CSN_LOW;					//拉低从机的片选NSS，从而选中该从机
    status = spi1_read_write_byte(reg);		//只有给从机写入操作指令+寄存器地址时，从机才会返回STATUS寄存器中的值，写入其他的值时，从机不会返回STATUS寄存器中的值
    spi1_read_write_byte(value);
    CSN_HIGH;					//释放片选NSS，SPI总线空闲

    return status;
}


/**********************************************************************
* 用SPI给某器件的寄存器中写入多个字节的值
* 输入参数reg：操作指令+寄存器地址
* 输入参数*pBuf：要写入的数据保存区域首地址
* 输入参数len：要写入的数据的个数
***********************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    CSN_LOW;

    status = spi1_read_write_byte(reg);
    while(len)
    {
        spi1_read_write_byte(*pBuf);
        pBuf++;
        len--;
    }

    CSN_HIGH;

    return status;
}

/**********************************************************************
* 用SPI从某器件的寄存器中读取一个字节的数据
* 输入参数reg：操作指令+寄存器地址
* 返回值：从寄存器中读取的数据
***********************************************************************/
u8 SPI_Read_Byte(u8 reg)
{
    u8 value;
    CSN_LOW;
    spi1_read_write_byte(reg);
    value = spi1_read_write_byte(0);
    CSN_HIGH;

    return value;
}

/**********************************************************************
* 用SPI从某器件的寄存器中读取多个字节的数据
* 输入参数reg：操作指令+寄存器地址
* 输入参数*pBuf：读取到的数据保存区域首地址
* 输入参数len：要读取数据的个数
***********************************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
    CSN_LOW;

    status = spi1_read_write_byte(reg);
    while(len) {
        *pBuf = spi1_read_write_byte(0);
        pBuf++;
        len--;
    }

    CSN_HIGH;

    return status;
}



/* USER CODE END 1 */

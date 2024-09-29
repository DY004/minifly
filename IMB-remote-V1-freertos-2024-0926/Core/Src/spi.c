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
u8 status;			//��¼��STATUS�Ĵ����з��ص�״ֵ̬

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
 * @brief       SPI2�ٶ����ú���
 * @note        SPI2ʱ��ѡ������APB1, ��PCLK1, Ϊ45Mhz
 *              SPI�ٶ� = PCLK2 / 2^(speed + 1)
 * @param       speed   : SPIʱ�ӷ�Ƶϵ��
 * @retval      ��
 */
void spi1_set_speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));          /* �ж���Ч�� */
    __HAL_SPI_DISABLE(&hspi1);                       /* �ر�SPI */
    hspi1.Instance->CR1 &= 0XFFC7;                   /* λ3-5���㣬�������ò����� */
    hspi1.Instance->CR1 |= speed;                    /* ����SPI�ٶ� */
    __HAL_SPI_ENABLE(&hspi1);                        /* ʹ��SPI */
}

/**
 * @brief       SPI2 ��дһ���ֽ�
 * @param       txdata:Ҫд����ֽ�
 * @retval      ��ȡ�����ֽ�
 */
uint8_t spi1_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;

    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 1000);

    return rxdata;                      /* �����յ������� */
}



/**********************************************************************
* ��SPI��ĳ�����ļĴ�����д��һ���ֽڵ����ݣ������üĴ���
* �������reg������ָ��+�Ĵ�����ַ
* �������value:д��Ĵ����е�����
***********************************************************************/
u8 SPI_Write_Byte(u8 reg, u8 value)
{
	CSN_LOW;					//���ʹӻ���ƬѡNSS���Ӷ�ѡ�иôӻ�
	status = spi1_read_write_byte(reg);		//ֻ�и��ӻ�д�����ָ��+�Ĵ�����ַʱ���ӻ��Ż᷵��STATUS�Ĵ����е�ֵ��д��������ֵʱ���ӻ����᷵��STATUS�Ĵ����е�ֵ
	spi1_read_write_byte(value);				
	CSN_HIGH;					//�ͷ�ƬѡNSS��SPI���߿���
	
	return status;
}


/**********************************************************************
* ��SPI��ĳ�����ļĴ�����д�����ֽڵ�ֵ
* �������reg������ָ��+�Ĵ�����ַ
* �������*pBuf��Ҫд������ݱ��������׵�ַ
* �������len��Ҫд������ݵĸ���
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
* ��SPI��ĳ�����ļĴ����ж�ȡһ���ֽڵ�����
* �������reg������ָ��+�Ĵ�����ַ
* ����ֵ���ӼĴ����ж�ȡ������
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
* ��SPI��ĳ�����ļĴ����ж�ȡ����ֽڵ�����
* �������reg������ָ��+�Ĵ�����ַ
* �������*pBuf����ȡ�������ݱ��������׵�ַ
* �������len��Ҫ��ȡ���ݵĸ���
***********************************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	CSN_LOW;
	
	status = spi1_read_write_byte(reg);
	while(len){
		*pBuf = spi1_read_write_byte(0);
		pBuf++;
		len--;
	}

	CSN_HIGH;
	
	return status;
}



/* USER CODE END 1 */
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32f1xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "adc.h"
#include "gpio.h"
#include "deal_datapacket.h"
#include "si24r1.h"
#include "senddata.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
//u16 TX_CNT = 0;                      //遥控数据发送计数
//u16 TX_ERROR = 0;                    //遥控数据发送失败计数
//float TX_ERROR_PERCENT = 0;          //遥控数据丢包率
extern u8 FLY_Connect_OK;            //飞机数据接收完成
extern u8 Reconnection_flag;         //遥控与飞机已连接
extern u8 ADC_CALIBRATOR_OK;         //遥控通道ADC校准标志，原始文件在：adc_dma.h中
extern u8 packetData[11];			 //打包后待发送的数据包，原始定义在：nRF.c文件中
extern vu8 ButtonMask;				 //标记哪个按钮被点击了，原始定义在：button.c文件中
extern vu16 ADC_ConvertedValue[4];	 //用来保存ADC各通道转换完成后的数据，原始定义在：adc_dma.c中


/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t g_adc_dma_sta=0;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line1 interrupt.
  */
//void EXTI1_IRQHandler(void)
//{
//  /* USER CODE BEGIN EXTI1_IRQn 0 */

//  /* USER CODE END EXTI1_IRQn 0 */
//  HAL_GPIO_EXTI_IRQHandler(NRF_IRQ_Pin);
//  /* USER CODE BEGIN EXTI1_IRQn 1 */
////	
////	
////	

//  /* USER CODE END EXTI1_IRQn 1 */
//}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY2_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */
//	 if(__HAL_GPIO_EXTI_GET_FLAG(EXTI_LINE_4) != RESET){
    if((GPIOB->IDR & KEY2_Pin) == RESET) {
        delay_ms(10);										//延迟20ms
        if((GPIOB->IDR & KEY2_Pin) == RESET) {			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
            while((GPIOB->IDR & KEY2_Pin) == RESET);
            printf("KEY2_Pin connect OK!!!\r\n");
            ButtonMask ^= 0x02;								//按键2被按下
            PackData(0x08);
            SI24R1_SendPacket(packetData);
        }
    }
//		EXTI_ClearITPendingBit(EXTI_Line15);
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_4);
//	}

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
//	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
//	{
//		CAL();
//		if(ADC_CALIBRATOR_OK)
//		{
//			ADC_Calibration();
//		}
//		PackData(0x01);							//打包数据，并且前导码为0x01
//		SI24R1_SendPacket(packetData);
//		Get_TxErrorPercent();                  	//求遥控数据丢包率
//		printf("BattV:%0.2f\r\n",ADC_ConvertedValue[4]*3.3*2/4096);
//		DMA_ClearITPendingBit(DMA1_IT_TC1);		//清除DMA1通道1传输完成中断
//		__HAL_DMA_CLEAR_FLAG();
//	}
//	printf("PA0的值：%d\r\n",ADC_ConvertedValue[1]);//根据遥感电位器的图，这个是左右。，数值正确，遥感左边是最小值，右边是最大值

//	printf("PA2的值：%d\r\n",ADC_ConvertedValue[2]);//根据遥感电位器的图，这个是左右。，数值正确，遥感左边是最小值，右边是最大值
//	printf("PA3的值：%d\r\n",AD_value[3]);//根据遥感电位器的图，这个是上下。，数值正确，遥感下边是最小值，上边是最大值
//	HAL_Delay(500);


	if(__HAL_DMA_GET_FLAG(&hdma_adc1,DMA_FLAG_TC1))//传输完成
	{
		g_adc_dma_sta = 1;//标志位置1


		CAL();
		if(ADC_CALIBRATOR_OK)
		{
			ADC_Calibration();
		}
		PackData(0x01);							//打包数据，并且前导码为0x01
		SI24R1_SendPacket(packetData);
		Get_TxErrorPercent();                  	//求遥控数据丢包率
//		printf("BattV:%0.2f\r\n",ADC_ConvertedValue[3]*3.3*2/4096);
		__HAL_DMA_CLEAR_FLAG(&hdma_adc1,DMA_FLAG_TC1);//清除传输完成标志位

	}







  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY1_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */
//	if(__HAL_GPIO_EXTI_GET_FLAG(EXTI_LINE_5) != RESET){
    if((GPIOB->IDR & KEY1_Pin) == RESET) {				//注意：这里不能用==SET来判断，因为SET = !RESET,也就是说SET的数字代表值是非零
        delay_ms(10);										//延迟20ms
        if((GPIOB->IDR & KEY1_Pin) == RESET) {			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
            while((GPIOB->IDR & KEY1_Pin) == RESET);		//等待按钮释放
            printf("KEY1_Pin connect OK!!!\r\n");
            if(ADC_ConvertedValue[1]>=100)					//油门拉倒最低，触发KEY1才是有效的
                return;
            ButtonMask ^= 0x01;														//按键1第一次被按下时为0x01，再次被按下时为0x00，以此重复
            PackData(0x08);									//打包数据，并且前导码为0x08
            SI24R1_SendPacket(packetData);
        }
    }
//		EXTI_ClearITPendingBit(EXTI_Line14);					//清除EXTI中断标志
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_5);
//	}

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY4_Pin);
  HAL_GPIO_EXTI_IRQHandler(KEY3_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */
//	if(__HAL_GPIO_EXTI_GET_FLAG(EXTI_LINE_13) != RESET){
    if((GPIOB->IDR & KEY3_Pin) == RESET) {
        delay_ms(10);										//延迟20ms
        if((GPIOB->IDR & KEY3_Pin) == RESET) {			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
            while((GPIOB->IDR & KEY3_Pin) == RESET);
            printf("KEY3_Pin connect OK!!!\r\n");
            ButtonMask |= 0x04;								//按键3被按下
            PackData(0x08);
            SI24R1_SendPacket(packetData);
        }
        ButtonMask &= ~0x04;
    }
//		EXTI_ClearITPendingBit(EXTI_Line13);
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_13);
//	}

//	else if(__HAL_GPIO_EXTI_GET_FLAG(EXTI_LINE_12) != RESET){
    if((GPIOB->IDR & KEY4_Pin)== RESET) {
        delay_ms(10);										//延迟20ms
        if((GPIOB->IDR & KEY4_Pin)== RESET) {				//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
            while((GPIOB->IDR & KEY4_Pin)== RESET);
            printf("KEY4_Pin connect OK!!!\r\n");
            ButtonMask ^= 0x08;								//按键4被按下
            PackData(0x08);
            SI24R1_SendPacket(packetData);
        }
        //	ButtonMask &= ~0x08;
    }
//		EXTI_ClearITPendingBit(EXTI_Line12);
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_12);
//	}



  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */




/* USER CODE END 1 */

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

#include "si24r1.h"
#include "remotedata.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

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

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
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
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(RF_IRQ_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

    uint8_t sta;
//	if(EXTI_GetITStatus(EXTI_Line2) != RESET )   //这个语句用hal库代替。

//	{
    SI24R1_CE_LOW;//拉低CE，以便读取NRF中STATUS中的数据
    sta = SI24R1_read_reg(R_REGISTER+STATUS); //读取STATUS中的数据，以便判断是由什么中断源触发的IRQ中断

    /* 发送完成中断 TX_OK */
    if(sta & TX_OK)
    {
        SI24R1set_Mode(IT_RX);
        SI24R1_write_reg(W_REGISTER+STATUS,TX_OK); //清除发送完成标志·
        SI24R1_write_reg(FLUSH_TX,0xff); //清除TX_FIFO
//		printf("Sent OK!!!!\r\n");
    }
    /* 接收完成中断 RX_OK */
    if(sta & RX_OK)
    {
        SI24R1_RxPacket(SI24R1_RX_DATA);
        Remote_Data_ReceiveAnalysis();
        SI24R1_write_reg(W_REGISTER+STATUS,RX_OK); //清除发送完成标志·
//		printf("Receive OK!!!!\r\n");
    }
    /* 达到最大重发次数中断  MAX_TX */
    if(sta & MAX_TX)
    {
        SI24R1set_Mode(IT_RX);
        SI24R1_write_reg(W_REGISTER+STATUS,MAX_TX);//清除接达到最大重发标志
        SI24R1_write_reg(FLUSH_TX,0xff); //清除TX_FIFO
//			printf("Sent Max Data!!!\r\n");
    }
//		EXTI_ClearITPendingBit(EXTI_LINE_2);
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_2);
//	}





  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	
//	if(huart1.Instance == USART1)	// 判断是由哪个串口触发的中断
//	{
////		HAL_UART_Transmit(&huart1,aRxBuffer1,1,100);	// 接收到数据马上使用串口1发送出去
//		HAL_UART_Receive_IT(&huart1,aRxBuffer1,1);		// 重新使能串口1接收中断
//	}


  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{

//	if(huart->Instance == USART1)	// 判断是由哪个串口触发的中断
//	{
//		HAL_UART_Transmit(&huart1,aRxBuffer1,1,100);	// 接收到数据马上使用串口1发送出去
//		HAL_UART_Receive_IT(&huart1,aRxBuffer1,1);		// 重新使能串口1接收中断
//	}

//}





/* USER CODE END 1 */

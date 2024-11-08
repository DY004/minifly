/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PWR_ON_OFF.h"
#include "oled.h"
#include "oleddispaly.h"
#include "si24r1.h"
#include "adc.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId PWR_TaskHandle;
osThreadId LED_TaskHandle;
osThreadId OLEDdisplay_TaskHandle;
osThreadId ReconnectionFly_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_PWR_Task(void const * argument);
void Start_LED_Task(void const * argument);
void StartT_OLEDdisplay_Task(void const * argument);
void Start_ReconnectionFly_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of PWR_Task */
  osThreadDef(PWR_Task, Start_PWR_Task, osPriorityNormal, 0, 128);
  PWR_TaskHandle = osThreadCreate(osThread(PWR_Task), NULL);

  /* definition and creation of LED_Task */
  osThreadDef(LED_Task, Start_LED_Task, osPriorityIdle, 0, 128);
  LED_TaskHandle = osThreadCreate(osThread(LED_Task), NULL);

  /* definition and creation of OLEDdisplay_Task */
  osThreadDef(OLEDdisplay_Task, StartT_OLEDdisplay_Task, osPriorityIdle, 0, 128);
  OLEDdisplay_TaskHandle = osThreadCreate(osThread(OLEDdisplay_Task), NULL);

  /* definition and creation of ReconnectionFly_Task */
  osThreadDef(ReconnectionFly_Task, Start_ReconnectionFly_Task, osPriorityIdle, 0, 128);
  ReconnectionFly_TaskHandle = osThreadCreate(osThread(ReconnectionFly_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_PWR_Task */
/**
  * @brief  Function implementing the PWR_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_PWR_Task */
void Start_PWR_Task(void const * argument)
{
  /* USER CODE BEGIN Start_PWR_Task */
    /* Infinite loop */
    for(;;)
    {
        PWR_ON_OFF();
        osDelay(20);
    }
  /* USER CODE END Start_PWR_Task */
}

/* USER CODE BEGIN Header_Start_LED_Task */
/**
* @brief Function implementing the LED_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LED_Task */
void Start_LED_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LED_Task */
    /* Infinite loop */
    for(;;)
    {
        HAL_GPIO_TogglePin(LED_red_GPIO_Port,LED_red_Pin);
//	  HAL_GPIO_TogglePin(LED_green_GPIO_Port,LED_green_Pin);
//        printf("PA0的值：%d\r\n",AD_value[0]);//根据遥感电位器的图，这个是上下。，数值正确，遥感下边是最小值，上边是最大值
//		printf("PA1的值：%d\r\n",AD_value[1]);//根据遥感电位器的图，这个是上下。，数值正确，遥感下边是最小值，上边是最大值
//		printf("PA2的值：%d\r\n",AD_value[2]);//根据遥感电位器的图，这个是上下。，数值正确，遥感下边是最小值，上边是最大值
//		printf("PA3的值：%d\r\n",AD_value[3]);//根据遥感电位器的图，这个是上下。，数值正确，遥感下边是最小值，上边是最大值
        osDelay(500);
    }
  /* USER CODE END Start_LED_Task */
}

/* USER CODE BEGIN Header_StartT_OLEDdisplay_Task */
/**
* @brief Function implementing the OLEDdisplay_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartT_OLEDdisplay_Task */
void StartT_OLEDdisplay_Task(void const * argument)
{
  /* USER CODE BEGIN StartT_OLEDdisplay_Task */
    /* Infinite loop */
    for(;;)
    {
        Display_Update();//OLED更新显示
//	    osDelay(1000);
    }
  /* USER CODE END StartT_OLEDdisplay_Task */
}

/* USER CODE BEGIN Header_Start_ReconnectionFly_Task */
/**
* @brief Function implementing the ReconnectionFly_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_ReconnectionFly_Task */
void Start_ReconnectionFly_Task(void const * argument)
{
  /* USER CODE BEGIN Start_ReconnectionFly_Task */
    /* Infinite loop */
    for(;;)
    {
        ReconnectionFly();//断线重连
//        osDelay(2);
    }
  /* USER CODE END Start_ReconnectionFly_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


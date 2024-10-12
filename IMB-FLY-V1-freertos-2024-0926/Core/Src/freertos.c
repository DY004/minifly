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
#include "WS2812B_led.h"
#include "bme280.h"
#include "paramsave.h"
#include "MPU6050.h"
#include "ANO_DT.h"
#include "imu.h"
#include "si24r1.h"
#include "remotedata.h"
#include "tim.h"

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
osThreadId BMP280_TaskHandle;
osThreadId SI24R1_SingalCheck_TaskHandle;
osThreadId SI24R1_GetAddr_TaskHandle;
osThreadId Airplane_Enable_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_PWR_Task(void const * argument);
void Start_LED_Task(void const * argument);
void Start_BMP280_Task(void const * argument);
void Start_SI24R1_SingalCheck(void const * argument);
void Start_SI24R1_GetAddr(void const * argument);
void Start_Airplane_Enable(void const * argument);

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

  /* definition and creation of BMP280_Task */
  osThreadDef(BMP280_Task, Start_BMP280_Task, osPriorityIdle, 0, 128);
  BMP280_TaskHandle = osThreadCreate(osThread(BMP280_Task), NULL);

  /* definition and creation of SI24R1_SingalCheck_Task */
  osThreadDef(SI24R1_SingalCheck_Task, Start_SI24R1_SingalCheck, osPriorityIdle, 0, 128);
  SI24R1_SingalCheck_TaskHandle = osThreadCreate(osThread(SI24R1_SingalCheck_Task), NULL);

  /* definition and creation of SI24R1_GetAddr_Task */
  osThreadDef(SI24R1_GetAddr_Task, Start_SI24R1_GetAddr, osPriorityIdle, 0, 128);
  SI24R1_GetAddr_TaskHandle = osThreadCreate(osThread(SI24R1_GetAddr_Task), NULL);

  /* definition and creation of Airplane_Enable_Task */
  osThreadDef(Airplane_Enable_Task, Start_Airplane_Enable, osPriorityIdle, 0, 128);
  Airplane_Enable_TaskHandle = osThreadCreate(osThread(Airplane_Enable_Task), NULL);

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
	  HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
	  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
	  osDelay(500);
  }
  /* USER CODE END Start_LED_Task */
}

/* USER CODE BEGIN Header_Start_BMP280_Task */
/**
* @brief Function implementing the BMP280_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_BMP280_Task */
void Start_BMP280_Task(void const * argument)
{
  /* USER CODE BEGIN Start_BMP280_Task */
  /* Infinite loop */
//	float bmp280_temp;
//	float bmp280_press;
//	float bmp280_humi;
//	float high;
	
	
  for(;;)
  {
//		bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
//		osDelay(500);
//		printf("bmp280_press:%f\r\n",bmp280_press);
//		osDelay(100);
//		printf("bmp280_temp :%f\r\n",bmp280_temp);
//		osDelay(100);
////		printf("bmp280_humidity :%f\r\n",bmp280_humi);
////		osDelay(100);
//		printf("bmp280_high :%f\r\n\r\n",high);
//	PID_ReadFlash();
//    SI24R1_SingalCheck(); //2.4G通信检测
//	SendToRemote(); //发送数据给遥控器
	osDelay(5);
  }
  /* USER CODE END Start_BMP280_Task */
}

/* USER CODE BEGIN Header_Start_SI24R1_SingalCheck */
/**
* @brief Function implementing the SI24R1_SingalCheck_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_SI24R1_SingalCheck */
void Start_SI24R1_SingalCheck(void const * argument)
{
  /* USER CODE BEGIN Start_SI24R1_SingalCheck */
  /* Infinite loop */
//	int16_t buff[3];
//	float buff_t;
	
  for(;;)
  {
//	  MPU6050_CalOff_Gyr();
//	  MPU6050_Offset();
//	  MPU6050_testConnection();
//	  ANO_DT_Data_Exchange();
//	  MPU6050_GyroRead(buff);
//	  MPU6050_TempRead(&buff_t);//
//	  Prepare_Data(); //获取姿态解算所需数据
//	  IMUupdate(&Gyr_rad,&Acc_filt,&Att_Angle); //四元数姿态解算  
	  osDelay(20);
  }
  /* USER CODE END Start_SI24R1_SingalCheck */
}

/* USER CODE BEGIN Header_Start_SI24R1_GetAddr */
/**
* @brief Function implementing the SI24R1_GetAddr_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_SI24R1_GetAddr */
void Start_SI24R1_GetAddr(void const * argument)
{
  /* USER CODE BEGIN Start_SI24R1_GetAddr */
  /* Infinite loop */
  for(;;)
  {
//	  ANO_DT_Data_Exchange(); //更新数据到上位机
	  osDelay(10);
  }
  /* USER CODE END Start_SI24R1_GetAddr */
}

/* USER CODE BEGIN Header_Start_Airplane_Enable */
/**
* @brief Function implementing the Airplane_Enable_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Airplane_Enable */
void Start_Airplane_Enable(void const * argument)
{
  /* USER CODE BEGIN Start_Airplane_Enable */
  /* Infinite loop */
  for(;;)
  {
//	  SI24R1_GetAddr(); //分配2.4G地址
//	  osDelay(5);
	  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,50);
	  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,50);
//	  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,200);
//	  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,200);
  }
  /* USER CODE END Start_Airplane_Enable */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


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
#include "kalman_cal.h"
#include "battery_adc.h"
#include "control.h"
#include "usart.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

extern FLOAT_XYZ 	 Gyr_degree,Gyr_rad,Acc_filt;	              //把陀螺仪的各通道读出的数据，转换成弧度制 和 度

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
osThreadId LowVoltage_Alarm_TaskHandle;
osThreadId SendToRemote_TaskHandle;
osThreadId ANO_DT_Data_TaskHandle;
osThreadId Prepare_Data_TaskHandle;
osThreadId IMUupdate_TaskHandle;
osThreadId Control_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_PWR_Task(void const * argument);
void Start_LED_Task(void const * argument);
void Start_BMP280_Task(void const * argument);
void Start_SI24R1_SingalCheck(void const * argument);
void Start_SI24R1_GetAddr(void const * argument);
void Start_LowVoltage_Alarm_Task(void const * argument);
void Start_SendToRemote_Task(void const * argument);
void Start_ANO_DT_Data_Task(void const * argument);
void StartT_Prepare_Data_Task(void const * argument);
void Start_IMUupdate_Task(void const * argument);
void Start_Control_Task(void const * argument);

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

  /* definition and creation of LowVoltage_Alarm_Task */
  osThreadDef(LowVoltage_Alarm_Task, Start_LowVoltage_Alarm_Task, osPriorityIdle, 0, 128);
  LowVoltage_Alarm_TaskHandle = osThreadCreate(osThread(LowVoltage_Alarm_Task), NULL);

  /* definition and creation of SendToRemote_Task */
  osThreadDef(SendToRemote_Task, Start_SendToRemote_Task, osPriorityIdle, 0, 128);
  SendToRemote_TaskHandle = osThreadCreate(osThread(SendToRemote_Task), NULL);

  /* definition and creation of ANO_DT_Data_Task */
  osThreadDef(ANO_DT_Data_Task, Start_ANO_DT_Data_Task, osPriorityIdle, 0, 128);
  ANO_DT_Data_TaskHandle = osThreadCreate(osThread(ANO_DT_Data_Task), NULL);

  /* definition and creation of Prepare_Data_Task */
  osThreadDef(Prepare_Data_Task, StartT_Prepare_Data_Task, osPriorityIdle, 0, 128);
  Prepare_Data_TaskHandle = osThreadCreate(osThread(Prepare_Data_Task), NULL);

  /* definition and creation of IMUupdate_Task */
  osThreadDef(IMUupdate_Task, Start_IMUupdate_Task, osPriorityIdle, 0, 128);
  IMUupdate_TaskHandle = osThreadCreate(osThread(IMUupdate_Task), NULL);

  /* definition and creation of Control_Task */
  osThreadDef(Control_Task, Start_Control_Task, osPriorityIdle, 0, 128);
  Control_TaskHandle = osThreadCreate(osThread(Control_Task), NULL);

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
//	  HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
//	  printf("KEY1_Pin  OK!!!\r\n");
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
//////		printf("bmp280_humidity :%f\r\n",bmp280_humi);
//////		osDelay(100);
//		printf("bmp280_high :%f\r\n\r\n",high);
        osDelay(1000);
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
    for(;;)
    {
        SI24R1_SingalCheck(); //2.4G通信检测
        osDelay(200);
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
        SI24R1_GetAddr(); //分配2.4G地址
        osDelay(400);
    }
  /* USER CODE END Start_SI24R1_GetAddr */
}

/* USER CODE BEGIN Header_Start_LowVoltage_Alarm_Task */
/**
* @brief Function implementing the LowVoltage_Alarm_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LowVoltage_Alarm_Task */
void Start_LowVoltage_Alarm_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LowVoltage_Alarm_Task */
    /* Infinite loop */
    for(;;)
    {
//        BATT_Alarm_LED(); //电池低电压报警
//        LowVoltage_Alarm();	//低电量报警
        osDelay(300);
    }
  /* USER CODE END Start_LowVoltage_Alarm_Task */
}

/* USER CODE BEGIN Header_Start_SendToRemote_Task */
/**
* @brief Function implementing the SendToRemote_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_SendToRemote_Task */
void Start_SendToRemote_Task(void const * argument)
{
  /* USER CODE BEGIN Start_SendToRemote_Task */
    /* Infinite loop */
    for(;;)
    {
        SendToRemote();
        osDelay(300);
    }
  /* USER CODE END Start_SendToRemote_Task */
}

/* USER CODE BEGIN Header_Start_ANO_DT_Data_Task */
/**
* @brief Function implementing the ANO_DT_Data_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_ANO_DT_Data_Task */
void Start_ANO_DT_Data_Task(void const * argument)
{
  /* USER CODE BEGIN Start_ANO_DT_Data_Task */
    /* Infinite loop */
    for(;;)
    {
//        ANO_DT_Data_Exchange(); //更新数据到上位机
		RGB_LED_Runing();
        osDelay(500);
    }
  /* USER CODE END Start_ANO_DT_Data_Task */
}

/* USER CODE BEGIN Header_StartT_Prepare_Data_Task */
/**
* @brief Function implementing the Prepare_Data_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartT_Prepare_Data_Task */
void StartT_Prepare_Data_Task(void const * argument)
{
  /* USER CODE BEGIN StartT_Prepare_Data_Task */
    /* Infinite loop */
    for(;;)
    {
        Prepare_Data(); //获取姿态解算所需数据
        osDelay(10);
    }
  /* USER CODE END StartT_Prepare_Data_Task */
}

/* USER CODE BEGIN Header_Start_IMUupdate_Task */
/**
* @brief Function implementing the IMUupdate_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_IMUupdate_Task */
void Start_IMUupdate_Task(void const * argument)
{
  /* USER CODE BEGIN Start_IMUupdate_Task */
    /* Infinite loop */
    for(;;)
    {
        IMUupdate(&Gyr_rad,&Acc_filt,&Att_Angle); //四元数姿态解算
        osDelay(10);
    }
  /* USER CODE END Start_IMUupdate_Task */
}

/* USER CODE BEGIN Header_Start_Control_Task */
/**
* @brief Function implementing the Control_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Control_Task */
void Start_Control_Task(void const * argument)
{
  /* USER CODE BEGIN Start_Control_Task */
    /* Infinite loop */
    for(;;)
    {
		Control(&Att_Angle,&Gyr_rad,&RC_Control,Airplane_Enable); //姿态控制
//		ANO_DT_Data_Receive_Prepare(*aRxBuffer1);
        osDelay(10);
    }
  /* USER CODE END Start_Control_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


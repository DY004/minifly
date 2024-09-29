/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RF_IRQ_Pin GPIO_PIN_2
#define RF_IRQ_GPIO_Port GPIOB
#define RF_CSN_Pin GPIO_PIN_12
#define RF_CSN_GPIO_Port GPIOB
#define RF_CE_Pin GPIO_PIN_8
#define RF_CE_GPIO_Port GPIOA
#define MPU6050_SCL_Pin GPIO_PIN_15
#define MPU6050_SCL_GPIO_Port GPIOA
#define MPU6050_SDA_Pin GPIO_PIN_3
#define MPU6050_SDA_GPIO_Port GPIOB
#define MPU6050_INT_Pin GPIO_PIN_4
#define MPU6050_INT_GPIO_Port GPIOB
#define PW_ON_OFF_Pin GPIO_PIN_5
#define PW_ON_OFF_GPIO_Port GPIOB
#define PW_EN_Pin GPIO_PIN_6
#define PW_EN_GPIO_Port GPIOB
#define RGBLED_Pin GPIO_PIN_9
#define RGBLED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

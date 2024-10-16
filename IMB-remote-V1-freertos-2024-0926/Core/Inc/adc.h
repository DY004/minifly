/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "sys.h"
extern vu16 ADC_Calibrator[4];      //遥控通道ADC校准值
extern vu16 ADC_ConvertedValue[4];  //遥控通道ADC值
extern uint16_t AD_value[4];
void ADC_Calibration(void);

#define adc_max  10
extern  uint16_t My_adcData [adc_max];
typedef struct {
	uint16_t value1;
	uint16_t value2;
	uint16_t value3;
	uint16_t value4;
}adcValue_type;

extern uint16_t AD_value[4];
 
extern  adcValue_type adcValue ;
 
 
void ADC_dispose (void);

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */


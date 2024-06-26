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
#include "stm32f0xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Sound_Pin GPIO_PIN_14
#define Sound_GPIO_Port GPIOC
#define MEM_NSS_Pin GPIO_PIN_15
#define MEM_NSS_GPIO_Port GPIOC
#define PWM_Pin GPIO_PIN_1
#define PWM_GPIO_Port GPIOB
#define Converter_Fault_Pin GPIO_PIN_2
#define Converter_Fault_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_10
#define LCD_D7_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_11
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_12
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_13
#define LCD_D4_GPIO_Port GPIOB
#define LCD_Enable_Pin GPIO_PIN_14
#define LCD_Enable_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_15
#define LCD_RS_GPIO_Port GPIOB
#define Protection_Reset_Pin GPIO_PIN_6
#define Protection_Reset_GPIO_Port GPIOC
#define Fan_Control_Pin GPIO_PIN_7
#define Fan_Control_GPIO_Port GPIOC
#define Heater_Control_Pin GPIO_PIN_8
#define Heater_Control_GPIO_Port GPIOC
#define UP_Pin GPIO_PIN_8
#define UP_GPIO_Port GPIOA
#define DOWN_Pin GPIO_PIN_9
#define DOWN_GPIO_Port GPIOA
#define LEFT_Pin GPIO_PIN_10
#define LEFT_GPIO_Port GPIOA
#define RIGHT_Pin GPIO_PIN_11
#define RIGHT_GPIO_Port GPIOA
#define NC_Pin GPIO_PIN_12
#define NC_GPIO_Port GPIOA
#define CH2_LO_Pin GPIO_PIN_6
#define CH2_LO_GPIO_Port GPIOB
#define CH1_LO_Pin GPIO_PIN_7
#define CH1_LO_GPIO_Port GPIOB
#define CH2_HI_Pin GPIO_PIN_8
#define CH2_HI_GPIO_Port GPIOB
#define CH1_HI_Pin GPIO_PIN_9
#define CH1_HI_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define LENGTH_DATA_ADC 9
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

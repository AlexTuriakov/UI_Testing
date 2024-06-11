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
#include "stm32f4xx_hal.h"

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
#define Protection_Reset_Pin GPIO_PIN_2
#define Protection_Reset_GPIO_Port GPIOE
#define Sound_Pin GPIO_PIN_5
#define Sound_GPIO_Port GPIOE
#define Temp1_Pin GPIO_PIN_0
#define Temp1_GPIO_Port GPIOC
#define Temp2_Pin GPIO_PIN_1
#define Temp2_GPIO_Port GPIOC
#define Temp3_Pin GPIO_PIN_2
#define Temp3_GPIO_Port GPIOC
#define Temp4_Pin GPIO_PIN_3
#define Temp4_GPIO_Port GPIOC
#define Ch1_Current_Pin GPIO_PIN_0
#define Ch1_Current_GPIO_Port GPIOA
#define Ch2_Current_Pin GPIO_PIN_1
#define Ch2_Current_GPIO_Port GPIOA
#define Ch1_Voltage_Pin GPIO_PIN_2
#define Ch1_Voltage_GPIO_Port GPIOA
#define Ch2_Voltage_Pin GPIO_PIN_3
#define Ch2_Voltage_GPIO_Port GPIOA
#define Offset_Ref_Pin GPIO_PIN_4
#define Offset_Ref_GPIO_Port GPIOA
#define PhA_Lo_Pin GPIO_PIN_7
#define PhA_Lo_GPIO_Port GPIOA
#define Bus_Voltage_Pin GPIO_PIN_4
#define Bus_Voltage_GPIO_Port GPIOC
#define LCD_D0_Pin GPIO_PIN_0
#define LCD_D0_GPIO_Port GPIOB
#define LCD_D1_Pin GPIO_PIN_1
#define LCD_D1_GPIO_Port GPIOB
#define LCD_D2_Pin GPIO_PIN_2
#define LCD_D2_GPIO_Port GPIOB
#define Ch1_Lo_Pin GPIO_PIN_8
#define Ch1_Lo_GPIO_Port GPIOE
#define Ch1_Hi_Pin GPIO_PIN_9
#define Ch1_Hi_GPIO_Port GPIOE
#define Ch2_Lo_Pin GPIO_PIN_10
#define Ch2_Lo_GPIO_Port GPIOE
#define Ch2_Hi_Pin GPIO_PIN_11
#define Ch2_Hi_GPIO_Port GPIOE
#define Converter_Fault_Pin GPIO_PIN_15
#define Converter_Fault_GPIO_Port GPIOE
#define LCD_RS_Pin GPIO_PIN_10
#define LCD_RS_GPIO_Port GPIOB
#define PhB_Lo_Pin GPIO_PIN_14
#define PhB_Lo_GPIO_Port GPIOB
#define PhA_Hi_Pin GPIO_PIN_6
#define PhA_Hi_GPIO_Port GPIOC
#define PhB_Hi_Pin GPIO_PIN_7
#define PhB_Hi_GPIO_Port GPIOC
#define SWIO_Pin GPIO_PIN_13
#define SWIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define LEFT_Pin GPIO_PIN_0
#define LEFT_GPIO_Port GPIOD
#define RIGHT_Pin GPIO_PIN_1
#define RIGHT_GPIO_Port GPIOD
#define UP_Pin GPIO_PIN_2
#define UP_GPIO_Port GPIOD
#define DOWN_Pin GPIO_PIN_3
#define DOWN_GPIO_Port GPIOD
#define OK_Pin GPIO_PIN_4
#define OK_GPIO_Port GPIOD
#define LCD_D3_Pin GPIO_PIN_3
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOB
#define LCD_Enable_Pin GPIO_PIN_8
#define LCD_Enable_GPIO_Port GPIOB
#define LCD_R_W_Pin GPIO_PIN_9
#define LCD_R_W_GPIO_Port GPIOB
#define Fan_Control_Pin GPIO_PIN_0
#define Fan_Control_GPIO_Port GPIOE
#define Heater_Control_Pin GPIO_PIN_1
#define Heater_Control_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define LENGTH_DATA_ADC 9
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

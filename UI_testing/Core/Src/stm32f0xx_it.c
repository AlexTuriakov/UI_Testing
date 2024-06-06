/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
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
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "keypad.h"
#include "conversion_data.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "climat_regulator.h"
#include "dessipator_control.h"
#include "cells_voltcontrol.h"
#include "converter_fault.h"
#include "auxiliary_function.h"
#include "flash_operation.h"
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
extern DMA_HandleTypeDef hdma_adc;
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern TIM_HandleTypeDef htim6;
/* USER CODE BEGIN EV */
extern volatile uint32_t rawAdcData[LENGTH_DATA_ADC];
unsigned long int qcall = 0;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
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
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
   SET_BIT(hdma_adc.DmaBaseAddress->IFCR, DMA_IFCR_CHTIF1);
   SET_BIT(hdma_adc.DmaBaseAddress->IFCR, DMA_IFCR_CTCIF1);
   SET_BIT(hdma_adc.DmaBaseAddress->IFCR, DMA_IFCR_CGIF1);
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

  /* USER CODE END DMA1_Channel2_3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global and DAC channel underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  HAL_DAC_IRQHandler(&hdac);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
  CLEAR_BIT(TIM6->SR, TIM_SR_UIF);
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM6){
		BUTTON_TimerProcess();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	sphisicValueEx_t measuringValue = {0};
			/*BatteryTester_ConversionData_calcPhisicValueFromAdcCodeEx(
				rawAdcData, LENGTH_DATA_ADC);*/
	BatteryTester_ConverterFault_faultHandler();
	BattetyTester_DessipatorControl_onHeaterControl(
				measuringValue.busVoltageInV);
	BatteryTester_CellsVoltcontrol_protectVoltageCellx(
			CELL_ONE, measuringValue.ch1_VoltageInV);
	if(BatteryTester_RegulatorCellOne_getRunStatus()){
		if(BatteryTester_AuxiliaryFunction_isTimeLoggedCellOne()){
			 BatteryTester_EEPROM_logTestingDatasCellOne(
					HAL_GetTick(), measuringValue.ch1_CurrentInA,
					measuringValue.ch1_VoltageInV, measuringValue.AverageTemps);
		}
		/*float sp = BatteryTester_RegulatorCellOne_getSetpoint();

		if(sp > 0.0){
			BatteryTester_RegulatorCellOne_setPulse(
					BatteryTester_RegulatorCellOne_updateBuck(sp,
							measuringValue.ch1_CurrentInA));
		}
		else
			if(sp < 0.0){
				BatteryTester_RegulatorCellOne_setPulse(
						BatteryTester_RegulatorCellOne_updateBoost(sp,
								measuringValue.ch1_CurrentInA));
			}
			else{
				BatteryTester_RegulatorCellOne_setPulse(0);
			}*/
	}
	BatteryTester_CellsVoltcontrol_protectVoltageCellx(
				CELL_TWO, measuringValue.ch2_VoltageInV);
	if(BatteryTester_RegulatorCellTwo_getRunStatus()){
		if(BatteryTester_AuxiliaryFunction_isTimeLoggedCellTwo()){
			 BatteryTester_EEPROM_logTestingDatasCellTwo(
					HAL_GetTick(), measuringValue.ch2_CurrentInA,
					measuringValue.ch2_VoltageInV, measuringValue.AverageTemps);
		}
		/*float sp = BatteryTester_RegulatorCellTwo_getSetpoint();
		if(sp > 0.0){
			BatteryTester_RegulatorCellTwo_setPulse(
					BatteryTester_RegulatorCellTwo_updateBuck(sp,
							measuringValue.ch2_CurrentInA));
		}
		else
			if(sp < 0.0){
				BatteryTester_RegulatorCellTwo_setPulse(
						BatteryTester_RegulatorCellTwo_updateBoost(sp,
								measuringValue.ch2_CurrentInA));
			}
			else{
				BatteryTester_RegulatorCellTwo_setPulse(0);
			}*/
	}
	if(BatteryTester_ClimatRegulator_getRunStatus()){
		/*float sp = BatteryTester_ClimatRegulator_getSetpoint();
		BatteryTester_ClimatRegulator_setPulse(
				BatteryTester_ClimatRegulator_update(sp,
						measuringValue.AverageTemps));*/
	}
}
/* USER CODE END 1 */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
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
#include "stm32f4xx_it.h"
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
void HAL_ADC_MultiModeDMAConvCplt(ADC_HandleTypeDef*);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
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
  * @brief This function handles Pre-fetch fault, memory access fault.
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
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
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
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */
  /*SET_BIT(hdma_adc1.DmaBaseAddress->IFCR, DMA_IFCR_CHTIF1);
  SET_BIT(hdma_adc1.DmaBaseAddress->IFCR, DMA_IFCR_CTCIF1);
  SET_BIT(hdma_adc1.DmaBaseAddress->IFCR, DMA_IFCR_CGIF1);*/
  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM6){
		BUTTON_TimerProcess();
	}
}

void HAL_ADC_MultiModeDMAConvCplt(ADC_HandleTypeDef* hadc){
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
		float sp = BatteryTester_RegulatorCellOne_getSetpoint();

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
			}
	}
	BatteryTester_CellsVoltcontrol_protectVoltageCellx(
				CELL_TWO, measuringValue.ch2_VoltageInV);
	if(BatteryTester_RegulatorCellTwo_getRunStatus()){
		if(BatteryTester_AuxiliaryFunction_isTimeLoggedCellTwo()){
			 BatteryTester_EEPROM_logTestingDatasCellTwo(
					HAL_GetTick(), measuringValue.ch2_CurrentInA,
					measuringValue.ch2_VoltageInV, measuringValue.AverageTemps);
		}
		float sp = BatteryTester_RegulatorCellTwo_getSetpoint();
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
			}
	}
	if(BatteryTester_ClimatRegulator_getRunStatus()){
		float sp = BatteryTester_ClimatRegulator_getSetpoint();
		BatteryTester_ClimatRegulator_setPulse(
				BatteryTester_ClimatRegulator_update(sp,
						measuringValue.AverageTemps));
	}
}
/* USER CODE END 1 */

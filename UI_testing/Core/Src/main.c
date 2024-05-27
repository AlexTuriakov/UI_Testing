/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_wc1602a.h"
#include "keypad.h"
#include "state_events.h"
#include "cells_voltcontrol.h"
#include "climat_regulator.h"
#include "dessipator_control.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "conversion_data.h"
#include "cells_voltcontrol.h"
#include "converter_fault.h"
#include "spi_eeprom.h"
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
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */
volatile uint32_t rawAdcData[LENGTH_DATA_ADC];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM6_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void BatteryTester_HAL_startAdcDmaCallback(void);
void BatteryTester_HAL_setDacCallback(unsigned int);
void BatteryTester_HAL_stopAdcCallback(void);
void BatteryTester_HAL_startCh1PwmCallback();
void BatteryTester_HAL_stopCh1PwmCallback();
void BatteryTester_HAL_setCh1PwmPulseCallback(unsigned int);
void BatteryTester_HAL_startCh2PwmCallback();
void BatteryTester_HAL_stopCh2PwmCallback();
void BatteryTester_HAL_setCh2PwmPulseCallback(unsigned int);
void BatteryTester_HAL_startThermostatCallback();
void BatteryTester_HAL_stopThermostatCallback();
void BatteryTester_HAL_setThermostatPwmPulseCallback(int pulse);
void BatteryTester_HAL_onDessipatorHardwareCallback();
void BatteryTester_HAL_offDessipatorHardwareCallback();
eBool_t BatteryTester_HAL_isStartCh1PwmCallback();
eBool_t BatteryTester_HAL_isStartCh2PwmCallback();
eBool_t BatteryTester_HAL_isConverterFaultCallback();
void BatteryTester_HAL_resetConverterFaultCallback();
void BatteryTester_HAL_onSoundHardwareCallback();
void BatteryTester_HAL_offSoundHardwareCallback();
uint8_t BatteryTester_HAL_transmitReceiveSpiCallback(uint8_t data);
void BatteryTester_HAL_selectChipCallback(eCSAdapter_t select);
void BatteryTester_HAL_receiveSpiCallback(uint8_t* pBuffer, uint16_t size, uint32_t timeout);
void BatteryTester_HAL_transmitSpiCallback(uint8_t* pBuffer, uint16_t size, uint32_t timeout);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM14_Init();
  MX_TIM6_Init();
  MX_ADC_Init();
  MX_TIM1_Init();
  MX_DAC_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  if(HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK){
	  Error_Handler();
  }
  BatteryTester_WC1602A_init();
  HAL_Delay(3000);
  BatteryTester_WC1602A_offCursor();

  WRITE_REG(htim14.Instance->CCR1, 79);

  BatteryTester_State_initState();
  BatteryTester_RegulatorCellOne_initDecorator(
  		  BatteryTester_HAL_startCh1PwmCallback,
  		  BatteryTester_HAL_stopCh1PwmCallback,
  		  BatteryTester_HAL_setCh1PwmPulseCallback);
  BatteryTester_RegulatorCellTwo_initDecorator(
		  BatteryTester_HAL_startCh2PwmCallback,
		  BatteryTester_HAL_stopCh2PwmCallback,
		  BatteryTester_HAL_setCh2PwmPulseCallback);
  BatteryTester_ClimatRegulator_initDecorator(
		  BatteryTester_HAL_startThermostatCallback,
		  BatteryTester_HAL_stopThermostatCallback,
		  BatteryTester_HAL_setThermostatPwmPulseCallback);
  BatteryTester_DessipatorControl_initDecorator(
		  BatteryTester_HAL_onDessipatorHardwareCallback,
		  BatteryTester_HAL_offDessipatorHardwareCallback, 0, 0);
  BatteryTester_CellsVoltcontrol_initDecorator(
		  BatteryTester_HAL_isStartCh1PwmCallback,
		  BatteryTester_HAL_isStartCh2PwmCallback);
  BatteryTester_ConverterFault_initDecorator(
		  BatteryTester_HAL_isConverterFaultCallback,
		  BatteryTester_HAL_resetConverterFaultCallback,
		  BatteryTester_HAL_onSoundHardwareCallback,
		  BatteryTester_HAL_offSoundHardwareCallback);
  BatteryTester_AT25SF081_initDecorator(
		  BatteryTester_HAL_transmitReceiveSpiCallback,
		  BatteryTester_HAL_selectChipCallback,
		  BatteryTester_HAL_receiveSpiCallback,
		  BatteryTester_HAL_transmitSpiCallback);

  if(HAL_TIM_Base_Start_IT(&htim6) != HAL_OK){
 	  Error_Handler();
   }
   if(BatteryTester_ConversionData_initDecorator(
 		BatteryTester_HAL_startAdcDmaCallback,
 		BatteryTester_HAL_setDacCallback,
 		BatteryTester_HAL_stopAdcCallback) != HAL_OK){
 	  Error_Handler();
   }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  BatteryTester_State_processState();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_TRGO;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 499;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 7;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 0;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 159;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 159;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */
  HAL_TIM_MspPostInit(&htim14);

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 0;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 499;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 10;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */
  HAL_TIM_MspPostInit(&htim16);

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 0;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 499;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 10;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */
  HAL_TIM_MspPostInit(&htim17);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Sound_Pin|MEM_NSS_Pin|Protection_Reset_Pin|Fan_Control_Pin
                          |Heater_Control_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D7_Pin|LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |LCD_Enable_Pin|LCD_RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Sound_Pin Protection_Reset_Pin Fan_Control_Pin Heater_Control_Pin */
  GPIO_InitStruct.Pin = Sound_Pin|Protection_Reset_Pin|Fan_Control_Pin|Heater_Control_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : MEM_NSS_Pin */
  GPIO_InitStruct.Pin = MEM_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MEM_NSS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Converter_Fault_Pin */
  GPIO_InitStruct.Pin = Converter_Fault_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Converter_Fault_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D7_Pin LCD_D6_Pin LCD_D5_Pin LCD_D4_Pin
                           LCD_Enable_Pin LCD_RS_Pin */
  GPIO_InitStruct.Pin = LCD_D7_Pin|LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |LCD_Enable_Pin|LCD_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : UP_Pin DOWN_Pin LEFT_Pin RIGHT_Pin */
  GPIO_InitStruct.Pin = UP_Pin|DOWN_Pin|LEFT_Pin|RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : NC_Pin */
  GPIO_InitStruct.Pin = NC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NC_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void  BatteryTester_HAL_startAdcDmaCallback(void){
	if(HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIM_Base_Start(&htim1) != HAL_OK){
		Error_Handler();
	}
	if(HAL_ADC_Start_DMA(&hadc, (uint32_t*) rawAdcData, LENGTH_DATA_ADC) != HAL_OK){
		Error_Handler();
	}
}

void BatteryTester_HAL_setDacCallback(unsigned int code){
	if(HAL_DAC_Start(&hdac, DAC_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	WRITE_REG(hdac.Instance->DHR12R1, code);
}

void BatteryTester_HAL_stopAdcCallback(void){
	if(HAL_ADC_Stop_DMA(&hadc) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIM_Base_Stop(&htim1) != HAL_OK){
		Error_Handler();
	}
}

void BatteryTester_HAL_startCh1PwmCallback(){
	/*if(HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIMEx_PWMN_Start(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}*/
	/********FOR TESTING***********/
	if (TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_stopCh1PwmCallback(){
	/*if(HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIMEx_PWMN_Stop(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}*/
	/********FOR TESTING***********/
	if (TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_setCh1PwmPulseCallback(unsigned int pulse){
	//WRITE_REG(htim17.Instance->CCR1, pulse);
	/********FOR TESTING***********/
	WRITE_REG(htim2.Instance->CCR1, pulse);
}

void BatteryTester_HAL_startCh2PwmCallback(){
	/*if(HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIMEx_PWMN_Start(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}*/
	/********FOR TESTING***********/
	if(TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_stopCh2PwmCallback(){
	/*if(HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIMEx_PWMN_Stop(&htim17, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}*/
	/********FOR TESTING***********/
	if(TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_setCh2PwmPulseCallback(unsigned int pulse){
	//WRITE_REG(htim17.Instance->CCR1, pulse);
	/********FOR TESTING***********/
	WRITE_REG(htim2.Instance->CCR2, pulse);
}

void BatteryTester_HAL_startThermostatCallback(){
	if(TIM_CHANNEL_STATE_GET(&htim17, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim17, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIMEx_PWMN_Start(&htim17, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_STATE_GET(&htim16, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim16, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIMEx_PWMN_Start(&htim16, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_stopThermostatCallback(){
	if(TIM_CHANNEL_STATE_GET(&htim17, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim17, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIMEx_PWMN_Stop(&htim17, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_STATE_GET(&htim16, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim16, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIMEx_PWMN_Stop(&htim16, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_setThermostatPwmPulseCallback(int pulse){
	unsigned int pulsePhA = 50 + pulse / 2;
	unsigned int pulsePhB = 50 - pulse / 2;
	WRITE_REG(htim17.Instance->CCR1, pulsePhA);
	WRITE_REG(htim16.Instance->CCR1, pulsePhB);
}

void BatteryTester_HAL_onDessipatorHardwareCallback(){
	HAL_GPIO_WritePin(
				Heater_Control_GPIO_Port,
				Heater_Control_Pin,
				GPIO_PIN_SET);
	HAL_GPIO_WritePin(
				Fan_Control_GPIO_Port,
				Fan_Control_Pin,
				GPIO_PIN_SET);
}

void BatteryTester_HAL_offDessipatorHardwareCallback(){
	HAL_GPIO_WritePin(
				Heater_Control_GPIO_Port,
				Heater_Control_Pin,
				GPIO_PIN_RESET);
	HAL_GPIO_WritePin(
				Fan_Control_GPIO_Port,
				Fan_Control_Pin,
				GPIO_PIN_RESET);
}

eBool_t BatteryTester_HAL_isStartCh1PwmCallback(){
	if (TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		return TRUE;
	}
	return FALSE;
}

eBool_t BatteryTester_HAL_isStartCh2PwmCallback(){
	if(TIM_CHANNEL_STATE_GET(&htim2, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
		return TRUE;
	}
	return FALSE;
}

eBool_t BatteryTester_HAL_isConverterFaultCallback(){
	if(HAL_GPIO_ReadPin(Converter_Fault_GPIO_Port, Converter_Fault_Pin) == GPIO_PIN_SET){
		return TRUE;
	}
	return FALSE;
}

/*@brief: Hardware reset converter fault signal callback
 * @danger: use blocking wait
 */
void BatteryTester_HAL_resetConverterFaultCallback(){
	HAL_GPIO_WritePin(
			Protection_Reset_GPIO_Port,
			Protection_Reset_Pin,
			GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(
			Protection_Reset_GPIO_Port,
			Protection_Reset_Pin,
			GPIO_PIN_RESET);
}

void BatteryTester_HAL_onSoundHardwareCallback(){
	HAL_GPIO_WritePin(
			Sound_GPIO_Port,
			Sound_Pin,
			GPIO_PIN_SET);
}

void BatteryTester_HAL_offSoundHardwareCallback(){
	HAL_GPIO_WritePin(
			Sound_GPIO_Port,
			Sound_Pin,
			GPIO_PIN_RESET);
}

uint8_t BatteryTester_HAL_transmitReceiveSpiCallback(uint8_t data){
	uint8_t ret;
	if(HAL_SPI_TransmitReceive(&hspi1, &data, &ret, 1, 100) != HAL_OK){
		Error_Handler();
	}
	return ret;
}

void BatteryTester_HAL_selectChipCallback(eCSAdapter_t select){
	HAL_GPIO_WritePin(MEM_NSS_GPIO_Port, MEM_NSS_Pin, select);
}

/*@brief  Receive an amount of data in blocking mode.
* @param  hspi pointer to a SPI_HandleTypeDef structure that contains
*               the configuration information for SPI module.
* @param  pData pointer to data buffer
* @param  Size amount of data to be received
* @param  Timeout Timeout duration
* @retval HAL status
*/
void BatteryTester_HAL_receiveSpiCallback(uint8_t* pBuffer, uint16_t size, uint32_t timeout){
	if(HAL_SPI_Receive(&hspi1, pBuffer, size, timeout) != HAL_OK){
		Error_Handler();
	}
}

/**
	  * @brief  Transmit an amount of data in blocking mode.
	  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
	  *               the configuration information for SPI module.
	  * @param  pData pointer to data buffer
	  * @param  Size amount of data to be sent
	  * @param  Timeout Timeout duration
	  * @retval HAL status
	  */
void BatteryTester_HAL_transmitSpiCallback(uint8_t* pBuffer, uint16_t size, uint32_t timeout){
	if(HAL_SPI_Transmit(&hspi1, pBuffer, size, timeout) != HAL_OK){
		Error_Handler();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

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
#include "flash_operation.h"
#include "auxiliary_function.h"
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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;

CRC_HandleTypeDef hcrc;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim8;

/* USER CODE BEGIN PV */
volatile uint32_t rawAdcData[LENGTH_DATA_ADC * 10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM6_Init(void);
static void MX_SPI3_Init(void);
static void MX_CRC_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
void BatteryTester_HAL_setThermostatPwmPulseCallback(int);
void BatteryTester_HAL_onDessipatorHardwareCallback();
void BatteryTester_HAL_offDessipatorHardwareCallback();
eBool_t BatteryTester_HAL_isStartCh1PwmCallback();
eBool_t BatteryTester_HAL_isStartCh2PwmCallback();
eBool_t BatteryTester_HAL_isConverterFaultCallback();
void BatteryTester_HAL_resetConverterFaultCallback();
void BatteryTester_HAL_onSoundHardwareCallback();
void BatteryTester_HAL_offSoundHardwareCallback();
//uint8_t BatteryTester_HAL_transmitReceiveSpiCallback(uint8_t);
//void BatteryTester_HAL_selectChipCallback(eCSAdapter_t);
void BatteryTester_HAL_receiveSpiCallback(uint8_t*, uint16_t, uint32_t);
void BatteryTester_HAL_transmitSpiCallback(uint8_t*, uint16_t, uint32_t);
void BatteryTester_HAL_transmitSpiDmaCallback(
		unsigned char*, unsigned char*, unsigned int);
void BatteryTester_HAL_transmitReceiveSpiDmaCallback(
		unsigned char*, unsigned char*, unsigned int);
eBool_t BatteryTester_HAL_isBusySpiCallback(void);
unsigned char BatteryTester_HAL_calculateCrcCallback(
		unsigned char*, unsigned int);
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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  MX_SPI3_Init();
  MX_CRC_Init();
  MX_DAC_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  /*if(HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK){
 	  Error_Handler();
   }*/
   BatteryTester_WC1602A_init();
   HAL_Delay(1000);
   BatteryTester_WC1602A_offCursor();

   //WRITE_REG(htim14.Instance->CCR1, 79);
   BatteryTester_State_initState();
   if(BatteryTester_EEPROM_initDecorator(
 		  BatteryTester_HAL_transmitSpiDmaCallback,
 		  BatteryTester_HAL_transmitReceiveSpiDmaCallback,
 		  BatteryTester_HAL_isBusySpiCallback,
 		  BatteryTester_HAL_calculateCrcCallback) != HAL_OK){
 	  Error_Handler();
   }
   if(BatteryTester_EEPROM_begin()){
 		BatteryTester_AuxiliaryFunction_initLogger();
 		BatteryTester_RegulatorCellOne_initDecorator(
 			  BatteryTester_HAL_startCh1PwmCallback,
 			  BatteryTester_HAL_stopCh1PwmCallback,
 			  BatteryTester_HAL_setCh1PwmPulseCallback,
 			  BatteryTester_HAL_isStartCh1PwmCallback);
 		BatteryTester_RegulatorCellTwo_initDecorator(
 			  BatteryTester_HAL_startCh2PwmCallback,
 			  BatteryTester_HAL_stopCh2PwmCallback,
 			  BatteryTester_HAL_setCh2PwmPulseCallback,
 			  BatteryTester_HAL_isStartCh2PwmCallback);
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
 		/*BatteryTester_AT25SF081_initDecorator(
 			  BatteryTester_HAL_transmitReceiveSpiCallback,
 			  BatteryTester_HAL_selectChipCallback,
 			  BatteryTester_HAL_receiveSpiCallback,
 			  BatteryTester_HAL_transmitSpiCallback);*/

 		if(HAL_TIM_Base_Start_IT(&htim6) != HAL_OK){
 		  Error_Handler();
 		 }
 		 if(BatteryTester_ConversionData_initDecorator(
 			BatteryTester_HAL_startAdcDmaCallback,
 			BatteryTester_HAL_setDacCallback,
 			BatteryTester_HAL_stopAdcCallback) != HAL_OK){
 		  Error_Handler();
 	     }

 	     BatteryTester_EEPROM_end();
   }
   else{
 	  Error_Handler();
   }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  BatteryTester_ConversionData_processTemperatureDecimator();
	  BatteryTester_State_processState();
	  BatteryTester_EEPROM_processLogging();

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_TRIPLEMODE_REGSIMULT;
  multimode.DMAAccessMode = ADC_DMAACCESSMODE_1;
  multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_15CYCLES;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ENABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 3;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ENABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 3;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 50;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 500;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 5249;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  htim6.Init.Prescaler = 83;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 50;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Protection_Reset_Pin|Sound_Pin|Fan_Control_Pin|Heater_Control_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_RS_Pin
                          |LCD_D3_Pin|LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin
                          |LCD_D7_Pin|LCD_Enable_Pin|LCD_R_W_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Protection_Reset_Pin */
  GPIO_InitStruct.Pin = Protection_Reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Protection_Reset_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Sound_Pin Fan_Control_Pin Heater_Control_Pin */
  GPIO_InitStruct.Pin = Sound_Pin|Fan_Control_Pin|Heater_Control_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D0_Pin LCD_D1_Pin LCD_D2_Pin LCD_RS_Pin
                           LCD_D3_Pin LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin
                           LCD_D7_Pin LCD_Enable_Pin LCD_R_W_Pin */
  GPIO_InitStruct.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_RS_Pin
                          |LCD_D3_Pin|LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin
                          |LCD_D7_Pin|LCD_Enable_Pin|LCD_R_W_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Converter_Fault_Pin */
  GPIO_InitStruct.Pin = Converter_Fault_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Converter_Fault_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LEFT_Pin RIGHT_Pin UP_Pin DOWN_Pin
                           OK_Pin */
  GPIO_InitStruct.Pin = LEFT_Pin|RIGHT_Pin|UP_Pin|DOWN_Pin
                          |OK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void  BatteryTester_HAL_startAdcDmaCallback(void){
	if(HAL_TIM_Base_Start(&htim2) != HAL_OK){
		Error_Handler();
	}
	if(HAL_ADC_Start(&hadc2) != HAL_OK){
		Error_Handler();
	}
	if(HAL_ADC_Start(&hadc3) != HAL_OK){
		Error_Handler();
	}
	if(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*) rawAdcData, LENGTH_DATA_ADC * 10) != HAL_OK){
		Error_Handler();
	}
}

void BatteryTester_HAL_setDacCallback(unsigned int code){
  	/*if(HAL_DAC_Start(&hdac, DAC_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	WRITE_REG(hdac.Instance->DHR12R1, code);*/
}

void BatteryTester_HAL_stopAdcCallback(void){
	if(HAL_ADCEx_MultiModeStop_DMA(&hadc1) != HAL_OK){
		Error_Handler();
	}
	if(HAL_ADC_Stop(&hadc2) != HAL_OK){
		Error_Handler();
	}
	if(HAL_ADC_Stop(&hadc3) != HAL_OK){
		Error_Handler();
	}
	if(HAL_TIM_Base_Stop(&htim2) != HAL_OK){
		Error_Handler();
	}
}

void BatteryTester_HAL_startCh1PwmCallback(){
	if (TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_stopCh1PwmCallback(){
	if (TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_setCh1PwmPulseCallback(unsigned int pulse){
	WRITE_REG(htim1.Instance->CCR1, pulse);
}

void BatteryTester_HAL_startCh2PwmCallback(){
	if (TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_stopCh2PwmCallback(){
	if (TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_setCh2PwmPulseCallback(unsigned int pulse){
	WRITE_REG(htim1.Instance->CCR2, pulse);
}

void BatteryTester_HAL_startThermostatCallback(){
	if(TIM_CHANNEL_STATE_GET(&htim8, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim8, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_STATE_GET(&htim8, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim8, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_stopThermostatCallback(){
	if(TIM_CHANNEL_STATE_GET(&htim8, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim8, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIMEx_PWMN_Stop(&htim8, TIM_CHANNEL_1) != HAL_OK){
			Error_Handler();
		}
	}
	if(TIM_CHANNEL_STATE_GET(&htim8, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
	if (TIM_CHANNEL_N_STATE_GET(&htim8, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIMEx_PWMN_Stop(&htim8, TIM_CHANNEL_2) != HAL_OK){
			Error_Handler();
		}
	}
}

void BatteryTester_HAL_setThermostatPwmPulseCallback(int pulse){
	unsigned int pulsePhA = 50 + pulse / 2;
	unsigned int pulsePhB = 50 - pulse / 2;
	WRITE_REG(htim8.Instance->CCR1, pulsePhA);
	WRITE_REG(htim8.Instance->CCR2, pulsePhB);
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
	if (TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_1) == HAL_TIM_CHANNEL_STATE_BUSY){
		return TRUE;
	}
	return FALSE;
}

eBool_t BatteryTester_HAL_isStartCh2PwmCallback(){
	if(TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_2) == HAL_TIM_CHANNEL_STATE_BUSY){
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

/*uint8_t BatteryTester_HAL_transmitReceiveSpiCallback(uint8_t data){
	uint8_t ret;
	if(HAL_SPI_TransmitReceive(&hspi1, &data, &ret, 1, 100) != HAL_OK){
		Error_Handler();
	}
	return ret;
}

void BatteryTester_HAL_selectChipCallback(eCSAdapter_t select){
	HAL_GPIO_WritePin(MEM_NSS_GPIO_Port, MEM_NSS_Pin, select);
}*/

/*@brief  Receive an amount of data in blocking mode.
* @param  hspi pointer to a SPI_HandleTypeDef structure that contains
*               the configuration information for SPI module.
* @param  pData pointer to data buffer
* @param  Size amount of data to be received
* @param  Timeout Timeout duration
* @retval HAL status
*/
void BatteryTester_HAL_receiveSpiCallback(uint8_t* pBuffer, uint16_t size, uint32_t timeout){
	/*if(HAL_SPI_Receive(&hspi1, pBuffer, size, timeout) != HAL_OK){
		Error_Handler();
	}*/
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
	/*if(HAL_SPI_Transmit(&hspi1, pBuffer, size, timeout) != HAL_OK){
		Error_Handler();
	}*/
}

void BatteryTester_HAL_transmitSpiDmaCallback(
		unsigned char* pBufferTx, unsigned char* notUsed, unsigned int sizeOfBytes){
	if(HAL_SPI_Transmit_DMA(&hspi3, (uint8_t*)pBufferTx, (uint16_t)sizeOfBytes) != HAL_OK){
		Error_Handler();
	}
}

void BatteryTester_HAL_transmitReceiveSpiDmaCallback(
		unsigned char* pBufferTx, unsigned char* pBufferRx, unsigned int sizeOfBytes){
	if(HAL_SPI_TransmitReceive_DMA(&hspi3, (uint8_t*)pBufferTx, (uint8_t*)pBufferRx,
					  (uint16_t)sizeOfBytes) != HAL_OK){
		Error_Handler();
	}
}

eBool_t BatteryTester_HAL_isBusySpiCallback(void){
	if (hspi3.State != HAL_SPI_STATE_READY)
	{
		return TRUE;
	}
	return FALSE;
}

unsigned char BatteryTester_HAL_calculateCrcCallback(
		unsigned char* pData, unsigned int sizeOfBytes){
	return (unsigned char)HAL_CRC_Calculate(&hcrc, (uint32_t*)pData, (uint32_t)sizeOfBytes);
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

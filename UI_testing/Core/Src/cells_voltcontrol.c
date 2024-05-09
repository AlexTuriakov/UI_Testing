/*
 * cells_voltcontrol.c
 *
 *  Created on: Apr 17, 2024
 *      Author: Tyuryakov_OA
 */
#include "cells_voltcontrol.h"
#include "stm32f0xx_hal.h"

	sVoltRange_t voltRangeCellOne = {0};
	sVoltRange_t voltRangeCellTwo = {0};
	BatteryCellVoltageStatusProtect_t statusProtectCellOne = VOLTAGE_PROTECT_CELL_OFF;
	BatteryCellVoltageStatusProtect_t statusProtectCellTwo = VOLTAGE_PROTECT_CELL_OFF;

extern TIM_HandleTypeDef htim1;

const unsigned short SIZE_VOLT_RANGE = sizeof(sVoltRange_t) / sizeof (float);

BatteryCellVoltageStatusProtect_t BatteryTester_CellsVoltcontrol_protectVoltageCellx(
		BatteryCellSelector_t cell, float input){
	switch(cell){
	case CELL_ONE:{
		if(!statusProtectCellOne && input <= voltRangeCellOne.minVoltageInVolts){
//			 BatteryTester_CellsVoltcontrol_stopPWMCh1();
			statusProtectCellOne = VOLTAGE_PROTECT_CELL_ON;
		}
		return statusProtectCellOne;
	}
	case CELL_TWO:{
		if(!statusProtectCellTwo && input <= voltRangeCellTwo.minVoltageInVolts){
//			BatteryTester_CellsVoltcontrol_stopPWMCh2();
			statusProtectCellTwo = VOLTAGE_PROTECT_CELL_ON;
		}
		return statusProtectCellTwo;
	}
	}

	return VOLTAGE_PROTECT_CELL_OFF;
}

BatteryCellVoltageStatusProtect_t BatteryTester_CellsVoltcontrol_getStatusProtectVoltageCellx(
		BatteryCellSelector_t cell){
	switch(cell){
	case CELL_ONE:{
		return statusProtectCellOne;
	}
	case CELL_TWO:{
		return statusProtectCellTwo;
	}
	}
	return VOLTAGE_PROTECT_CELL_OFF;
}

void BatteryTester_CellsVoltcontrol_resetProtectVoltageCellx(BatteryCellSelector_t cell){
	switch(cell){
	case CELL_ONE:{
		statusProtectCellOne = VOLTAGE_PROTECT_CELL_OFF;
		return;
	}
	case CELL_TWO:{
		statusProtectCellTwo = VOLTAGE_PROTECT_CELL_OFF;
		return;
	}
	}
}

sVoltRange_t BatteryTester_CellsVoltcontrol_getVoltRangeCellx(BatteryCellSelector_t cell){
	switch(cell){
	case CELL_ONE:{
		return voltRangeCellOne;
	}
	case CELL_TWO:{
		return voltRangeCellTwo;
	}
	default:{
		sVoltRange_t zero = {0};
		return zero;
	}
	}
}

void BatteryTester_CellsVoltcontrol_setVoltRangeCellx(BatteryCellSelector_t cell, sVoltRange_t* pRange){
	if(!pRange){
		return;
	}

	sVoltRange_t* pOutput;
	switch(cell){
	case CELL_ONE:{
		pOutput = &voltRangeCellOne;
		break;
	}
	case CELL_TWO:{
		pOutput = &voltRangeCellTwo;
		break;
	}
	default:{
		return;
	}
	}

	for(int i = 0; i < SIZE_VOLT_RANGE; i++){
		*(((float*)pOutput) + i) = *(((float*)pRange) + i);
	}
}

/*@brief:
 * Reading parameters from flash memory is under development
 * */
void BatteryTester_CellsVoltcontrol_initVoltageProtectCells(){
	voltRangeCellOne.minVoltageInVolts = 2.8;
	voltRangeCellOne.maxVoltageInVolts = 3.6;

	voltRangeCellTwo.minVoltageInVolts = 2.8;
	voltRangeCellTwo.maxVoltageInVolts = 3.6;
	/*implement reading parameters from flash memory*/
}

void BatteryTester_CellsVoltcontrol_stopPWMCh1(){
	if(TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_1) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_1) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
	}

}
void BatteryTester_CellsVoltcontrol_stopPWMCh2(){
	if(TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_2) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_2) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
	}
}

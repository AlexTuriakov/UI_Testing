/*
 * cells_voltcontrol.c
 *
 *  Created on: Apr 17, 2024
 *      Author: Tyuryakov_OA
 */
#include "cells_voltcontrol.h"
#include "stm32f0xx_hal.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "auxiliary_function.h"

#define TESTING

/*********declaration of variables***********/
static	sVoltRange_t voltRangeCellOne = {0};
static	sVoltRange_t voltRangeCellTwo = {0};
static	eBatteryCellVoltageStatusProtect_t statusProtectCellOne = VOLTAGE_PROTECT_CELL_OFF;
static	eBatteryCellVoltageStatusProtect_t statusProtectCellTwo = VOLTAGE_PROTECT_CELL_OFF;
const unsigned short SIZE_VOLT_RANGE = sizeof(sVoltRange_t) / sizeof (float);
static BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t g_isStartCh1Callback;
static BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t g_isStartCh2Callback;


/**********functional implementation**************/
void BatteryTester_CellsVoltcontrol_protectVoltageCellx(
		eBatteryCellSelector_t cell, float input){
	switch(cell){
	case CELL_ONE:{
		if(BatteryTester_CellsVoltcontrol_isStartCellOne()){
			if(input < voltRangeCellOne.minVoltageInVolts ||
				input > voltRangeCellOne.maxVoltageInVolts){
				BatteryTester_RegulatorCellOne_stopHardware();
				statusProtectCellOne = VOLTAGE_PROTECT_CELL_ON;
			}
			else{
				statusProtectCellOne = VOLTAGE_PROTECT_CELL_OFF;
			}
		}
		else{
			statusProtectCellOne = VOLTAGE_PROTECT_CELL_OFF;
		}
		return;
	}
	case CELL_TWO:{
		if(BatteryTester_CellsVoltcontrol_isStartCellTwo()){
			if(input < voltRangeCellTwo.minVoltageInVolts ||
				input > voltRangeCellTwo.maxVoltageInVolts){
				BatteryTester_RegulatorCellTwo_stopHardware();
				statusProtectCellTwo = VOLTAGE_PROTECT_CELL_ON;
			}
			else{
				statusProtectCellTwo = VOLTAGE_PROTECT_CELL_OFF;
			}
		}
		else{
			statusProtectCellTwo = VOLTAGE_PROTECT_CELL_OFF;
		}
		return;
	}
	}
}

eBatteryCellVoltageStatusProtect_t BatteryTester_CellsVoltcontrol_getStatusProtectVoltageCellx(
		eBatteryCellSelector_t cell){
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

void BatteryTester_CellsVoltcontrol_resetProtectVoltageCellx(eBatteryCellSelector_t cell){
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

sVoltRange_t BatteryTester_CellsVoltcontrol_getVoltRangeCellx(eBatteryCellSelector_t cell){
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

void BatteryTester_CellsVoltcontrol_setVoltRangeCellx(eBatteryCellSelector_t cell, sVoltRange_t* pRange){
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
	__BatteryTester_AuxiliaryFunction_copy((void*)pRange, (void*) pOutput, SIZE_VOLT_RANGE);

/*	for(int i = 0; i < SIZE_VOLT_RANGE; i++){
		*(((float*)pOutput) + i) = *(((float*)pRange) + i);
	}*/
}

/*@brief:
 * Reading parameters from flash memory is under development
 * */
void BatteryTester_CellsVoltcontrol_initDecorator(
		BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t isStartCh1Callback,
		BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t isStartCh2Callback){
	g_isStartCh1Callback = isStartCh1Callback;
	g_isStartCh2Callback = isStartCh2Callback;

	if(BatteryTester_CellsVoltcontrol_readDataFromEEPROM() != HAL_OK){
		voltRangeCellOne.minVoltageInVolts = 2.5;
		voltRangeCellOne.maxVoltageInVolts = 3.65;

		voltRangeCellTwo.minVoltageInVolts = 2.5;
		voltRangeCellTwo.maxVoltageInVolts = 3.65;
	}

	/*implement reading parameters from flash memory*/
}

HAL_StatusTypeDef BatteryTester_CellsVoltcontrol_readDataFromEEPROM(){
#ifdef TESTING
	voltRangeCellOne.minVoltageInVolts = 2.5;
	voltRangeCellOne.maxVoltageInVolts = 3.65;

	voltRangeCellTwo.minVoltageInVolts = 2.5;
	voltRangeCellTwo.maxVoltageInVolts = 3.65;
#endif
	return HAL_OK;
}

eBool_t BatteryTester_CellsVoltcontrol_isStartCellOne(){
	if(g_isStartCh1Callback){
		return g_isStartCh1Callback();
	}
	return TRUE;
}

eBool_t BatteryTester_CellsVoltcontrol_isStartCellTwo(){
	if(g_isStartCh2Callback){
		return g_isStartCh2Callback();
	}
	return TRUE;
}

/*@brief: BatteryTester_CellsVoltcontrol_stopPWMCh1
 *@deprecated
 */
void BatteryTester_CellsVoltcontrol_stopPWMCh1(){
/*
	if(TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_1) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_1) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
	}
*/
}

/*@brief: BatteryTester_CellsVoltcontrol_stopPWMCh2
 *@deprecated
 */
void BatteryTester_CellsVoltcontrol_stopPWMCh2(){
/*	if(TIM_CHANNEL_STATE_GET(&htim1, TIM_CHANNEL_2) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	}
	if(TIM_CHANNEL_N_STATE_GET(&htim1, TIM_CHANNEL_2) != HAL_TIM_CHANNEL_STATE_READY){
		HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
	}*/
}

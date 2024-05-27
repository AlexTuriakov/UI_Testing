/*
 * auxiliary_function.c
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */
#include "auxiliary_function.h"

static unsigned int g_periodLoggingTestCellOneInMilliSec = 10000;
static unsigned int g_periodLoggingTestCellTwoInMilliSec = 10000;
static unsigned int g_startTickForTimerCellOne = 0;
static unsigned int g_startTickForTimerCellTwo = 0;


inline void __BatteryTester_AuxiliaryFunction_copy(
		void* pInput, void* pOutput, unsigned int size){
	for(int i = 0; i < size; i++){
		*(((float*)pOutput) + i) = *(((float*)pInput) + i);
	}
}

void BatteryTester_AuxiliaryFunction_setStartTickForTimerCellOne(){
	g_startTickForTimerCellOne = HAL_GetTick();
}

void BatteryTester_AuxiliaryFunction_setStatrTickForTimerCellTwo(){
	g_startTickForTimerCellTwo = HAL_GetTick();
}

void BatteryTester_AuxiliaryFunction_setPeriodLoggingTestCellOneInMillisec(unsigned int period){
	g_periodLoggingTestCellOneInMilliSec = period;
}

void  BatteryTester_AuxiliaryFunction_setPeriodLoggingTestCellTwoInMillisec(unsigned int period){
	g_periodLoggingTestCellTwoInMilliSec = period;
}

unsigned int BatteryTester_AuxiliaryFunction_getPeriodLoggingTestCellOneInMillisec(){
	return g_periodLoggingTestCellOneInMilliSec;
}

unsigned int  BatteryTester_AuxiliaryFunction_getPeriodLoggingTestCellTwoInMillisec(){
	return g_periodLoggingTestCellTwoInMilliSec;
}

eBool_t  BatteryTester_AuxiliaryFunction_isTimeLoggedCellOne(){
	if(HAL_GetTick() - g_startTickForTimerCellOne >= g_periodLoggingTestCellOneInMilliSec){
		g_startTickForTimerCellOne = HAL_GetTick();
		return TRUE;
	}
	return FALSE;
}

eBool_t BatteryTester_AuxiliaryFunction_isTimeLoggedCellTwo(){
	if(HAL_GetTick() - g_startTickForTimerCellTwo >= g_periodLoggingTestCellTwoInMilliSec){
		g_startTickForTimerCellTwo = HAL_GetTick();
		return TRUE;
	}
	return FALSE;
}

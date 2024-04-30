/*
 * dessipator_control.c
 *
 *  Created on: Apr 18, 2024
 *      Author: Tyuryakov_OA
 */

#include "dessipator_control.h"
#include "main.h"
#include "stm32f0xx_hal.h"

sVoltRange_t dessipatorControlRange = {0};
DessipatorStatus_t dessipatorStatus = DESSIPATOR_OFF;


void BatteryTester_DessipatorControl_initHeaterControl(){
	dessipatorControlRange.minVoltageInVolts = 12.0;
	dessipatorControlRange.maxVoltageInVolts = 12.3;
}

void BattetyTester_DessipatorControl_onHeaterControl(float inputBusVoltage){
	if(!dessipatorStatus &&
			inputBusVoltage >= dessipatorControlRange.maxVoltageInVolts){
		BatteryTester_DessipatorControl_onHeater();
		BatteryTester_DessipatorControl_onFan();
		dessipatorStatus = DESSIPATOR_ON;
	}
	else if(dessipatorStatus &&
			inputBusVoltage <= dessipatorControlRange.minVoltageInVolts){
		BatteryTester_DessipatorControl_offHeater();
		BatteryTester_DessipatorControl_offFan();
		dessipatorStatus = DESSIPATOR_OFF;
	}
}

void BatteryTester_DessipatorControl_onHeater(){
/*	HAL_GPIO_WritePin(
			Heater_Control_GPIO_Port,
			Heater_Control_Pin,
			GPIO_PIN_SET);*/
}

void BatteryTester_DessipatorControl_offHeater(){
/*	HAL_GPIO_WritePin(
			Heater_Control_GPIO_Port,
			Heater_Control_Pin,
			GPIO_PIN_RESET);*/
}

void BatteryTester_DessipatorControl_onFan(){
/*	HAL_GPIO_WritePin(
			Fan_Control_GPIO_Port,
			Fan_Control_Pin,
			GPIO_PIN_SET);*/
}

void BatteryTester_DessipatorControl_offFan(){
/*	HAL_GPIO_WritePin(
			Fan_Control_GPIO_Port,
			Fan_Control_Pin,
			GPIO_PIN_RESET);*/
}

sVoltRange_t BatteryTester_DessipatorControl_getHeaterControlRange(){
	return dessipatorControlRange;
}

void BatteryTester_DessipatorControl_setHeaterControlRange(
		sVoltRange_t* pRange){
	if(!pRange){
		return;
	}
	dessipatorControlRange.minVoltageInVolts =
			pRange->minVoltageInVolts;
	dessipatorControlRange.maxVoltageInVolts =
			pRange->maxVoltageInVolts;
}

void BatteryTester_DessipatorControl_resetDessipatorStatus(){
	dessipatorStatus = DESSIPATOR_OFF;
}

DessipatorStatus_t BatteryTester_DessipatorControl_getDessipatorStatus(){
	return dessipatorStatus;
}

void BatteryTester_DessipatorControl_setHeaterControlMin(float newVal){
	dessipatorControlRange.minVoltageInVolts = newVal;
}
void BatteryTester_DessipatorControl_setHeaterControlMax(float newVal){
	dessipatorControlRange.maxVoltageInVolts = newVal;
}

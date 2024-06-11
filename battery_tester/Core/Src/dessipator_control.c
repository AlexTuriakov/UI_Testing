/*
 * dessipator_control.c
 *
 *  Created on: Apr 18, 2024
 *      Author: Tyuryakov_OA
 */

#include "dessipator_control.h"
#include "flash_operation.h"

#define TESTING

/*********declaration of variables***********/
static sVoltRange_t dessipatorControlRange = {0};
static eDessipatorStatus_t dessipatorStatus = DESSIPATOR_OFF;
static BatteryTester_DessipatorControl_toggleHardware_t g_onHeaterCallback = 0;
static BatteryTester_DessipatorControl_toggleHardware_t g_offHeaterCallback = 0;
static BatteryTester_DessipatorControl_toggleHardware_t g_onFanCallback = 0;
static BatteryTester_DessipatorControl_toggleHardware_t g_offFanCallback = 0;


/**********functional implementation**************/
void BatteryTester_DessipatorControl_initDecorator(
		BatteryTester_DessipatorControl_toggleHardware_t onHeaterCallback,
		BatteryTester_DessipatorControl_toggleHardware_t offHeaterCallback,
		BatteryTester_DessipatorControl_toggleHardware_t onFanCallback,
		BatteryTester_DessipatorControl_toggleHardware_t offFanCallback){

	g_onHeaterCallback = onHeaterCallback;
	g_offHeaterCallback = offHeaterCallback;
	g_onFanCallback = onFanCallback;
	g_offFanCallback = offFanCallback;
	if(!BatteryTester_DessipatorControl_readDataFromEEPROM()){
		dessipatorControlRange.minVoltageInVolts = 12.0;
		dessipatorControlRange.maxVoltageInVolts = 14.8;
	}
}

/*@brief:
 *
 */
eBool_t BatteryTester_DessipatorControl_readDataFromEEPROM(){
#ifdef TESTING

#endif
	return BatteryTester_EEPROM_readSetDessipator(&dessipatorControlRange);
}

void BattetyTester_DessipatorControl_onHeaterControl(float inputBusVoltage){
	if(!dessipatorStatus &&
			inputBusVoltage >= dessipatorControlRange.maxVoltageInVolts){
		BatteryTester_DessipatorControl_onHeater();
		BatteryTester_DessipatorControl_onFan();
	}
	else if(dessipatorStatus &&
			inputBusVoltage <= dessipatorControlRange.minVoltageInVolts){
		BatteryTester_DessipatorControl_offHeater();
		BatteryTester_DessipatorControl_offFan();
	}
}

void BatteryTester_DessipatorControl_onHeater(){
/*	HAL_GPIO_WritePin(Heater_Control_GPIO_Port,Heater_Control_Pin,GPIO_PIN_SET);*/
	if(g_onHeaterCallback){
		g_onHeaterCallback();
		dessipatorStatus = DESSIPATOR_ON;
	}
}

void BatteryTester_DessipatorControl_offHeater(){
/*	HAL_GPIO_WritePin(Heater_Control_GPIO_Port,Heater_Control_Pin,GPIO_PIN_RESET);*/
	if(g_offHeaterCallback){
		g_offHeaterCallback();
		dessipatorStatus = DESSIPATOR_OFF;
	}
}

void BatteryTester_DessipatorControl_onFan(){
/*	HAL_GPIO_WritePin(Fan_Control_GPIO_Port,Fan_Control_Pin,GPIO_PIN_SET);*/
	if(g_onFanCallback){
		g_onFanCallback();
	}
}

void BatteryTester_DessipatorControl_offFan(){
/*	HAL_GPIO_WritePin(Fan_Control_GPIO_Port,Fan_Control_Pin,GPIO_PIN_RESET);*/
	if(g_offFanCallback){
		g_offFanCallback();
	}
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

eDessipatorStatus_t BatteryTester_DessipatorControl_getDessipatorStatus(){
	return dessipatorStatus;
}

/*@brief: BatteryTester_DessipatorControl_resetDessipatorStatus
 * @deprecated
 */
void BatteryTester_DessipatorControl_resetDessipatorStatus(){
	dessipatorStatus = DESSIPATOR_OFF;
}

/*@brief: BatteryTester_DessipatorControl_setHeaterControlMin
 * @deprecated
 */
void BatteryTester_DessipatorControl_setHeaterControlMin(float newVal){
	dessipatorControlRange.minVoltageInVolts = newVal;
}

/*@brief: BatteryTester_DessipatorControl_setHeaterControlMax
 * @deprecated
 */
void BatteryTester_DessipatorControl_setHeaterControlMax(float newVal){
	dessipatorControlRange.maxVoltageInVolts = newVal;
}


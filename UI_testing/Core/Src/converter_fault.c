/*
 * converter_fault.c
 *
 *  Created on: May 16, 2024
 *      Author: Tyuryakov_OA
 */
#include "converter_fault.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "climat_regulator.h"
#include "dessipator_control.h"

/*********declaration of variables***********/
static eConverterFaultState_t on = CONVERTER_FAULT_OFF;
static eSoundFaultState_t onSoundFault = FAULT_SOUND_OFF;
static BatteryTester_ConverterFault_isConverterFaultCallback g_isConverterFaultCallback = 0;
static BatteryTester_ConverterFault_hardwareCallback_t g_resetProtectionCallback = 0;
static BatteryTester_ConverterFault_hardwareCallback_t g_soundFaultOnCallback = 0;
static BatteryTester_ConverterFault_hardwareCallback_t g_soundFaultOffCallback = 0;


/**********functional implementation**************/
void BatteryTester_ConverterFault_initDecorator(
		BatteryTester_ConverterFault_isConverterFaultCallback isConverterFaultCallback,
		BatteryTester_ConverterFault_hardwareCallback_t resetProtectionCallback,
		BatteryTester_ConverterFault_hardwareCallback_t soundFaultOnCallback,
		BatteryTester_ConverterFault_hardwareCallback_t soundFaultOffCallback){
	g_isConverterFaultCallback = isConverterFaultCallback;
	g_resetProtectionCallback = resetProtectionCallback;
	g_soundFaultOnCallback = soundFaultOnCallback;
	g_soundFaultOffCallback = soundFaultOffCallback;
}

void BatteryTester_ConverterFault_faultHandler(){
	if(BatteryTester_ConverterFault_isConverterFault()){
		BatteryTester_ClimatRegulator_stopHardware();
		BatteryTester_DessipatorControl_offHeater();
		BatteryTester_DessipatorControl_offFan();
		BatteryTester_RegulatorCellOne_stopHardware();
		BatteryTester_RegulatorCellTwo_stopHardware();
		BatteryTester_ConverterFault_soundFaultOn();
		on = CONVERTER_FAULT_ON;
	}
	else{
		on = CONVERTER_FAULT_OFF;
	}
}

void BatteryTester_ConverterFault_resetFaultHandler(){
	if(g_resetProtectionCallback){
		g_resetProtectionCallback();
		on = CONVERTER_FAULT_OFF;
	}
}

eBool_t BatteryTester_ConverterFault_isConverterFault(){
	if(g_isConverterFaultCallback){
		return g_isConverterFaultCallback();
	}
	return FALSE;
}

void BatteryTester_ConverterFault_soundFaultOn(){
	if(!onSoundFault && g_soundFaultOnCallback){
		g_soundFaultOnCallback();
		onSoundFault = FAULT_SOUND_ON;
	}
}

void BatteryTester_ConverterFault_soundFaultOff(){
	if(onSoundFault && g_soundFaultOffCallback){
		g_soundFaultOffCallback();
		onSoundFault = FAULT_SOUND_OFF;
	}
}

/*
 * regulator_cell_two.c
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */
/*	@brief includes
*/
#include "pid_regulator.h"
#include "regulator_cell_two.h"
#include "conversion_data.h"
#include "flash_operation.h"
#include <stdlib.h>

#define TESTING
/*
 * @brief Private variables
*/
	sPIDController_t regulatorBuckSettingsCellTwo = {0};
	sPIDController_t regulatorBoostSettingsCellTwo = {0};
	sPWMSettings_t pwmBuckSettingsCellTwo = {0};
	sPWMSettings_t pwmBoostSettingsCellTwo = {0};

	static float startSetpoint = 0;
	static eCellTwoRun_t on = CELL_TWO_RUN_OFF;
	static BatteryTester_RegulatorCellTwo_HardwareCallback_t g_startHardware = 0;
	static BatteryTester_RegulatorCellTwo_HardwareCallback_t g_stopHardware = 0;
	static BatteryTester_RegulatorCellTwo_setHardwarePwmPulseCallback_t g_setPulse = 0;
	static BatteryTester_RegulatorCellTwo_isStateCallback_t g_isStartHardware = 0;
/*
 * @brief Extern variables
*/
//	extern sphisicValue_t value;
//	extern TIM_HandleTypeDef htim1;
/*
 * @brief
*/
void BatteryTester_RegulatorCellTwo_initDecorator(
		BatteryTester_RegulatorCellTwo_HardwareCallback_t startHardware,
		BatteryTester_RegulatorCellTwo_HardwareCallback_t stopHardware,
		BatteryTester_RegulatorCellTwo_setHardwarePwmPulseCallback_t setPulse,
		BatteryTester_RegulatorCellTwo_isStateCallback_t isStartHardware){
	g_startHardware = startHardware;
	g_stopHardware = stopHardware;
	g_setPulse = setPulse;
	g_isStartHardware = isStartHardware;
	if(!BatteryTester_RegulatorCellTwo_readDataFromEEPROM()){
	//// BUCK CONVERTER ///////////////
		regulatorBuckSettingsCellTwo.Ki = -0.1;
		regulatorBuckSettingsCellTwo.Kp = 10.0;
		regulatorBuckSettingsCellTwo.dt = 1.0 / 16000;
		regulatorBuckSettingsCellTwo.maxLimit = 18.0;
		regulatorBuckSettingsCellTwo.minLimit = 0.0;

		pwmBuckSettingsCellTwo.maxDutyCycle = 50;
		pwmBuckSettingsCellTwo.minDutyCycle = 10;
		pwmBuckSettingsCellTwo.periodPwm = 499;
		pwmBuckSettingsCellTwo.minPidOutput = regulatorBuckSettingsCellTwo.minLimit;
		pwmBuckSettingsCellTwo.maxPidOutput = regulatorBuckSettingsCellTwo.maxLimit;
		pwmBuckSettingsCellTwo.scale = (float)(pwmBuckSettingsCellTwo.maxDutyCycle - pwmBuckSettingsCellTwo.minDutyCycle) /
				(regulatorBuckSettingsCellTwo.maxLimit - regulatorBuckSettingsCellTwo.minLimit);
	//// BOOST CONVERTER //////////////
		regulatorBoostSettingsCellTwo.Kd = 0.0;
		regulatorBoostSettingsCellTwo.Ki = -0.1;
		regulatorBoostSettingsCellTwo.Kp = 10.0;
		regulatorBoostSettingsCellTwo.dt = 1.0 / 16000;
		regulatorBoostSettingsCellTwo.maxLimit = 0;
		regulatorBoostSettingsCellTwo.minLimit = -18;

		pwmBoostSettingsCellTwo.maxDutyCycle = 90;
		pwmBoostSettingsCellTwo.minDutyCycle = 50;
		pwmBoostSettingsCellTwo.periodPwm = 499;
		pwmBoostSettingsCellTwo.minPidOutput = regulatorBoostSettingsCellTwo.minLimit;
		pwmBoostSettingsCellTwo.maxPidOutput = regulatorBoostSettingsCellTwo.maxLimit;
		pwmBoostSettingsCellTwo.scale = (float)(pwmBoostSettingsCellTwo.maxDutyCycle - pwmBoostSettingsCellTwo.minDutyCycle) /
				(regulatorBoostSettingsCellTwo.maxLimit - regulatorBoostSettingsCellTwo.minLimit);
	}
}

/*
 * @brief extra function
*/
/*inline void BatteryTester_RegulatorCellTwo_onPWMMode1(){
	if(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0){
		htim1.Instance->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	}
}*/

/*
 * @brief extra function
*/
/*inline void BatteryTester_RegulatorCellTwo_onPWMMode2(){
	if(!(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0)){
		htim1.Instance->CCMR1 |= TIM_CCMR1_OC1M_0;
	}
}*/
/*					 ^ DutyCycle
 * 			  ///////| buck
 * 			  boost	 |///////
 * @brief    --------|--------->
 * 		-setpoint    |		setpoint
 * 					 |
 *
 * 	if(setpoint >= 0.0 )
*/
int BatteryTester_RegulatorCellTwo_updateBuck(float setpoint, float feedback){
	return PID_calcOutputToPWM((sPWMSettings_t*) &pwmBuckSettingsCellTwo,
			PID_update((sPIDController_t *) &regulatorBuckSettingsCellTwo,
			setpoint, feedback));
}

/*					 ^ DutyCycle
 * 			  ///////| buck
 * 			  boost	 |///////
 * @brief    --------|--------->
 * 		-setpoint    |		setpoint
 * 					 |
 *
 * 	if(setpoint < 0.0 )
*/
int BatteryTester_RegulatorCellTwo_updateBoost(float setpoint, float feedback){
	return PID_calcOutputToPWM((sPWMSettings_t*) &pwmBoostSettingsCellTwo,
				PID_update((sPIDController_t *) &regulatorBoostSettingsCellTwo,
				setpoint, feedback));
}

sPIDController_t BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings(){
	return regulatorBuckSettingsCellTwo;
}

void BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(sPIDController_t* pSettings){
	if(!pSettings){
		return;
	}
	unsigned int size = sizeof(sPIDController_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&regulatorBuckSettingsCellTwo, size);
}

sPWMSettings_t BattetyTester_RegulatorCellTwo_getBuckPWMSettings(){
	return pwmBuckSettingsCellTwo;
}

void BatteryTester_RegulatorCellTwo_setBuckPWMSettings(sPWMSettings_t* pSettings){
	if(!pSettings){
		return;
	}
	unsigned int size = sizeof(sPWMSettings_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmBuckSettingsCellTwo, size);
}

sPIDController_t BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings(){
	return regulatorBoostSettingsCellTwo;
}

void BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(sPIDController_t* pSettings){
	if(!pSettings){
		return;
	}
	unsigned int size = sizeof(sPIDController_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&regulatorBoostSettingsCellTwo, size);
}

sPWMSettings_t BattetyTester_RegulatorCellTwo_getBoostPWMSettings(){
	return pwmBoostSettingsCellTwo;
}

void BatteryTester_RegulatorCellTwo_setBoostPWMSettings(sPWMSettings_t* pSettings){
	if(!pSettings){
		return;
	}
	unsigned int size = sizeof(sPWMSettings_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmBoostSettingsCellTwo, size);
}

void BatteryTester_RegulatorCellTwo_resetBuckAccumulatedDeviation(){
	PID_resetAccumulatedDeviation((sPIDController_t *) &regulatorBuckSettingsCellTwo);
}

void BatteryTester_RegulatorCellTwo_resetBoostAccumulatedDeviation(){
	PID_resetAccumulatedDeviation((sPIDController_t *) &regulatorBoostSettingsCellTwo);
}

float BatteryTester_RegulatorCellTwo_getSetpoint(){
	return startSetpoint;
}

void BatteryTester_RegulatorCellTwo_setSetpoint(float setpoint){
	if(setpoint < regulatorBuckSettingsCellTwo.maxLimit &&
			setpoint > regulatorBoostSettingsCellTwo.minLimit){
		startSetpoint = setpoint;
	}
}

eCellTwoRun_t BatteryTester_RegulatorCellTwo_getRunStatus(){
	return on;
}

void BatteryTester_RegulatorCellTwo_toggleRunMode(){
	if(on == CELL_TWO_RUN_OFF){
		BatteryTester_RegulatorCellTwo_startHardware();
	}
	else{
		BatteryTester_RegulatorCellTwo_stopHardware();
	}
}

/*@brief:
 *
 */
eBool_t BatteryTester_RegulatorCellTwo_readDataFromEEPROM(){
#ifdef TESTING

#endif
	return BatteryTester_EEPROM_readSetRegCellTwo(
			&regulatorBuckSettingsCellTwo, &regulatorBoostSettingsCellTwo,
			&pwmBuckSettingsCellTwo, &pwmBoostSettingsCellTwo);
}

void BatteryTester_RegulatorCellTwo_startHardware(){
	if(g_startHardware){
		g_startHardware();
		if(BatteryTester_RegulatorCellTwo_isStartHardware())
			on = CELL_TWO_RUN_ON;
	}
}

void BatteryTester_RegulatorCellTwo_stopHardware(){
	if(g_stopHardware){
		g_stopHardware();
		on = CELL_TWO_RUN_OFF;
	}
}

void BatteryTester_RegulatorCellTwo_setPulse(int pulse){
	if(g_setPulse){
		g_setPulse(abs(pulse));
	}
}

eBool_t BatteryTester_RegulatorCellTwo_isStartHardware(void){
	if(g_isStartHardware){
		return g_isStartHardware();
	}
	return FALSE;
}

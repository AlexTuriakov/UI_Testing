/*
 * regulator_cell_one.c
 *
 *  Created on: Apr 15, 2024
 *      Author: Tyuryakov_OA
 */

/*	@brief includes
*/
#include "pid_regulator.h"
#include "regulator_cell_one.h"
#include "conversion_data.h"
#include "auxiliary_function.h"

/*
 * @brief Private variables
*/
	sPIDController_t regulatorBuckSettingsCellOne = {0};
	sPIDController_t regulatorBoostSettingsCellOne = {0};
	sPWMSettings_t pwmBuckSettingsCellOne = {0};
	sPWMSettings_t pwmBoostSettingsCellOne = {0};
/*
 * @brief Extern variables
*/
	extern sphisicValue_t value;
//	extern TIM_HandleTypeDef htim1;
/*
 * @brief
*/
void BatteryTester_RegulatorCellOne_init(){
//// BUCK CONVERTER ///////////////
	regulatorBuckSettingsCellOne.Kd = 0.0;
	regulatorBuckSettingsCellOne.Ki = -0.1;
	regulatorBuckSettingsCellOne.Kp = 10.0;
	regulatorBuckSettingsCellOne.dt = 1.0 / 16000;
	regulatorBuckSettingsCellOne.maxLimit = 40.0;
	regulatorBuckSettingsCellOne.minLimit = -40.0;

	pwmBuckSettingsCellOne.maxDutyCycle = 50;
	pwmBuckSettingsCellOne.minDutyCycle = 10;
	pwmBuckSettingsCellOne.periodPwm = 500;
	pwmBuckSettingsCellOne.minPidOutput = regulatorBuckSettingsCellOne.minLimit;
	pwmBuckSettingsCellOne.maxPidOutput = regulatorBuckSettingsCellOne.maxLimit;
	pwmBuckSettingsCellOne.scale = (float)(pwmBuckSettingsCellOne.maxDutyCycle - pwmBuckSettingsCellOne.minDutyCycle) /
			(regulatorBuckSettingsCellOne.maxLimit - regulatorBuckSettingsCellOne.minLimit);
//// BOOST CONVERTER //////////////
	regulatorBoostSettingsCellOne.Kd = 0.0;
	regulatorBoostSettingsCellOne.Ki = -0.1;
	regulatorBoostSettingsCellOne.Kp = 10.0;
	regulatorBoostSettingsCellOne.dt = 1.0 / 16000;
	regulatorBoostSettingsCellOne.maxLimit = 30.0;
	regulatorBoostSettingsCellOne.minLimit = -30.0;

	pwmBoostSettingsCellOne.maxDutyCycle = 90;
	pwmBoostSettingsCellOne.minDutyCycle = 50;
	pwmBoostSettingsCellOne.periodPwm = 500;
	pwmBoostSettingsCellOne.minPidOutput = regulatorBoostSettingsCellOne.minLimit;
	pwmBoostSettingsCellOne.maxPidOutput = regulatorBoostSettingsCellOne.maxLimit;
	pwmBoostSettingsCellOne.scale = (float)(pwmBoostSettingsCellOne.maxDutyCycle - pwmBoostSettingsCellOne.minDutyCycle) /
			(regulatorBoostSettingsCellOne.maxLimit - regulatorBoostSettingsCellOne.minLimit);

}

/*
 * @brief extra function
*/
/*inline void BatteryTester_RegulatorCellOne_onPWMMode1(){
	if(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0){
		htim1.Instance->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	}
}*/

/*
 * @brief extra function
*/
/*inline void BatteryTester_RegulatorCellOne_onPWMMode2(){
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
unsigned int BatteryTester_RegulatorCellOne_updateBuck(float setpoint, float feedback){
	return PID_calcOutputToPWM((sPWMSettings_t*) &pwmBuckSettingsCellOne,
			PID_update((sPIDController_t *) &regulatorBuckSettingsCellOne,
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
unsigned int BatteryTester_RegulatorCellOne_updateBoost(float setpoint, float feedback){
	return PID_calcOutputToPWM((sPWMSettings_t*) &pwmBoostSettingsCellOne,
				PID_update((sPIDController_t *) &regulatorBoostSettingsCellOne,
				setpoint, feedback));
}

sPIDController_t BatteryTester_RegulatorCellOne_getBuckRegulatorSettings(){
	return regulatorBuckSettingsCellOne;
}

void BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(sPIDController_t* pSettings){
	if(!pSettings){
		return;
	}
	if(regulatorBuckSettingsCellOne.minLimit != pSettings->minLimit){
		pwmBuckSettingsCellOne.minPidOutput = pSettings->minLimit;
	}
	if(regulatorBuckSettingsCellOne.maxLimit != pSettings->maxLimit){
		pwmBuckSettingsCellOne.maxPidOutput = pSettings->maxLimit;
	}
	unsigned int size = sizeof(sPIDController_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&regulatorBuckSettingsCellOne, size);
}

sPWMSettings_t BattetyTester_RegulatorCellOne_getBuckPWMSettings(){
	return pwmBuckSettingsCellOne;
}

void BatteryTester_RegulatorCellOne_setBuckPWMSettings(sPWMSettings_t* pSettings){
	if(!pSettings){
		return;
	}
	unsigned int size = sizeof(sPWMSettings_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmBuckSettingsCellOne, size);
}

sPIDController_t BatteryTester_RegulatorCellOne_getBoostRegulatorSettings(){
	return regulatorBoostSettingsCellOne;
}

void BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(sPIDController_t* pSettings){
	if(!pSettings){
		return;
	}
	if(regulatorBoostSettingsCellOne.minLimit != pSettings->minLimit){
		pwmBoostSettingsCellOne.minPidOutput = pSettings->minLimit;
	}
	if(regulatorBoostSettingsCellOne.maxLimit != pSettings->maxLimit){
		pwmBoostSettingsCellOne.maxPidOutput = pSettings->maxLimit;
	}
	unsigned int size = sizeof(sPWMSettings_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&regulatorBoostSettingsCellOne, size);
}

sPWMSettings_t BattetyTester_RegulatorCellOne_getBoostPWMSettings(){
	return pwmBoostSettingsCellOne;
}

void BatteryTester_RegulatorCellOne_setBoostPWMSettings(sPWMSettings_t* pSettings){
	if(!pSettings){
		return;
	}
	unsigned int size = sizeof(sPWMSettings_t) / sizeof(float);
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmBoostSettingsCellOne, size);
}

void BatteryTester_RegulatorCellOne_resetBuckAccumulatedDeviation(){
	PID_resetAccumulatedDeviation((sPIDController_t *) &regulatorBuckSettingsCellOne);
}

void BatteryTester_RegulatorCellOne_resetBoostAccumulatedDeviation(){
	PID_resetAccumulatedDeviation((sPIDController_t *) &regulatorBoostSettingsCellOne);
}


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
#include "auxiliary_function.h"

/*
 * @brief Private variables
*/
	sPIDController_t regulatorBuckSettingsCellTwo = {0};
	sPIDController_t regulatorBoostSettingsCellTwo = {0};
	sPWMSettings_t pwmBuckSettingsCellTwo = {0};
	sPWMSettings_t pwmBoostSettingsCellTwo = {0};
/*
 * @brief Extern variables
*/
//	extern sphisicValue_t value;
//	extern TIM_HandleTypeDef htim1;
/*
 * @brief
*/
void BatteryTester_RegulatorCellTwo_init(){
//// BUCK CONVERTER ///////////////
	regulatorBuckSettingsCellTwo.Kd = 0.0;
	regulatorBuckSettingsCellTwo.Ki = -0.1;
	regulatorBuckSettingsCellTwo.Kp = 10.0;
	regulatorBuckSettingsCellTwo.dt = 1.0 / 16000;
	regulatorBuckSettingsCellTwo.maxLimit = 40.0;
	regulatorBuckSettingsCellTwo.minLimit = -40.0;

	pwmBuckSettingsCellTwo.maxDutyCycle = 50;
	pwmBuckSettingsCellTwo.minDutyCycle = 10;
	pwmBuckSettingsCellTwo.periodPwm = 500;
	pwmBuckSettingsCellTwo.minPidOutput = regulatorBuckSettingsCellTwo.minLimit;
	pwmBuckSettingsCellTwo.maxPidOutput = regulatorBuckSettingsCellTwo.maxLimit;
	pwmBuckSettingsCellTwo.scale = (float)(pwmBuckSettingsCellTwo.maxDutyCycle - pwmBuckSettingsCellTwo.minDutyCycle) /
			(regulatorBuckSettingsCellTwo.maxLimit - regulatorBuckSettingsCellTwo.minLimit);
//// BOOST CONVERTER //////////////
	regulatorBoostSettingsCellTwo.Kd = 0.0;
	regulatorBoostSettingsCellTwo.Ki = -0.1;
	regulatorBoostSettingsCellTwo.Kp = 10.0;
	regulatorBoostSettingsCellTwo.dt = 1.0 / 16000;
	regulatorBoostSettingsCellTwo.maxLimit = 30.0;
	regulatorBoostSettingsCellTwo.minLimit = -30.0;

	pwmBoostSettingsCellTwo.maxDutyCycle = 90;
	pwmBoostSettingsCellTwo.minDutyCycle = 50;
	pwmBoostSettingsCellTwo.periodPwm = 500;
	pwmBoostSettingsCellTwo.minPidOutput = regulatorBoostSettingsCellTwo.minLimit;
	pwmBoostSettingsCellTwo.maxPidOutput = regulatorBoostSettingsCellTwo.maxLimit;
	pwmBoostSettingsCellTwo.scale = (float)(pwmBoostSettingsCellTwo.maxDutyCycle - pwmBoostSettingsCellTwo.minDutyCycle) /
			(regulatorBoostSettingsCellTwo.maxLimit - regulatorBoostSettingsCellTwo.minLimit);

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
unsigned int BatteryTester_RegulatorCellTwo_updateBuck(float setpoint, float feedback){
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
unsigned int BatteryTester_RegulatorCellTwo_updateBoost(float setpoint, float feedback){
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
	unsigned int size = sizeof(sPWMSettings_t) / sizeof(float);
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


/*
 * climat_regulator.c
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */

#include "climat_regulator.h"
#include "pid_regulator.h"
//#include "conversion_data.h"
#include "auxiliary_function.h"

/*
 * @brief Private variables
*/
	sPIDController_t regulatorClimatSettings = {0};
	sPWMSettings_t pwmClimatSettings = {0};
	float hysteresis = 1.0;
/*
 * @brief Extern variables
*/
	//extern sphisicValue_t value;
	extern TIM_HandleTypeDef htim1;
//	extern TIM_HandleTypeDef htim1;



void BatteryTester_ClimatRegulator_init(){
	//// CLIMAT CONVERTER ///////////////
		regulatorClimatSettings.Kd = 0.0;
		regulatorClimatSettings.Ki = -0.1;
		regulatorClimatSettings.Kp = 10.0;
		regulatorClimatSettings.dt = 1.0 / 16000;
		regulatorClimatSettings.maxLimit = 40.0;
		regulatorClimatSettings.minLimit = -40.0;

		pwmClimatSettings.maxDutyCycle = 90;
		pwmClimatSettings.minDutyCycle = 0;
		pwmClimatSettings.periodPwm = 500;
		pwmClimatSettings.minPidOutput = regulatorClimatSettings.minLimit;
		pwmClimatSettings.maxPidOutput = regulatorClimatSettings.maxLimit;
		pwmClimatSettings.scale = (float)(pwmClimatSettings.maxDutyCycle - pwmClimatSettings.minDutyCycle) /
				(regulatorClimatSettings.maxLimit - regulatorClimatSettings.minLimit);
}

inline void BatteryTester_ClimatRegulator_onPWMModeHeat(){
	if(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0){
		htim1.Instance->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	}
	if(!(htim1.Instance->CCMR1 & TIM_CCMR1_OC2M_0)){
		htim1.Instance->CCMR1 |= TIM_CCMR1_OC2M_0;
	}
}

inline void BatteryTester_ClimatRegulator_onPWMModeCool(){

	if(!(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0)){
		htim1.Instance->CCMR1 |= TIM_CCMR1_OC1M_0;
	}
	if(htim1.Instance->CCMR1 & TIM_CCMR1_OC2M_0){
		htim1.Instance->CCMR1 &= ~TIM_CCMR1_OC2M_0;
	}
}

inline void BatteryTester_ClimatRegulator_stopTimerCounter(){
	htim1.Instance->CR1 &= ~TIM_CR1_CEN;
}

inline void BatteryTester_ClimatRegulator_startTimerCounter(){
	htim1.Instance->CR1 |= TIM_CR1_CEN;
}

inline void BatteryTester_ClimatRegulator_stopPhAHigh(){
	if(htim1.Instance->CCER & TIM_CCER_CC1E){	//може це зайве
		htim1.Instance->CCER &= ~TIM_CCER_CC1E;
	}
}

inline void BatteryTester_ClimatRegulator_startPhAHigh(){
	if(!(htim1.Instance->CCER & TIM_CCER_CC1E)){ //може це зайве
		htim1.Instance->CCER |= TIM_CCER_CC1E;
	}
}

inline void BatteryTester_ClimatRegulator_stopPhBHigh(){
	if(htim1.Instance->CCER & TIM_CCER_CC2E){
		htim1.Instance->CCER &= ~TIM_CCER_CC2E;
	}
}

inline void BatteryTester_ClimatRegulator_startPhBHigh(){
	if(!(htim1.Instance->CCER & TIM_CCER_CC2E)){
		htim1.Instance->CCER |= TIM_CCER_CC2E;
	}
}

unsigned int BatteryTester_ClimatRegulator_update(float setpoint, float feedback){
	BatteryTester_ClimatRegulator_selectPWMMode(setpoint, feedback);

	return PID_calcOutputToPWM((sPWMSettings_t*) &pwmClimatSettings,
					PID_update((sPIDController_t *) &regulatorClimatSettings,
					setpoint, feedback));
}

sPIDController_t BatteryTester_ClimatRegulator_getRegulatorSettings(){
	return regulatorClimatSettings;
}

void BatteryTester_ClimatRegulator_setRegulatorSettings(sPIDController_t* pSettings){
	if(!pSettings){
		return;
	}
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&regulatorClimatSettings,
			sizeof(sPIDController_t) / sizeof(float));
}

sPWMSettings_t BattetyTester_ClimatRegulator_getPWMSettings(){
	return pwmClimatSettings;
}

void BatteryTester_ClimatRegulator_setPWMSettings(sPWMSettings_t* pSettings){
	if(!pSettings){
		return;
	}
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmClimatSettings,
			sizeof(sPIDController_t) / sizeof(float));
}

inline void BatteryTester_ClimatRegulator_selectPWMMode(float setpoint, float feedback){
	if(setpoint - feedback < -hysteresis){
		// cool
//		BatteryTester_ClimatRegulator_stopTimerCounter();
		BatteryTester_ClimatRegulator_onPWMModeCool();
		BatteryTester_ClimatRegulator_startPhAHigh();
		BatteryTester_ClimatRegulator_stopPhBHigh();
//		BatteryTester_ClimatRegulator_startTimerCounter();
	}
	else if(setpoint - feedback > hysteresis){
		// heat
//		BatteryTester_ClimatRegulator_stopTimerCounter();
		BatteryTester_ClimatRegulator_onPWMModeHeat();
		BatteryTester_ClimatRegulator_startPhBHigh();
		BatteryTester_ClimatRegulator_stopPhAHigh();
//		BatteryTester_ClimatRegulator_startTimerCounter();
	}
}

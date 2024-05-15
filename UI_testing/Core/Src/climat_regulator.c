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

#define TESTING
//#define BatteryTester_ClimatRegulator_setPWMSettings BatteryTester_ClimatRegulator__setPWMSettings

/*
 * @brief: typedef
 * @deprecated
 */
typedef void (*BatteryTester_ClimatRegulator_setPWMSettingsNew_t)(sPWMSettings_t* pSettings);
/*
 * @brief Private variables
*/
static sPIDController_t regulatorClimatSettings = {0};
static sPWMSettings_t pwmClimatSettings = {0};
static float startSetpoint = 0.0;
static eThermostatRun_t on = THERMOSTAT_RUN_OFF;
static BatteryTester_ClimatRegulator_HardwareCallback_t g_startHardware = 0;
static BatteryTester_ClimatRegulator_HardwareCallback_t g_stopHardware = 0;
static BatteryTester_ClimatRegulator_setHardwarePwmPulseCallback_t g_setPulse = 0;
static BatteryTester_ClimatRegulator_setPWMSettingsNew_t
	replaceFunction = BatteryTester_ClimatRegulator__setPWMSettings;
/*
 * @brief: BatteryTester_ClimatRegulator_callbackChangeSettingsPwm_t
 * @deprecated
 */
static BatteryTester_ClimatRegulator_callbackChangeSettingsPwm_t callbackSetPeriod = 0;
/*
 * @brief: BatteryTester_ClimatRegulator_callbackChangeSettingsPwm_t
 * @deprecated
 */
static BatteryTester_ClimatRegulator_callbackChangeSettingsPwm_t callbackSetDeadTime = 0;
/*
 * @brief: hysteresis
 * @deprecated
 */
float hysteresis = 1.0;
/*
 * @brief Extern variables
*/
	//extern sphisicValue_t value;
	/*
	 * @brief: htim1
	 * @deprecated
	 */
	extern TIM_HandleTypeDef htim1;
//	extern TIM_HandleTypeDef htim1;



/*@brief:
 * @deprecated:
 * Climate control module initialization function.
 * Call in main().
 * In addition to it, you need to call 2 more functions:
 * 			BatteryTester_ClimatRegulator_setCallbackChangePeriodPwm
 * 			BatteryTester_ClimatRegulator_setCallbackChangeDeadTimePwm
 * To set callbacks for hardware change of period
 * and dead time of PWM complementary pair.
 * These callbacks must first be implemented
 * in the main program. They must implement
 * the hardware logic for setting the appropriate values.
 */
/*void BatteryTester_ClimatRegulator_init(){
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
		BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
		implement reading parameters from flash memory
}*/

void BatteryTester_ClimatRegulator_initDecorator(
		BatteryTester_ClimatRegulator_HardwareCallback_t startHardware,
		BatteryTester_ClimatRegulator_HardwareCallback_t stopHardware,
		BatteryTester_ClimatRegulator_setHardwarePwmPulseCallback_t setPulse){
	g_startHardware = startHardware;
	g_stopHardware = stopHardware;
	g_setPulse = setPulse;
	if(BatteryTester_ClimatRegulator_readDataFromEEPROM() != HAL_OK){
		regulatorClimatSettings.Kd = 0.0;
		regulatorClimatSettings.Ki = -0.1;
		regulatorClimatSettings.Kp = 10.0;
		regulatorClimatSettings.dt = 1.0 / 16000;
		regulatorClimatSettings.maxLimit = 100.0;
		regulatorClimatSettings.minLimit = -30.0;

		pwmClimatSettings.maxDutyCycle = 95;
		pwmClimatSettings.minDutyCycle = 5;
		pwmClimatSettings.periodPwm = 499;
		pwmClimatSettings.minPidOutput = regulatorClimatSettings.minLimit;
		pwmClimatSettings.maxPidOutput = regulatorClimatSettings.maxLimit;
		BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
	}
//// CLIMAT CONVERTER ///////////////
}

inline void BatteryTester_ClimatRegulator_calcScalePwmClimatSettings(){
	pwmClimatSettings.scale = (float)(pwmClimatSettings.maxDutyCycle - pwmClimatSettings.minDutyCycle) /
					(regulatorClimatSettings.maxLimit - regulatorClimatSettings.minLimit);
}


unsigned int BatteryTester_ClimatRegulator_update(float setpoint, float feedback){
//	BatteryTester_ClimatRegulator_selectPWMMode(setpoint, feedback);
	return PID_calcOutputToPWM((sPWMSettings_t*) &pwmClimatSettings,
				PID_updateDelta((sPIDController_t *) &regulatorClimatSettings,
				setpoint, feedback));
}

sPIDController_t BatteryTester_ClimatRegulator_getRegulatorSettings(){
	return regulatorClimatSettings;
}

void BatteryTester_ClimatRegulator_setRegulatorSettings(sPIDController_t* pSettings){
	if(!pSettings){
		return;
	}
	if(regulatorClimatSettings.minLimit != pSettings->minLimit){
		pwmClimatSettings.minPidOutput = pSettings->minLimit;
	}
	if(regulatorClimatSettings.maxLimit != pSettings->maxLimit){
		pwmClimatSettings.maxPidOutput = pSettings->maxLimit;
	}
	BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&regulatorClimatSettings,
			sizeof(sPIDController_t) / sizeof(float));
}

sPWMSettings_t BattetyTester_ClimatRegulator_getPWMSettings(){
	return pwmClimatSettings;
}


void BatteryTester_ClimatRegulator__setPWMSettings(sPWMSettings_t* pSettings){
	if(!pSettings){
		return;
	}
	BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmClimatSettings,
			sizeof(sPIDController_t) / sizeof(float));
}

float BatteryTester_ClimatRegulator_getSetpoint(){
	return startSetpoint;
}

void BatteryTester_ClimatRegulator_setSetpoint(float setpoint){
	if(setpoint < regulatorClimatSettings.maxLimit &&
			setpoint > regulatorClimatSettings.minLimit){
		startSetpoint = setpoint;
	}
}

eThermostatRun_t BatteryTester_ClimatRegulator_getRunStatus(){
	return on;
}

void BatteryTester_ClimatRegulator_toggleRunMode(){
	on ^= THERMOSTAT_RUN_ON;
	if(on == THERMOSTAT_RUN_ON){
		BatteryTester_ClimatRegulator_startHardware();
	}
	else{
		BatteryTester_ClimatRegulator_stopHardware();
	}
}

/*@brief:
 * @Todo: Implementation required
 */
HAL_StatusTypeDef BatteryTester_ClimatRegulator_readDataFromEEPROM(){
#ifdef TESTING
	regulatorClimatSettings.Kd = 0.0;
	regulatorClimatSettings.Ki = -0.1;
	regulatorClimatSettings.Kp = 10.0;
	regulatorClimatSettings.dt = 1.0 / 16000;
	regulatorClimatSettings.maxLimit = 100.0;
	regulatorClimatSettings.minLimit = -30.0;

	pwmClimatSettings.maxDutyCycle = 95;
	pwmClimatSettings.minDutyCycle = 5;
	pwmClimatSettings.periodPwm = 499;
	pwmClimatSettings.minPidOutput = regulatorClimatSettings.minLimit;
	pwmClimatSettings.maxPidOutput = regulatorClimatSettings.maxLimit;
	BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
#endif
	return HAL_OK;
}

void BatteryTester_ClimatRegulator_startHardware(){
	if(g_startHardware){
		g_startHardware();
	}
}

void BatteryTester_ClimatRegulator_stopHardware(){
	if(g_stopHardware){
		g_stopHardware();
	}
}

void BatteryTester_ClimatRegulator_setPulse(int pulse){
	if(g_setPulse){
		g_setPulse(pulse);
	}
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_onPWMModeHeat(){
	if(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0){
		htim1.Instance->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	}
	if(!(htim1.Instance->CCMR1 & TIM_CCMR1_OC2M_0)){
		htim1.Instance->CCMR1 |= TIM_CCMR1_OC2M_0;
	}
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_onPWMModeCool(){

	if(!(htim1.Instance->CCMR1 & TIM_CCMR1_OC1M_0)){
		htim1.Instance->CCMR1 |= TIM_CCMR1_OC1M_0;
	}
	if(htim1.Instance->CCMR1 & TIM_CCMR1_OC2M_0){
		htim1.Instance->CCMR1 &= ~TIM_CCMR1_OC2M_0;
	}
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_stopTimerCounter(){
	htim1.Instance->CR1 &= ~TIM_CR1_CEN;
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_startTimerCounter(){
	htim1.Instance->CR1 |= TIM_CR1_CEN;
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_stopPhAHigh(){
	if(htim1.Instance->CCER & TIM_CCER_CC1E){
		htim1.Instance->CCER &= ~TIM_CCER_CC1E;
	}
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_startPhAHigh(){
	if(!(htim1.Instance->CCER & TIM_CCER_CC1E)){
		htim1.Instance->CCER |= TIM_CCER_CC1E;
	}
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_stopPhBHigh(){
	if(htim1.Instance->CCER & TIM_CCER_CC2E){
		htim1.Instance->CCER &= ~TIM_CCER_CC2E;
	}
}

/*
 * @brief:
 * @deprecated
 */
inline void BatteryTester_ClimatRegulator_startPhBHigh(){
	if(!(htim1.Instance->CCER & TIM_CCER_CC2E)){
		htim1.Instance->CCER |= TIM_CCER_CC2E;
	}
}

/*
 * @brief: BatteryTester_ClimatRegulator_setPWMSettings
 * @deprecated
 */
void BatteryTester_ClimatRegulator_setPWMSettings(sPWMSettings_t* pSettings){
	if(replaceFunction){
		replaceFunction(pSettings);
		return;
	}

	/*
	 * @brief: this part is deprecated
	 */
	if(!pSettings){
		return;
	}
	if(callbackSetPeriod && pwmClimatSettings.periodPwm != pSettings->periodPwm){
		callbackSetPeriod((unsigned int)pSettings->periodPwm);
	}
	else{
		pSettings->periodPwm = pwmClimatSettings.periodPwm;
	}
	if(callbackSetDeadTime && pwmClimatSettings.deadTime != pSettings->deadTime){
		callbackSetDeadTime((unsigned int)pSettings->deadTime);
	}
	else{
		pSettings->deadTime = pwmClimatSettings.deadTime;
	}
	BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
	__BatteryTester_AuxiliaryFunction_copy(
			(void*)pSettings, (void*)&pwmClimatSettings,
			sizeof(sPIDController_t) / sizeof(float));
}

/*
 * @brief: BatteryTester_ClimatRegulator_setCallbackChangePeriodPwm
 * @deprecated
 */
void BatteryTester_ClimatRegulator_setCallbackChangePeriodPwm(
		void (*callback)(unsigned int newValue)){
	callbackSetPeriod = callback;
}

/*
 * @brief: BatteryTester_ClimatRegulator_setCallbackChangeDeadTimePwm
 * @deprecated
 */
void BatteryTester_ClimatRegulator_setCallbackChangeDeadTimePwm(
		void (*callback)(unsigned int newValue)){
	callbackSetDeadTime = callback;
}

/*
 * @brief: BatteryTester_ClimatRegulator_selectPWMMode
 * @deprecated
 */
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


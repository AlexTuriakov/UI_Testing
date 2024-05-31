/*
 * climat_regulator.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CLIMAT_REGULATOR_H_
#define INC_CLIMAT_REGULATOR_H_

#include "pid_regulator.h"
#include "auxiliary_function.h"
/**************type declaration****************/
/*
 * @brief:
 * @deprecated
 */
typedef void (*BatteryTester_ClimatRegulator_callbackChangeSettingsPwm_t)(unsigned int newValue);
typedef enum{
	THERMOSTAT_RUN_OFF,
	THERMOSTAT_RUN_ON
} eThermostatRun_t;
typedef void (*BatteryTester_ClimatRegulator_HardwareCallback_t)(void);
typedef void (*BatteryTester_ClimatRegulator_setHardwarePwmPulseCallback_t)(int pulse);


/****************Function declaration****************/
void BatteryTester_ClimatRegulator_initDecorator(
		BatteryTester_ClimatRegulator_HardwareCallback_t,
		BatteryTester_ClimatRegulator_HardwareCallback_t,
		BatteryTester_ClimatRegulator_setHardwarePwmPulseCallback_t);
void BatteryTester_ClimatRegulator_onPWMModeHeat();
void BatteryTester_ClimatRegulator_onPWMModeCool();
void BatteryTester_ClimatRegulator_selectPWMMode(float, float);
unsigned int BatteryTester_ClimatRegulator_update(float, float);
sPIDController_t BatteryTester_ClimatRegulator_getRegulatorSettings();
void BatteryTester_ClimatRegulator_setRegulatorSettings(sPIDController_t*);
sPWMSettings_t BattetyTester_ClimatRegulator_getPWMSettings();
void BatteryTester_ClimatRegulator_setPWMSettings(sPWMSettings_t*);
void BatteryTester_ClimatRegulator_stopTimerCounter();
void BatteryTester_ClimatRegulator_startTimerCounter();
void BatteryTester_ClimatRegulator_stopPhAHigh();
void BatteryTester_ClimatRegulator_startPhAHigh();
void BatteryTester_ClimatRegulator_stopPhBHigh();
void BatteryTester_ClimatRegulator_startPhBHigh();
void BatteryTester_ClimatRegulator_calcScalePwmClimatSettings();
void BatteryTester_ClimatRegulator_setCallbackChangePeriodPwm(void (*callback)(unsigned int newValue));
void BatteryTester_ClimatRegulator_setCallbackChangeDeadTimePwm(void (*callback)(unsigned int newValue));
void BatteryTester_ClimatRegulator__setPWMSettings(sPWMSettings_t*);
float BatteryTester_ClimatRegulator_getSetpoint();
void BatteryTester_ClimatRegulator_setSetpoint(float);
eThermostatRun_t BatteryTester_ClimatRegulator_getRunStatus();
void BatteryTester_ClimatRegulator_toggleRunMode();
eBool_t BatteryTester_ClimatRegulator_readDataFromEEPROM();
void BatteryTester_ClimatRegulator_startHardware();
void BatteryTester_ClimatRegulator_stopHardware();
void BatteryTester_ClimatRegulator_setPulse(int);

#endif /* INC_CLIMAT_REGULATOR_H_ */

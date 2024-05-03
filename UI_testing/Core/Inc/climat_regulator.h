/*
 * climat_regulator.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CLIMAT_REGULATOR_H_
#define INC_CLIMAT_REGULATOR_H_

#include "pid_regulator.h"
typedef void (*BatteryTester_ClimatRegulator_callbackChangeSettingsPwm_t)(unsigned int newValue);

void BatteryTester_ClimatRegulator_init();
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
#endif /* INC_CLIMAT_REGULATOR_H_ */

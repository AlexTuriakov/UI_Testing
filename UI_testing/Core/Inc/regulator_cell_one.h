/*
 * regulator_cell_one.h
 *
 *  Created on: Apr 15, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_REGULATOR_CELL_ONE_H_
#define INC_REGULATOR_CELL_ONE_H_

#include "pid_regulator.h"

/**************type declaration****************/
typedef enum{
	CELL_ONE_RUN_OFF,
	CELL_ONE_RUN_ON
} eCellOneRun_t;
typedef void (*BatteryTester_RegulatorCellOne_HardwareCallback_t)(void);
typedef void (*BatteryTester_RegulatorCellOne_setHardwarePwmPulseCallback_t)(unsigned int pulse);


/****************Function declaration****************/
void BatteryTester_RegulatorCellOne_initDecorator(
		BatteryTester_RegulatorCellOne_HardwareCallback_t startHardware,
		BatteryTester_RegulatorCellOne_HardwareCallback_t stopHardware,
		BatteryTester_RegulatorCellOne_setHardwarePwmPulseCallback_t setPulse);
/*void BatteryTester_RegulatorCellOne_onPWMMode1();
void BatteryTester_RegulatorCellOne_onPWMMode2();*/
unsigned int BatteryTester_RegulatorCellOne_updateBuck(float, float);
unsigned int BatteryTester_RegulatorCellOne_updateBoost(float, float);
sPIDController_t BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
void BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(sPIDController_t*);
sPWMSettings_t BattetyTester_RegulatorCellOne_getBuckPWMSettings();
void BatteryTester_RegulatorCellOne_setBuckPWMSettings(sPWMSettings_t*);
sPIDController_t BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
void BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(sPIDController_t*);
sPWMSettings_t BattetyTester_RegulatorCellOne_getBoostPWMSettings();
void BatteryTester_RegulatorCellOne_setBoostPWMSettings(sPWMSettings_t*);
void BatteryTester_RegulatorCellOne_resetBuckAccumulatedDeviation();
void BatteryTester_RegulatorCellOne_resetBoostAccumulatedDeviation();
eCellOneRun_t BatteryTester_RegulatorCellOne_getRunStatus();
void BatteryTester_RegulatorCellOne_toggleRunMode();
HAL_StatusTypeDef BatteryTester_RegulatorCellOne_readDataFromEEPROM();
void BatteryTester_RegulatorCellOne_startHardware();
void BatteryTester_RegulatorCellOne_stopHardware();
void BatteryTester_RegulatorCellOne_setPulse(int);
void BatteryTester_RegulatorCellOne_setSetpoint(float);
float BatteryTester_RegulatorCellOne_getSetpoint();

#endif /* INC_REGULATOR_CELL_ONE_H_ */

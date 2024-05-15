/*
 * regulator_cell_two.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_REGULATOR_CELL_TWO_H_
#define INC_REGULATOR_CELL_TWO_H_

#include "pid_regulator.h"

/**************type declaration****************/
typedef enum{
	CELL_TWO_RUN_OFF,
	CELL_TWO_RUN_ON
} eCellTwoRun_t;
typedef void (*BatteryTester_RegulatorCellTwo_HardwareCallback_t)(void);
typedef void (*BatteryTester_RegulatorCellTwo_setHardwarePwmPulseCallback_t)(unsigned int pulse);


/****************Function declaration****************/
void BatteryTester_RegulatorCellTwo_initDecorator(
		BatteryTester_RegulatorCellTwo_HardwareCallback_t,
		BatteryTester_RegulatorCellTwo_HardwareCallback_t,
		BatteryTester_RegulatorCellTwo_setHardwarePwmPulseCallback_t);
unsigned int BatteryTester_RegulatorCellTwo_updateBuck(float, float);
unsigned int BatteryTester_RegulatorCellTwo_updateBoost(float, float);
sPIDController_t BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
void BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(sPIDController_t*);
sPWMSettings_t BattetyTester_RegulatorCellTwo_getBuckPWMSettings();
void BatteryTester_RegulatorCellTwo_setBuckPWMSettings(sPWMSettings_t*);
sPIDController_t BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
void BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(sPIDController_t*);
sPWMSettings_t BattetyTester_RegulatorCellTwo_getBoostPWMSettings();
void BatteryTester_RegulatorCellTwo_setBoostPWMSettings(sPWMSettings_t*);
void BatteryTester_RegulatorCellTwo_resetBuckAccumulatedDeviation();
void BatteryTester_RegulatorCellTwo_resetBoostAccumulatedDeviation();
float BatteryTester_RegulatorCellTwo_getSetpoint();
void BatteryTester_RegulatorCellTwo_setSetpoint(float);
eCellTwoRun_t BatteryTester_RegulatorCellTwo_getRunStatus();
void BatteryTester_RegulatorCellTwo_toggleRunMode();
HAL_StatusTypeDef BatteryTester_RegulatorCellTwo_readDataFromEEPROM();
void BatteryTester_RegulatorCellTwo_startHardware();
void BatteryTester_RegulatorCellTwo_stopHardware();
void BatteryTester_RegulatorCellTwo_setPulse(int);

#endif /* INC_REGULATOR_CELL_TWO_H_ */

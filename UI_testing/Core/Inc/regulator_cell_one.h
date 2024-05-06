/*
 * regulator_cell_one.h
 *
 *  Created on: Apr 15, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_REGULATOR_CELL_ONE_H_
#define INC_REGULATOR_CELL_ONE_H_

#include "pid_regulator.h"

void BatteryTester_RegulatorCellOne_init();

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

#endif /* INC_REGULATOR_CELL_ONE_H_ */

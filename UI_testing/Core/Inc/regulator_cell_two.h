/*
 * regulator_cell_two.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_REGULATOR_CELL_TWO_H_
#define INC_REGULATOR_CELL_TWO_H_

#include "pid_regulator.h"

void BatteryTester_RegulatorCellTwo_init();

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

#endif /* INC_REGULATOR_CELL_TWO_H_ */

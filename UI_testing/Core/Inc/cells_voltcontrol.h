/*
 * cells_voltcontrol.h
 *
 *  Created on: Apr 17, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CELLS_VOLTCONTROL_H_
#define INC_CELLS_VOLTCONTROL_H_

#include "auxiliary_function.h"

typedef enum { CELL_ONE = 0,
	   	       CELL_TWO = 1 } BatteryCellSelector_t;

typedef enum {VOLTAGE_PROTECT_CELL_OFF = 0,
	VOLTAGE_PROTECT_CELL_ON = 1} BatteryCellVoltageStatusProtect_t;

BatteryCellVoltageStatusProtect_t BatteryTester_CellsVoltcontrol_protectVoltageCellx(
			BatteryCellSelector_t, float);
void BatteryTester_CellsVoltcontrol_resetProtectVoltageCellx(
		BatteryCellSelector_t cell);
BatteryCellVoltageStatusProtect_t BatteryTester_CellsVoltcontrol_getStatusProtectVoltageCellx(
		BatteryCellSelector_t);
sVoltRange_t BatteryTester_CellsVoltcontrol_getVoltRangeCellx(
		BatteryCellSelector_t);
void BatteryTester_CellsVoltcontrol_setVoltRangeCellx(
		BatteryCellSelector_t, sVoltRange_t*);
void BatteryTester_CellsVoltcontrol_initVoltageProtectCells();
void BatteryTester_CellsVoltcontrol_stopPWMCh1();
void BatteryTester_CellsVoltcontrol_stopPWMCh2();


#endif /* INC_CELLS_VOLTCONTROL_H_ */

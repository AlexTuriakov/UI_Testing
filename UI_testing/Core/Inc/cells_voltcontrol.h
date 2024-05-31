/*
 * cells_voltcontrol.h
 *
 *  Created on: Apr 17, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CELLS_VOLTCONTROL_H_
#define INC_CELLS_VOLTCONTROL_H_

#include "auxiliary_function.h"
#include "stm32f0xx_hal.h"

/**************type declaration****************/
typedef enum { CELL_ONE = 0,
	   	       CELL_TWO = 1 } eBatteryCellSelector_t;
typedef enum {VOLTAGE_PROTECT_CELL_OFF = 0,
			  VOLTAGE_PROTECT_CELL_ON = 1} eBatteryCellVoltageStatusProtect_t;
typedef eBool_t (*BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t)(void);

/****************Function declaration****************/
void BatteryTester_CellsVoltcontrol_protectVoltageCellx(
			eBatteryCellSelector_t, float);
void BatteryTester_CellsVoltcontrol_resetProtectVoltageCellx(
		eBatteryCellSelector_t cell);
eBatteryCellVoltageStatusProtect_t BatteryTester_CellsVoltcontrol_getStatusProtectVoltageCellx(
		eBatteryCellSelector_t);
sVoltRange_t BatteryTester_CellsVoltcontrol_getVoltRangeCellx(
		eBatteryCellSelector_t);
void BatteryTester_CellsVoltcontrol_setVoltRangeCellx(
		eBatteryCellSelector_t, sVoltRange_t*);
void BatteryTester_CellsVoltcontrol_initDecorator(
		BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t,
		BatteryTester_CellsVoltcontrol_isStartHardwareCallback_t);
eBool_t BatteryTester_CellsVoltcontrol_readDataFromEEPROM();
eBool_t BatteryTester_CellsVoltcontrol_isStartCellOne();
eBool_t BatteryTester_CellsVoltcontrol_isStartCellTwo();
void BatteryTester_CellsVoltcontrol_stopPWMCh1();
void BatteryTester_CellsVoltcontrol_stopPWMCh2();

#endif /* INC_CELLS_VOLTCONTROL_H_ */

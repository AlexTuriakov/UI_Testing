/*
 * dessipator_control.h
 *
 *  Created on: Apr 18, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_DESSIPATOR_CONTROL_H_
#define INC_DESSIPATOR_CONTROL_H_

#include "auxiliary_function.h"

typedef enum {DESSIPATOR_OFF = 0,
	DESSIPATOR_ON = 1} DessipatorStatus_t;

void BatteryTester_DessipatorControl_initHeaterControl();
void BattetyTester_DessipatorControl_onHeaterControl(float);
void BatteryTester_DessipatorControl_onHeater();
void BatteryTester_DessipatorControl_offHeater();
void BatteryTester_DessipatorControl_onFan();
void BatteryTester_DessipatorControl_offFan();
sVoltRange_t BatteryTester_DessipatorControl_getHeaterControlRange();
void BatteryTester_DessipatorControl_setHeaterControlRange(sVoltRange_t*);
void BatteryTester_DessipatorControl_setHeaterControlMin(float);
void BatteryTester_DessipatorControl_setHeaterControlMax(float);
void BatteryTester_DessipatorControl_resetDessipatorStatus();
DessipatorStatus_t BatteryTester_DessipatorControl_getDessipatorStatus();

#endif /* INC_DESSIPATOR_CONTROL_H_ */

/*
 * set_param.h
 *
 *  Created on: May 7, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_SET_PARAM_H_
#define INC_SET_PARAM_H_

#include "state_events.h"

/**************type declaration****************/

/****************Function declaration****************/
void BatteryTester_SetParam_init(float);
float BatteryTester_SetParam_getParam();
void BatteryTester_SetParam_setParam(float);
void BatteryTester_SetParam_ok();
void BatteryTester_SetParam_display();
void BatteryTester_moveCursor();
void BatteryTester_SetParam_left();
void BatteryTester_SetParam_right();
void BatteryTester_SetParam_up();
void BatteryTester_SetParam_down();

#endif /* INC_SET_PARAM_H_ */

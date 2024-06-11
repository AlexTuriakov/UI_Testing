/*
 * set_param.h
 *
 *  Created on: May 7, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_SET_PARAM_H_
#define INC_SET_PARAM_H_


/**************type declaration****************/
typedef struct _sParamsSets{
	unsigned char sign;
	unsigned char integerOrder;
	unsigned char decimalOrder;
	float value;
	float minValue;
	float maxValue;
} sParamSets_t;

/****************Function declaration****************/
void BatteryTester_SetParam_init(sParamSets_t);
float BatteryTester_SetParam_getParam();
void BatteryTester_SetParam_setParam(sParamSets_t);
void BatteryTester_SetParam_ok();
void BatteryTester_SetParam_left();
void BatteryTester_SetParam_right();
void BatteryTester_SetParam_up();
void BatteryTester_SetParam_down();

#endif /* INC_SET_PARAM_H_ */

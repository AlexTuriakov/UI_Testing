/*
 * auxiliary_function.h
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_AUXILIARY_FUNCTION_H_
#define INC_AUXILIARY_FUNCTION_H_

#define SIZE_LINE_BUFFER_LCD 17

/**************type declaration****************/
typedef struct _sVoltRange_t{
	float minVoltageInVolts;
	float maxVoltageInVolts;
} sVoltRange_t;

typedef enum{ FALSE,
			  TRUE } eBool_t;
/****************Function declaration****************/
void __BatteryTester_AuxiliaryFunction_copy(void*, void*, unsigned int);
void BatteryTester_AuxiliaryFunction_setStartTickForTimerCellOne();
void BatteryTester_AuxiliaryFunction_setStatrTickForTimerCellTwo();
void BatteryTester_AuxiliaryFunction_setPeriodLoggingTestCellOneInMillisec(unsigned int);
void  BatteryTester_AuxiliaryFunction_setPeriodLoggingTestCellTwoInMillisec(unsigned int);
unsigned int BatteryTester_AuxiliaryFunction_getPeriodLoggingTestCellOneInMillisec();
unsigned int BatteryTester_AuxiliaryFunction_getPeriodLoggingTestCellTwoInMillisec();
eBool_t  BatteryTester_AuxiliaryFunction_isTimeLoggedCellOne();
eBool_t BatteryTester_AuxiliaryFunction_isTimeLoggedCellTwo();
void BatteryTester_AuxiliaryFunction_setPeriodLoggingTestCellOneInMillisec(unsigned int);
void BatteryTester_AuxiliaryFunction_setPeriodLoggingTestCellTwoInMillisec(unsigned int);
void BatteryTester_AuxiliaryFunction_initLogger();

#endif /* INC_AUXILIARY_FUNCTION_H_ */

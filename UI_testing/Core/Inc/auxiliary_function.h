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

/****************Function declaration****************/
void __BatteryTester_AuxiliaryFunction_copy(void*, void*, unsigned int);

#endif /* INC_AUXILIARY_FUNCTION_H_ */

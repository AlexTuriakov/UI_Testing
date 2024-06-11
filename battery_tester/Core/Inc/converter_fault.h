/*
 * converter_fault.h
 *
 *  Created on: May 16, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CONVERTER_FAULT_H_
#define INC_CONVERTER_FAULT_H_

#include "auxiliary_function.h"
/**************type declaration****************/
typedef enum{ CONVERTER_FAULT_OFF,
			  CONVERTER_FAULT_ON
} eConverterFaultState_t;
typedef enum{ FAULT_SOUND_OFF,
			  FAULT_SOUND_ON
} eSoundFaultState_t;
typedef eBool_t (*BatteryTester_ConverterFault_isConverterFaultCallback)(void);
typedef void (*BatteryTester_ConverterFault_hardwareCallback_t)(void);


/****************Function declaration****************/
void BatteryTester_ConverterFault_initDecorator(
		BatteryTester_ConverterFault_isConverterFaultCallback,
		BatteryTester_ConverterFault_hardwareCallback_t,
		BatteryTester_ConverterFault_hardwareCallback_t,
		BatteryTester_ConverterFault_hardwareCallback_t);
void BatteryTester_ConverterFault_faultHandler();
void BatteryTester_ConverterFault_resetFaultHandler();
eBool_t BatteryTester_ConverterFault_isConverterFault();
void BatteryTester_ConverterFault_soundFaultOn();
void BatteryTester_ConverterFault_soundFaultOff();

#endif /* INC_CONVERTER_FAULT_H_ */

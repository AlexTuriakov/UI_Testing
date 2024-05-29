/*
 * flash_operation.h
 *
 *  Created on: May 23, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_FLASH_OPERATION_H_
#define INC_FLASH_OPERATION_H_

#include "auxiliary_function.h"


typedef void (*BatteryTester_EEPROM_HardwareOperationCallback_t)(unsigned char*, unsigned int);
typedef eBool_t (*BatteryTester_EEPROM_HardwareStateCallback_t)(void);
typedef unsigned int (*BatteryTester_EEPROM_HardwareCalculateCrcCallback_t)(unsigned int*, unsigned int);

void BatteryTester_EEPROM_logTestingDatasCellOne(unsigned int, float, float, float);
void BatteryTester_EEPROM_logTestingDatasCellTwo(unsigned int, float, float, float);
HAL_StatusTypeDef BatteryTester_EEPROM_initDecorator(
		BatteryTester_EEPROM_HardwareOperationCallback_t transmitDmaCallback,
		BatteryTester_EEPROM_HardwareStateCallback_t isBusySpiCallback,
		BatteryTester_EEPROM_HardwareCalculateCrcCallback_t calculateCrcCallback)
void BatteryTester_EEPROM_processLogging();
void BatteryTester_EEPROM_writePageLogCellOne();
void BatteryTester_EEPROM_writePageLogCellTwo();
unsigned int BatteryTester_EEPROM_getAddressPage(unsigned int, unsigned int);
void BatteryTester_EEPROM_newLogCellOne();
void BatteryTester_EEPROM_newLogCellTwo();
eBool_t BatteryTester_EEPROM_isBusySpi();
void BatteryTester_EEPROM_eraseBlockCommand(unsigned char*, unsigned int, unsigned int);

#endif /* INC_FLASH_OPERATION_H_ */

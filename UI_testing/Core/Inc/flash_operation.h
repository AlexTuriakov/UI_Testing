/*
 * flash_operation.h
 *
 *  Created on: May 23, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_FLASH_OPERATION_H_
#define INC_FLASH_OPERATION_H_

#include "stm32f0xx_hal.h"
#include "auxiliary_function.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "climat_regulator.h"
#include "conversion_data.h"
#include "dessipator_control.h"
#include "cells_voltcontrol.h"

/*****************************************************/
#define ERROR_TIMEOUT						0x1
#define ERROR_READ							0x2
#define ERROR_PROGRAM						0x4
#define ERROR_CHECK_WRITE					0x8
#define ERROR_SET_REG_ONE					0x10
#define ERROR_SET_REG_TWO					0x20
#define ERROR_SET_REG_CLIMAT				0x40
#define ERROR_SET_MEASUREMENT				0x80
#define ERROR_SET_DESSIPATOR				0x100
#define ERROR_SET_VOLTCONTROL				0x200
#define ERROR_SET_LOGGER					0x400
#define ERROR_MEMORY_OUT					0x800
#define ERROR_BUFFER_ONE					0x1000
#define ERROR_BUFFER_TWO					0x2000
#define ERROR_NOT_TRANSMIT_CALLBACK			0x4000
#define ERROR_NOT_TRANSMIT_RECEIVE_CALLBACK 0x8000
#define ERROR_BUFFER_READWRITE				0x10000
#define ERROR_BAD_MEMORY_POINTER			0x20000
#define ERROR_MATCH_CRC						0x40000
#define ERROR_NOT_BUSY_CALLBACK				0x80000

/*****************************************************/

typedef void (*BatteryTester_EEPROM_HardwareOperationCallback_t)(unsigned char*, unsigned char*, unsigned int);
typedef eBool_t (*BatteryTester_EEPROM_HardwareStateCallback_t)(void);
typedef unsigned char (*BatteryTester_EEPROM_HardwareCalculateCrcCallback_t)(unsigned char*, unsigned int);
typedef void (*BatteryTester_EEPROM_displayStatusCallback_t)(const char*);

void BatteryTester_EEPROM_logTestingDatasCellOne(unsigned int, float, float, float);
void BatteryTester_EEPROM_logTestingDatasCellTwo(unsigned int, float, float, float);
HAL_StatusTypeDef BatteryTester_EEPROM_initDecorator(
		BatteryTester_EEPROM_HardwareOperationCallback_t transmitDmaCallback,
				BatteryTester_EEPROM_HardwareOperationCallback_t transmitRecieveDmaCallback,
				BatteryTester_EEPROM_HardwareStateCallback_t isBusySpiCallback,
				BatteryTester_EEPROM_HardwareCalculateCrcCallback_t calculateCrcCallback);
void BatteryTester_EEPROM_processLogging();
void BatteryTester_EEPROM_writePageLogCellOne();
void BatteryTester_EEPROM_writePageLogCellTwo();
unsigned int BatteryTester_EEPROM_getAddressPage(unsigned int, unsigned int);
void BatteryTester_EEPROM_newLogCellOne();
void BatteryTester_EEPROM_newLogCellTwo();
eBool_t BatteryTester_EEPROM_isBusySpi();
void BatteryTester_EEPROM_eraseBlockCommand(unsigned char*, unsigned int, unsigned int);
eBool_t BatteryTester_EEPROM_programAllSettingsSecure();
eBool_t BatteryTester_EEPROM_begin();
void BatteryTester_EEPROM_end();
void BatteryTester_EEPROM_resetLastError();
unsigned int BatteryTester_EEPROM_getLastError();
unsigned char BatteryTester_EEPROM_calculateCRC(unsigned char*, unsigned int);
eBool_t BatteryTester_EEPROM_readSetLogger(unsigned int*, unsigned int*);
eBool_t BatteryTester_EEPROM_readSetVoltcontrol(sVoltRange_t*, sVoltRange_t*);
eBool_t BatteryTester_EEPROM_readSetClimatcontrol(sPIDController_t*, sPWMSettings_t*);
eBool_t BatteryTester_EEPROM_readSetMeasurement(sMinValueFromRange_t*, sMaxValueFromRange_t*,
		sValueCalibrationOffset_t*, ntcSchemeParameters_t*,	float*);
eBool_t BatteryTester_EEPROM_readSetDessipator(sVoltRange_t*);
eBool_t BatteryTester_EEPROM_readSetRegCellOne(sPIDController_t*, sPIDController_t*,
		sPWMSettings_t*, sPWMSettings_t*);
eBool_t BatteryTester_EEPROM_readSetRegCellTwo(sPIDController_t*, sPIDController_t*,
		sPWMSettings_t*, sPWMSettings_t*);
void BatteryTester_EEPROM_setDisplayStatusCallback(
		BatteryTester_EEPROM_displayStatusCallback_t);

#endif /* INC_FLASH_OPERATION_H_ */

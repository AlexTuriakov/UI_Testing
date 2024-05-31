/*
 * flash_operation.c
 *
 *  Created on: May 23, 2024
 *      Author: Tyuryakov_OA
 */
#include "flash_operation.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "climat_regulator.h"
#include "conversion_data.h"
#include "dessipator_control.h"
#include "cells_voltcontrol.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // для malloc, free

/******************************************************/
#define EEPROM_PAGE_SIZE 256
#define PAGE_BUFFER_SIZE 260
#define SIZE_AREA_OF_LOG 0x70000
#define NUM_BLOCKS_OF_LOG_AREA 7
#define _5_ms 			5
#define _1_ms 			1
#define BLOCK_SIZE 		0x10000
#define SPI_WAIT_CYCLE	200000
#define MARK_DATA_WRITE	0x1DB7

/******************************************************/
#define COMMAND_WRITE_ENABLE 	0x06
#define COMMAND_PROGRAM_PAGE	0x02
#define COMMAND_ERASE_BLOCK 	0xd8
#define COMMAND_ERASE_SECTOR	0x20
#define COMMAND_READ_ARRAY		0x0b

/*****************************************************/
#define ADDRESS_SETTINGS_PAGES 				0x00000000
#define ADDRESS_REGULATOR_CELL_ONE_SETTINGS 0x00000000
#define ADDRESS_REGULATOR_CELL_TWO_SETTINGS 0x00000100
#define ADDRESS_CONVERSION_DATA_SETTINGS	0x00000200
#define ADDRESS_CLIMAT_REGULATOR_SETTINGS 	0x00000300
#define ADDRESS_DESSIPATOR_CONTROL_SETTINGS 0x00000400
#define ADDRESS_VOLT_CONTROL_SETTINGS		0x00000500
#define ADDRESS_LOGGER_SETTINGS				0x00000600
#define ADDRESS_TESTING_DATA_CELL_ONE 		0x00010000
#define ADDRESS_TESTING_DATA_CELL_TWO 		0x00080000

/*****************************************************/

/*****************************************************/
static unsigned char* g_bufferPageForCellOne = 0;
static unsigned char* g_bufferPageForCellTwo = 0;
static unsigned short g_iterBufferCellOne = 4;
static unsigned short g_iterBufferCellTwo = 4;
const unsigned int SIZE_TISTING_DATA = sizeof(unsigned int) + sizeof(float) * 3;
const unsigned int g_limitBuffer = PAGE_BUFFER_SIZE - SIZE_TISTING_DATA;
static BatteryTester_EEPROM_HardwareOperationCallback_t g_transmitDmaCallback = 0;
static BatteryTester_EEPROM_HardwareOperationCallback_t g_transmitRecieveDmaCallback = 0;
static BatteryTester_EEPROM_HardwareStateCallback_t g_isBusySpiCallback = 0;
static BatteryTester_EEPROM_HardwareCalculateCrcCallback_t g_calculateCrcCallback = 0;
static unsigned int g_addressCurrentPageForCellOne = ADDRESS_TESTING_DATA_CELL_ONE;
static unsigned int g_addressCurrentPageForCellTwo = ADDRESS_TESTING_DATA_CELL_TWO;
static eBool_t g_flagTransmitCellOne = FALSE;
static eBool_t g_flagTransmitCellTwo = FALSE;
static eBool_t g_flagNewSessionCellOne = FALSE;
static eBool_t g_flagNewSessionCellTwo = FALSE;
static unsigned short g_countBlocksForCellOne, g_countBlocksForCellTwo;
static unsigned int g_tick1, g_tick2;
static unsigned char *g_pBuffer1, *g_pBuffer2;
static unsigned char *g_readWriteBuffer = 0;
static unsigned int g_lastError = 0;

/*
 * ****@private functions********
 */
static inline void _BatteryTester_EEPROM_transmitDmaWithSetWELb(unsigned char*, unsigned int);
eBool_t _BatteryTester_EEPROM_programSettingsRegCellOne();
eBool_t _BatteryTester_EEPROM_programSettingsRegCellTwo();
eBool_t _BatteryTester_EEPROM_programSettingsRegClimat();
eBool_t _BatteryTester_EEPROM_programSettingsMeasurement();
eBool_t _BatteryTester_EEPROM_programSettingsDessipator();
eBool_t _BatteryTester_EEPROM_programSettingsVoltControl();
eBool_t _BatteryTester_EEPROM_programSettingsLogger();
eBool_t _BatteryTester_EEPROM_readSettings(unsigned int,unsigned char*, unsigned int);
eBool_t _BatteryTester_EEPROM_checkRead(unsigned char*, unsigned int);
static inline eBool_t _BatteryTester_EEPROM_checkWrite(unsigned int, unsigned int);
/***************************************************************************/
/*
 * @deprecated
 */
//static unsigned int g_numCurrentPageForCellOne = 0;
//static unsigned int g_numCurrentPageForCellTwo = 0;

void BatteryTester_EEPROM_logTestingDatasCellOne(
		unsigned int tick,
		float current,
		float voltage,
		float temperature){
	if(g_iterBufferCellOne <= g_limitBuffer){
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &tick, 4); //magic number: sizeof(unsigned int)
		g_iterBufferCellOne += 4;
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &current, 4); //sizeof(float)
		g_iterBufferCellOne += 4;
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &voltage, 4);
		g_iterBufferCellOne += 4;
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &temperature, 4);
		g_iterBufferCellOne += 4;
	}
}

void BatteryTester_EEPROM_logTestingDatasCellTwo(
		unsigned int tick,
		float current,
		float voltage,
		float temperature){
	if(g_iterBufferCellTwo <= g_limitBuffer){
		memcpy(&g_bufferPageForCellTwo[g_iterBufferCellTwo], &tick, 4); //sizeof(unsigned int)
		g_iterBufferCellTwo += 4;
		memcpy(&g_bufferPageForCellTwo[g_iterBufferCellTwo], &current, 4); //sizeof(float)
		g_iterBufferCellTwo += 4;
		memcpy(&g_bufferPageForCellTwo[g_iterBufferCellTwo], &voltage, 4);
		g_iterBufferCellTwo += 4;
		memcpy(&g_bufferPageForCellTwo[g_iterBufferCellTwo], &temperature, 4);
		g_iterBufferCellTwo += 4;
	}
}

HAL_StatusTypeDef BatteryTester_EEPROM_initDecorator(
		BatteryTester_EEPROM_HardwareOperationCallback_t transmitDmaCallback,
		BatteryTester_EEPROM_HardwareOperationCallback_t transmitRecieveDmaCallback,
		BatteryTester_EEPROM_HardwareStateCallback_t isBusySpiCallback,
		BatteryTester_EEPROM_HardwareCalculateCrcCallback_t calculateCrcCallback){
	g_transmitRecieveDmaCallback = transmitRecieveDmaCallback;
	g_calculateCrcCallback = calculateCrcCallback;
	g_isBusySpiCallback = isBusySpiCallback;
	g_transmitDmaCallback = transmitDmaCallback;
	g_lastError = 0; // not error
	if(!g_bufferPageForCellOne){
		g_bufferPageForCellOne = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
		if(!g_bufferPageForCellOne){
			g_lastError |= ERROR_BUFFER_ONE | ERROR_MEMORY_OUT;
			return HAL_ERROR;
		}
	}
	if(!g_bufferPageForCellTwo){
		g_bufferPageForCellTwo = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
		if(!g_bufferPageForCellTwo){
			g_lastError |= ERROR_BUFFER_TWO | ERROR_MEMORY_OUT;
			return HAL_ERROR;
		}
	}
	return HAL_OK;
}

void BatteryTester_EEPROM_processLogging(){
	if(g_flagNewSessionCellOne){
		if(g_countBlocksForCellOne){
			unsigned int address = ADDRESS_TESTING_DATA_CELL_ONE +
					(NUM_BLOCKS_OF_LOG_AREA - g_countBlocksForCellOne) * BLOCK_SIZE;
			if(!BatteryTester_EEPROM_isBusySpi()){
				BatteryTester_EEPROM_eraseBlockCommand(g_pBuffer1, address, 4);
				g_countBlocksForCellOne--;
			}
		}
		else{
			g_flagNewSessionCellOne = FALSE;
			free(g_pBuffer1);
			g_pBuffer1 = 0;
		}
	}
	else{
		//if the cell is off then just write down the remaining data
		if(g_iterBufferCellOne > 4 && !BatteryTester_RegulatorCellOne_getRunStatus()){
			if(!BatteryTester_EEPROM_isBusySpi()){
				BatteryTester_EEPROM_writePageLogCellOne();
				g_iterBufferCellOne = 4;
			}
		}
		// page is full time is write to eeprom
		if(g_iterBufferCellOne >= PAGE_BUFFER_SIZE){
			// write data in eeprom
			if(!g_flagTransmitCellOne && !BatteryTester_EEPROM_isBusySpi()){
				BatteryTester_EEPROM_writePageLogCellOne();
				g_flagTransmitCellOne = TRUE;
				g_tick1 = HAL_GetTick();
			}
			//set flag transmit run to not write again
			//wait 5ms check is busy spi
			//when spi free set begin iter
			if(g_flagTransmitCellOne && HAL_GetTick() - g_tick1 >= _1_ms){
				if(!BatteryTester_EEPROM_isBusySpi()){
					g_flagTransmitCellOne = FALSE;
					g_iterBufferCellOne = 4;
				}
			}
		}
	}

	if(g_flagNewSessionCellTwo){
		if(g_countBlocksForCellTwo){
			unsigned int address = ADDRESS_TESTING_DATA_CELL_TWO +
					(NUM_BLOCKS_OF_LOG_AREA - g_countBlocksForCellTwo) * BLOCK_SIZE;
			if(!BatteryTester_EEPROM_isBusySpi()){
				BatteryTester_EEPROM_eraseBlockCommand(g_pBuffer2, address, 4);
				g_countBlocksForCellTwo--;
			}
		}
		else{
			g_flagNewSessionCellTwo = FALSE;
			free(g_pBuffer2);
			g_pBuffer2 = 0;
		}
	}
	else{
		if(g_iterBufferCellTwo > 4 && !BatteryTester_RegulatorCellTwo_getRunStatus()){
			if(!BatteryTester_EEPROM_isBusySpi()){
				BatteryTester_EEPROM_writePageLogCellTwo();
				g_iterBufferCellTwo = 4;
			}
		}
		if(g_iterBufferCellTwo >= PAGE_BUFFER_SIZE){
			// write page in eeprom
			if(!g_flagTransmitCellTwo && !BatteryTester_EEPROM_isBusySpi()){
				BatteryTester_EEPROM_writePageLogCellTwo();
				g_flagTransmitCellTwo = TRUE;
				g_tick2 = HAL_GetTick();
			}
			if(g_flagTransmitCellTwo && HAL_GetTick() - g_tick2 >= _1_ms){
				g_flagTransmitCellTwo = FALSE;
				g_iterBufferCellTwo = 4;
			}
		}
	}
}

void BatteryTester_EEPROM_writePageLogCellOne(){
	if(g_transmitDmaCallback && g_bufferPageForCellOne){
		memcpy(&g_bufferPageForCellOne[0], &g_addressCurrentPageForCellOne, 4);
		g_bufferPageForCellOne[0] = COMMAND_PROGRAM_PAGE;
		unsigned char command = COMMAND_WRITE_ENABLE;
		g_transmitDmaCallback(&command, NULL, 1);
		for(int i = 0; i < SPI_WAIT_CYCLE; i++){
			if(!BatteryTester_EEPROM_isBusySpi()){
				g_transmitDmaCallback(g_bufferPageForCellOne, NULL, (unsigned int)g_iterBufferCellOne);
				g_addressCurrentPageForCellOne += EEPROM_PAGE_SIZE;
				break;
				//		g_numCurrentPageForCellOne++;
			}
		}
	}
}

void BatteryTester_EEPROM_writePageLogCellTwo(){
	if(g_transmitDmaCallback && g_bufferPageForCellTwo){
		memcpy(&g_bufferPageForCellTwo[0], &g_addressCurrentPageForCellTwo, 4);
		g_bufferPageForCellTwo[0] = COMMAND_PROGRAM_PAGE;
		unsigned char command = COMMAND_WRITE_ENABLE;
		g_transmitDmaCallback(&command, NULL, 1);
		for(int i = 0; i < SPI_WAIT_CYCLE; i++){
			if(!BatteryTester_EEPROM_isBusySpi()){
				g_transmitDmaCallback(g_bufferPageForCellTwo, NULL, (unsigned int)g_iterBufferCellTwo);
				g_addressCurrentPageForCellTwo += EEPROM_PAGE_SIZE;
				break;
		//		g_numCurrentPageForCellTwo++;
			}
		}
	}
}

/*
 * @deprecated
 */
unsigned int BatteryTester_EEPROM_getAddressPage(unsigned int beginAddress, unsigned int page){
	return beginAddress + page * EEPROM_PAGE_SIZE;
}

void BatteryTester_EEPROM_newLogCellOne(){
	//g_lastError = 0;
	// erase memory before testing
	// set current page address
	if(g_flagNewSessionCellOne){
		return;						//deja vu
	}
	g_flagNewSessionCellOne = TRUE;
	g_addressCurrentPageForCellOne = ADDRESS_TESTING_DATA_CELL_ONE;
	g_iterBufferCellOne = 4;
	g_countBlocksForCellOne = NUM_BLOCKS_OF_LOG_AREA;
	if(!g_pBuffer1){
		g_pBuffer1 = (unsigned char*)malloc(4);
		if(!g_pBuffer1){
			g_lastError = ERROR_MEMORY_OUT;
		}
	}
}

void BatteryTester_EEPROM_newLogCellTwo(){
	//g_lastError = 0;
	// erase memory before testing
	// set current page address
	if(g_flagNewSessionCellTwo){
		return;
	}
	g_flagNewSessionCellTwo = TRUE;
	g_addressCurrentPageForCellTwo = ADDRESS_TESTING_DATA_CELL_TWO;
	g_iterBufferCellTwo = 4;
	g_countBlocksForCellTwo = NUM_BLOCKS_OF_LOG_AREA;
	if(!g_pBuffer2){
		g_pBuffer2 = (unsigned char*)malloc(4);
		if(!g_pBuffer2){
			g_lastError = ERROR_MEMORY_OUT;
		}
	}
}

eBool_t BatteryTester_EEPROM_isBusySpi(){
	if(g_isBusySpiCallback){
		return g_isBusySpiCallback();
	}
	return TRUE;
}

void BatteryTester_EEPROM_eraseBlockCommand(unsigned char* pBuffer, unsigned int address, unsigned int size){
	//g_lastError = 0;
	if(!pBuffer || !g_transmitDmaCallback){
		g_lastError |= ERROR_BAD_MEMORY_POINTER | ERROR_NOT_TRANSMIT_CALLBACK;
		return;
	}
	memcpy(&pBuffer[0], &address, 4);
	pBuffer[0] = COMMAND_ERASE_BLOCK;
	_BatteryTester_EEPROM_transmitDmaWithSetWELb(pBuffer, 4);
}

/*
 * @private
 */
static inline void _BatteryTester_EEPROM_transmitDmaWithSetWELb(unsigned char* pBuffer, unsigned int size){
	unsigned char command = COMMAND_WRITE_ENABLE;
	g_transmitDmaCallback(&command, NULL, 1);
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			g_transmitDmaCallback(pBuffer, NULL, size);
			break;
		}
	}
}

void BatteryTester_EEPROM_eraseSectorCommand(unsigned char* pBuffer, unsigned int address){
	//g_lastError = 0;
	if(!pBuffer || !g_transmitDmaCallback){
		g_lastError |= ERROR_BAD_MEMORY_POINTER | ERROR_NOT_TRANSMIT_CALLBACK;
		return;
	}

	memcpy(&pBuffer[0], &address, 4);
	pBuffer[0] = COMMAND_ERASE_SECTOR;
	_BatteryTester_EEPROM_transmitDmaWithSetWELb(pBuffer, 4);
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsRegCellOne(){
	//g_lastError = 0;
	unsigned int address = ADDRESS_REGULATOR_CELL_ONE_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	// get settings and assign them to g_readWriteBuffer
	sPIDController_t buck = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	sPWMSettings_t buckPwm = BattetyTester_RegulatorCellOne_getBuckPWMSettings();
	sPIDController_t boost = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	sPWMSettings_t boostPwm = BattetyTester_RegulatorCellOne_getBoostPWMSettings();
	unsigned int iter = 4, temp = sizeof(sPIDController_t);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &buck, temp);
	iter += temp;
	memcpy(&g_readWriteBuffer[iter], &boost, temp);
	iter += temp;
	temp = sizeof(sPWMSettings_t);
	memcpy(&g_readWriteBuffer[iter], &buckPwm, temp);
	iter += temp;
	memcpy(&g_readWriteBuffer[iter], &boostPwm, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_REG_ONE | ERROR_TIMEOUT;
	return FALSE;
}

unsigned char BatteryTester_EEPROM_calculateCRC(unsigned char* pBuffer, unsigned int lengthOfBytes){
	if(g_calculateCrcCallback){
		return (unsigned char) g_calculateCrcCallback((unsigned int*) pBuffer, lengthOfBytes);
	}
	return -1;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsRegCellTwo(){
	//g_lastError = 0;
	unsigned int address = ADDRESS_REGULATOR_CELL_TWO_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	// get settings and assign them to g_readWriteBuffer
	sPIDController_t buck = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	sPWMSettings_t buckPwm = BattetyTester_RegulatorCellTwo_getBuckPWMSettings();
	sPIDController_t boost = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	sPWMSettings_t boostPwm = BattetyTester_RegulatorCellTwo_getBoostPWMSettings();
	unsigned int iter = 4, temp = sizeof(sPIDController_t);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &buck, temp);
	iter += temp;
	memcpy(&g_readWriteBuffer[iter], &boost, temp);
	iter += temp;
	temp = sizeof(sPWMSettings_t);
	memcpy(&g_readWriteBuffer[iter], &buckPwm, temp);
	iter += temp;
	memcpy(&g_readWriteBuffer[iter], &boostPwm, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_REG_TWO | ERROR_TIMEOUT;
	return FALSE;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsRegClimat(){
	//g_lastEror = 0;
	unsigned int address = ADDRESS_CLIMAT_REGULATOR_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	sPIDController_t reg = BatteryTester_ClimatRegulator_getRegulatorSettings();
	sPWMSettings_t pwm = BattetyTester_ClimatRegulator_getPWMSettings();
	unsigned int iter = 4, temp = sizeof(sPIDController_t);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &reg, temp);
	iter += temp;
	temp = sizeof(sPWMSettings_t);
	memcpy(&g_readWriteBuffer[iter], &pwm, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_REG_CLIMAT | ERROR_TIMEOUT;
	return FALSE;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsMeasurement(){
	//g_lastError = 0;
	unsigned int address = ADDRESS_CONVERSION_DATA_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	sMinValueFromRange_t min = BatteryTester_ConversionData_getMinValueFromRange();
	sMaxValueFromRange_t max = BatteryTester_ConversionData_getMaxValueFromRange();
	sValueCalibrationOffset_t offset = BatteryTester_ConversionData_getValueCalibrationOffset();
	ntcSchemeParameters_t ntc[NUM_TEMPERATURE];
	for(unsigned short i = 0; i < NUM_TEMPERATURE; i++)
		ntc[i] = BatteryTester_ConversionData_getNtcSchemeParams(i);
	float dac = BatteryTester_ConversionData_getRefOffsetInVolts();
	unsigned int iter = 4, temp = sizeof(sMinValueFromRange_t);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &min, temp);
	iter += temp;
	temp = sizeof(sMaxValueFromRange_t);
	memcpy(&g_readWriteBuffer[iter], &max, temp);
	iter += temp;
	temp = sizeof(sValueCalibrationOffset_t);
	memcpy(&g_readWriteBuffer[iter], &offset, temp);
	iter +=temp;
	temp = sizeof(ntcSchemeParameters_t) * NUM_TEMPERATURE;
	memcpy(&g_readWriteBuffer[iter], ntc, temp);
	iter += temp;
	temp = sizeof(float);
	memcpy(&g_readWriteBuffer[iter], &dac, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_MEASUREMENT | ERROR_TIMEOUT;
	return FALSE;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsDessipator(){
	//g_lastError = 0;
	unsigned int address = ADDRESS_DESSIPATOR_CONTROL_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	sVoltRange_t rng = BatteryTester_DessipatorControl_getHeaterControlRange();
	unsigned int iter = 4, temp = sizeof(sVoltRange_t);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &rng, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_DESSIPATOR | ERROR_TIMEOUT;
	return FALSE;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsVoltControl(){
	//g_lastError = 0;
	unsigned int address = ADDRESS_VOLT_CONTROL_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	sVoltRange_t rng = BatteryTester_DessipatorControl_getHeaterControlRange();
	unsigned int iter = 4, temp = sizeof(sVoltRange_t);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &rng, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_VOLTCONTROL | ERROR_TIMEOUT;
	return FALSE;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_programSettingsLogger(){
	//g_lastError = 0;
	unsigned int address = ADDRESS_LOGGER_SETTINGS;
	memcpy(&g_readWriteBuffer[0], &address, 4);
	g_readWriteBuffer[0] = COMMAND_PROGRAM_PAGE;
	unsigned int set1 = BatteryTester_AuxiliaryFunction_getPeriodLoggingTestCellOneInMillisec();
	unsigned int set2 = BatteryTester_AuxiliaryFunction_getPeriodLoggingTestCellTwoInMillisec();
	unsigned int iter = 4, temp = sizeof(unsigned int);
	// to begin mark of data write
	unsigned short marker = MARK_DATA_WRITE;
	memcpy(&g_readWriteBuffer[iter], &marker, 2);
	iter +=2;
	memcpy(&g_readWriteBuffer[iter], &set1, temp);
	iter += temp;
	memcpy(&g_readWriteBuffer[iter], &set2, temp);
	iter += temp;
	// to end crc value
	g_readWriteBuffer[iter] = BatteryTester_EEPROM_calculateCRC(&g_readWriteBuffer[6], iter - 6);
	iter++;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			_BatteryTester_EEPROM_transmitDmaWithSetWELb(g_readWriteBuffer, iter);
			return TRUE;
		}
	}
	g_lastError |= ERROR_SET_LOGGER | ERROR_TIMEOUT;
	return FALSE;
}


eBool_t BatteryTester_EEPROM_programAllSettingsSecure(){
	eBool_t ret = TRUE;
	// opcode + address + mark_write + data + crc
	unsigned int size = 4 + 1 + 2 * sizeof(sPIDController_t) + 2 * sizeof(sPWMSettings_t) + 1;
	BatteryTester_EEPROM_resetLastError();
	if(BatteryTester_EEPROM_begin()){
		BatteryTester_EEPROM_eraseSectorCommand(g_readWriteBuffer, ADDRESS_SETTINGS_PAGES);
		// write and read for check
		/*****************************************************/
		if(_BatteryTester_EEPROM_programSettingsRegCellOne()){
			HAL_Delay(1);
			//size =
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_REGULATOR_CELL_ONE_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/****************************************************/
		if(_BatteryTester_EEPROM_programSettingsRegCellTwo()){
			HAL_Delay(1);
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_REGULATOR_CELL_TWO_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/*****************************************************/
		if(_BatteryTester_EEPROM_programSettingsRegClimat()){
			HAL_Delay(1);
			size = 4 + 1 + sizeof(sPIDController_t) + sizeof(sPWMSettings_t) + 1;
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_CLIMAT_REGULATOR_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/************************************************/
		if(_BatteryTester_EEPROM_programSettingsMeasurement()){
			HAL_Delay(1);
			size = 4 + 1 + sizeof(sMinValueFromRange_t) + sizeof(sMaxValueFromRange_t) +
					sizeof(sValueCalibrationOffset_t) +
					NUM_TEMPERATURE * sizeof(ntcSchemeParameters_t) +
					sizeof(float) + 1;
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_CONVERSION_DATA_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/*******************************************************/
		if(_BatteryTester_EEPROM_programSettingsDessipator()){
			HAL_Delay(1);
			size = 4 + 1 + sizeof(sVoltRange_t) + 1;
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_DESSIPATOR_CONTROL_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/******************************************************/
		if(_BatteryTester_EEPROM_programSettingsVoltControl()){
			HAL_Delay(1);
			size = 4 + 1 + sizeof(sVoltRange_t) + 1;
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_VOLT_CONTROL_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/******************************************************/
		if(_BatteryTester_EEPROM_programSettingsLogger()){
			HAL_Delay(1);
			size = 4 + 1 + 2 * sizeof(unsigned int) + 1;
			ret &= _BatteryTester_EEPROM_checkWrite(ADDRESS_LOGGER_SETTINGS, size);
		}
		else{
			ret = FALSE;
		}
		/******************************************************/
		BatteryTester_EEPROM_end();
	}
	else{
		return FALSE;
	}
	return ret;
}

inline eBool_t _BatteryTester_EEPROM_checkWrite(unsigned int address, unsigned int size){
	if(_BatteryTester_EEPROM_readSettings(address, g_readWriteBuffer, size)){
		HAL_Delay(1);
		if(!_BatteryTester_EEPROM_checkRead(g_readWriteBuffer, size)){
			return FALSE;
		}
	}
	else{
		return FALSE;
	}
	return TRUE;
}

// reading functions will be here

//begin with select resource
eBool_t BatteryTester_EEPROM_begin(){
	//g_lastError = 0;
	if(!g_transmitRecieveDmaCallback || !g_transmitDmaCallback){
		g_lastError |= ERROR_NOT_TRANSMIT_RECEIVE_CALLBACK | ERROR_NOT_TRANSMIT_CALLBACK;
		return FALSE;
	}
	if(!g_readWriteBuffer){
		g_readWriteBuffer = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
		if(!g_readWriteBuffer){
			g_lastError |= ERROR_BUFFER_READWRITE | ERROR_MEMORY_OUT;
		}
		return (eBool_t)g_readWriteBuffer;
	}
	return TRUE;
}

// end operation
void BatteryTester_EEPROM_end(){
	free(g_readWriteBuffer);
	g_readWriteBuffer = 0;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_readSettings(unsigned int address, unsigned char* pBuffer, unsigned int size){
	memcpy(pBuffer, &address, 4);
	pBuffer[0] = COMMAND_READ_ARRAY;
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			g_transmitRecieveDmaCallback(pBuffer, &pBuffer[4], size);
			return TRUE;
		}
	}
	g_lastError |= ERROR_READ | ERROR_TIMEOUT;
	return FALSE;
}

/*
 * @private
 */
eBool_t _BatteryTester_EEPROM_checkRead(unsigned char* pBuffer, unsigned int size){
	for(int i = 0; i < SPI_WAIT_CYCLE; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			unsigned short marker;
			memcpy(&marker, &pBuffer[4], 2);
			if(marker != MARK_DATA_WRITE){
				g_lastError |= ERROR_CHECK_WRITE;
				return FALSE;
			}
			unsigned char crc = BatteryTester_EEPROM_calculateCRC(&pBuffer[6], size - 7);
			if(pBuffer[size - 1] != crc){
				g_lastError |= ERROR_MATCH_CRC;
				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

void BatteryTester_EEPROM_resetLastError(){
	g_lastError = 0;
}

unsigned int BatteryTester_EEPROM_getLastError(){
	return g_lastError;
}

/*
 * flash_operation.c
 *
 *  Created on: May 23, 2024
 *      Author: Tyuryakov_OA
 */
#include "auxiliary_function.h"
#include "flash_operation.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // для malloc, free


#define EEPROM_PAGE_SIZE 256
#define PAGE_BUFFER_SIZE 260

#define COMMAND_WRITE_ENABLE 0x06
#define COMMAND_PROGRAM_PAGE 0x02
#define COMMAND_ERASE_SECTOR 0x20

#define ADDRESS_SETTINGS_PAGES 0x00000000
#define ADDRESS_TESTING_DATA_CELL_ONE 0x00010000
#define ADDRESS_TESTING_DATA_CELL_TWO 0x00080000

static eBool_t g_flagLogCellTwo = FALSE;
static eBool_t g_flagLogCellOne = FALSE;
static unsigned char* g_bufferPageForCellOne = 0;
static unsigned char* g_bufferPageForCellTwo = 0;
static unsigned short g_iterBufferCellOne = 4;
static unsigned short g_iterBufferCellTwo = 4;
static const unsigned int SIZE_TISTING_DATA = sizeof(unsigned int) + sizeof(float) * 3;
static const unsigned int g_limitBuffer = PAGE_BUFFER_SIZE - SIZE_TISTING_DATA;
static BatteryTester_EEPROM_HardwareOperation_t g_transmitDmaCallback = 0;
static unsigned int g_addressCurrentPageForCellOne = ADDRESS_TESTING_DATA_CELL_ONE;
static unsigned int g_addressCurrentPageForCellTwo = ADDRESS_TESTING_DATA_CELL_TWO;
/*
 * @deprecated
 */
static unsigned int g_numCurrentPageForCellOne = 0;
static unsigned int g_numCurrentPageForCellTwo = 0;

void BatteryTester_EEPROM_logTestingDatasCellOne(
		unsigned int tick,
		float current,
		float voltage,
		float temperature){
	if(g_iterBufferCellOne <= g_limitBuffer){
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &tick, 4); //sizeof(unsigned int)
		g_iterBufferCellOne += 4;
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &current, 4); //sizeof(float)
		g_iterBufferCellOne += 4;
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &voltage, 4);
		g_iterBufferCellOne += 4;
		memcpy(&g_bufferPageForCellOne[g_iterBufferCellOne], &temperature, 4);
		g_iterBufferCellOne += 4;
	}
//	g_flagLogCellOne = TRUE;
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

void BatteryTester_EEPROM_initDecorator(
		BatteryTester_EEPROM_HardwareOperation_t transmitDmaCallback){
	g_transmitDmaCallback = transmitDmaCallback;
	if(!g_bufferPageForCellOne){
		g_bufferPageForCellOne = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
	}
	if(!g_bufferPageForCellTwo){
		g_bufferPageForCellTwo = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
	}

}

void BatteryTester_EEPROM_processLogging(){
	if(g_iterBufferCellOne >= PAGE_BUFFER_SIZE){
		// write data in eeprom
		BatteryTester_EEPROM_writePageLogCellOne();
		// clear buffer or alloc new (where it is free?)
		if(!BSY){
			// iter set to begin
			g_iterBufferCellOne = 4;
		}

	}
	if(g_iterBufferCellTwo >= PAGE_BUFFER_SIZE){
		// write data in eeprom
		BatteryTester__EEPROM_writePageLogCellTwo();
		// clear buffer or alloc new (where it is free?)
		if(!BSY){
			g_iterBufferCellTwo = 4;
		}
		// iter set to begin

	}
}

void BatteryTester_EEPROM_writePageLogCellOne(){
	if(g_transmitDmaCallback && g_bufferPageForCellOne){
		memcpy(&g_bufferPageForCellOne[0], &g_addressCurrentPageForCellOne, 4);
		g_bufferPageForCellOne[0] = COMMAND_PROGRAM_PAGE;
		g_transmitDmaCallback(COMMAND_WRITE_ENABLE, 1);
		g_transmitDmaCallback(g_bufferPageForCellOne, (unsigned int)g_iterBufferCellOne);
		g_addressCurrentPageForCellOne += EEPROM_PAGE_SIZE;
//		g_numCurrentPageForCellOne++;
	}
}

void BatteryTester__EEPROM_writePageLogCellTwo(){
	if(g_transmitDmaCallback && g_bufferPageForCellTwo){
		memcpy(&g_bufferPageForCellTwo[0], &g_addressCurrentPageForCellTwo, 4);
		g_bufferPageForCellTwo[0] = COMMAND_PROGRAM_PAGE;
		g_transmitDmaCallback(COMMAND_WRITE_ENABLE, 1);
		g_transmitDmaCallback(g_bufferPageForCellTwo, (unsigned int)g_iterBufferCellTwo);
		g_addressCurrentPageForCellTwo += EEPROM_PAGE_SIZE;
//		g_numCurrentPageForCellTwo++;
	}
}

/*
 * @deprecated
 */
unsigned int BatteryTester_EEPROM_getAddressPage(unsigned int beginAddress, unsigned int page){
	return beginAddress + page * EEPROM_PAGE_SIZE;
}

void BatteryTester_EEPROM_prepareForOnCellOne(){

}

void BatteryTester_EEPROM_prepareForOnCellTwo(){

}

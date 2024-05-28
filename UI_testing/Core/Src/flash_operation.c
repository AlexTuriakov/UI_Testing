/*
 * flash_operation.c
 *
 *  Created on: May 23, 2024
 *      Author: Tyuryakov_OA
 */
#include "flash_operation.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // для malloc, free


#define EEPROM_PAGE_SIZE 256
#define PAGE_BUFFER_SIZE 260
#define SIZE_AREA_OF_LOG 0x70000
#define NUM_BLOCKS_OF_LOG_AREA 7
#define _5_ms 5
#define BLOCK_SIZE 0x10000

#define COMMAND_WRITE_ENABLE 0x06
#define COMMAND_PROGRAM_PAGE 0x02
#define COMMAND_ERASE_BLOCK 0xd8

#define ADDRESS_SETTINGS_PAGES 				0x00000000
#define ADDRESS_REGULATOR_CELL_ONE_SETTINGS 0x00000000
#define ADDRESS_REGULATOR_CELL_TWO_SETTINGS 0x00000100
#define ADDRESS_CONVERSION_DATA_SETTINGS	0x00000200
#define ADDRESS_CLIMAT_REGULATOR_SETTINGS 	0x00000300
#define ADDRESS_DESSIPATOR_CONTROL_SETTINGS 0x00000380
#define ADDRESS_VOLT_CONTROL_SETTINGS		0x00000390
#define ADDRESS_LOGGER_SETTINGS				0x000003b0
#define ADDRESS_EMPTY_PAGE_5				0x00000400
#define ADDRESS_EMPTY_PAGE_6				0x00000500
#define ADDRESS_TESTING_DATA_CELL_ONE 		0x00010000
#define ADDRESS_TESTING_DATA_CELL_TWO 		0x00080000


static unsigned char* g_bufferPageForCellOne = 0;
static unsigned char* g_bufferPageForCellTwo = 0;
static unsigned short g_iterBufferCellOne = 4;
static unsigned short g_iterBufferCellTwo = 4;
const unsigned int SIZE_TISTING_DATA = sizeof(unsigned int) + sizeof(float) * 3;
const unsigned int g_limitBuffer = PAGE_BUFFER_SIZE - SIZE_TISTING_DATA;
static BatteryTester_EEPROM_HardwareOperation_t g_transmitDmaCallback = 0;
static BatteryTester_EEPROM_HardwareState_t g_isBusySpiCallback = 0;
static unsigned int g_addressCurrentPageForCellOne = ADDRESS_TESTING_DATA_CELL_ONE;
static unsigned int g_addressCurrentPageForCellTwo = ADDRESS_TESTING_DATA_CELL_TWO;
static eBool_t g_flagTransmitCellOne = FALSE;
static eBool_t g_flagTransmitCellTwo = FALSE;
static eBool_t g_flagNewSessionCellOne = FALSE;
static eBool_t g_flagNewSessionCellTwo = FALSE;
static unsigned short g_countBlocksForCellOne, g_countBlocksForCellTwo;
static unsigned int g_tick1, g_tick2;
static unsigned char *g_pBuffer1, *g_pBuffer2;

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
		BatteryTester_EEPROM_HardwareOperation_t transmitDmaCallback,
		BatteryTester_EEPROM_HardwareState_t isBusySpiCallback){
	g_isBusySpiCallback = isBusySpiCallback;
	g_transmitDmaCallback = transmitDmaCallback;
	if(!g_bufferPageForCellOne){
		g_bufferPageForCellOne = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
	}
	if(!g_bufferPageForCellTwo){
		g_bufferPageForCellTwo = (unsigned char*)malloc(PAGE_BUFFER_SIZE * sizeof(unsigned char));
	}

}

void BatteryTester_EEPROM_processLogging(){
	if(g_flagNewSessionCellOne){
		if(!g_countBlocksForCellOne){
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
			if(g_flagTransmitCellOne && HAL_GetTick() - g_tick1 >= _5_ms){
				if(!BatteryTester_EEPROM_isBusySpi()){
					g_flagTransmitCellOne = FALSE;
					g_iterBufferCellOne = 4;
				}
			}
		}
	}

	if(g_flagNewSessionCellTwo){
		if(!g_countBlocksForCellTwo){
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
			if(g_flagTransmitCellTwo && HAL_GetTick() - g_tick2 >= _5_ms){
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
		g_transmitDmaCallback(&command, 1);
		for(int i = 0; i < 200000; i++){
			if(!BatteryTester_EEPROM_isBusySpi()){
				g_transmitDmaCallback(g_bufferPageForCellOne, (unsigned int)g_iterBufferCellOne);
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
		g_transmitDmaCallback(&command, 1);
		for(int i = 0; i < 200000; i++){
			if(!BatteryTester_EEPROM_isBusySpi()){
				g_transmitDmaCallback(g_bufferPageForCellTwo, (unsigned int)g_iterBufferCellTwo);
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
	// erase memory before testing
	// set current page address
	g_flagNewSessionCellOne = TRUE;
	g_addressCurrentPageForCellOne = ADDRESS_TESTING_DATA_CELL_ONE;
	g_iterBufferCellOne = 4;
	g_countBlocksForCellOne = NUM_BLOCKS_OF_LOG_AREA;
	g_pBuffer1 = (unsigned char*)malloc(4);

}

void BatteryTester_EEPROM_newLogCellTwo(){
	// erase memory before testing
	// set current page address
	g_flagNewSessionCellTwo = TRUE;
	g_addressCurrentPageForCellTwo = ADDRESS_TESTING_DATA_CELL_TWO;
	g_iterBufferCellTwo = 4;
	g_countBlocksForCellTwo = NUM_BLOCKS_OF_LOG_AREA;
	g_pBuffer2 = (unsigned char*)malloc(4);
}

eBool_t BatteryTester_EEPROM_isBusySpi(){
	if(g_isBusySpiCallback){
		return g_isBusySpiCallback();
	}
	return TRUE;
}

void BatteryTester_EEPROM_eraseBlockCommand(unsigned char* pBuffer, unsigned int address, unsigned int size){
	if(!pBuffer)
		return;
	memcpy(&pBuffer[0], &address, 4);
			pBuffer[0] = COMMAND_ERASE_BLOCK;
	unsigned char command = COMMAND_WRITE_ENABLE;
	g_transmitDmaCallback(&command, 1);
	for(int i = 0; i < 200000; i++){
		if(!BatteryTester_EEPROM_isBusySpi()){
			g_transmitDmaCallback(pBuffer, 4);
			break;
		}
	}
}

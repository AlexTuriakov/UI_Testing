/*
 * conversion_data.c
 *
 *  Created on: Apr 8, 2024
 *      Author: Tyuryakov_OA
 */
#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "conversion_data.h"
#include <math.h>
#include "auxiliary_function.h"


#define VREF 3.0
#define RESOLUTION 65536
#define NUM_TEMPERATURE 4

static sscaleFactors_t factors;
//volatile uint32_t dataAdc[10];
static ntcSchemeParameters_t ntcParams[NUM_TEMPERATURE];

void BatteryTester_ConversionData_calcPhisicValueFromAdcCode(
		volatile uint32_t* rawData, uint32_t sizeData, volatile sphisicValue_t* pValue){
	if(!rawData || !pValue){
		return;
	}
	for(int i = 0; i < sizeData; i++ ){
		*(((float*)pValue) + i) = *(rawData + i) * *(((float*)&factors) + i);
	}


}

void BatteryTester_ConversionData_calcPhisicValueFromAdcCodeEx(
		volatile uint32_t* rawData, uint32_t sizeDataEx, volatile sphisicValueEx_t* pValue){
	if(!rawData || !pValue){
			return;
	}
	uint32_t sizeData = sizeDataEx - 1;
	for(int i = 0; i < sizeData - 1; i++ ){
		*(((float*)pValue) + i) = *(rawData + i) * *(((float*)&factors) + i);
	}
	BatteryTester_ConversionData_calcPhisicTemperatureEx(sizeDataEx, pValue);
	pValue->AvaregeTemps =
			(pValue->temp1IndegC +
			pValue->temp2IndegC +
			pValue->temp3IndegC +
			pValue->temp4IndegC) / 4.0;
}

/*@brief:
 * Initializes internal scaling structures.
 * So far, pseudo-initialization for testing
 * has been performed here.
 */

void BatteryTester_ConversionData_initScaleFactorsStruct(){
	uint32_t size = sizeof(factors) / sizeof(float);
	for(int i = 0; i < size; i++ ){
		*(((float*)&factors) + i) = VREF / RESOLUTION;
		}


}

inline void BatteryTester_ConversionData_calcPhisicTemperatureEx(
		uint32_t sizeDataEx,
		volatile sphisicValueEx_t* pValue){
	/*if(!pValue && sizeDataEx <= (NUM_TEMPERATURE + 1)){// end item to be AvaregeTemps
		return;
	}
	const uint32_t size = sizeDataEx - NUM_TEMPERATURE;
	for(int i = --sizeDataEx, j = 0; i >= size; i--, j++ ){
		*(((float*)pValue) + i) = 1 /
		(1 / ntcParams[j].temperatureOrigIndegC +
		1 / ntcParams[j].factorB *
		log(*(((float*)pValue) + i) /
		(RESOLUTION -1 - *(((float*)pValue) + i)) *
		ntcParams[j].resistanceUpInOhm / ntcParams[j].resistanceOrigInOhm));
	}*/
}

ntcSchemeParameters_t BatteryTester_ConversionData_getNtcSchemeParams(unsigned short index){
	if(index >= NUM_TEMPERATURE){
		ntcSchemeParameters_t bad = {-1.0};
		return bad;
	}
	return ntcParams[index];
}

void BatteryTester_ConversionData_setNtcSchemeParams(unsigned short index, ntcSchemeParameters_t* newParams){
	if(index >= NUM_TEMPERATURE){
		return;
	}
	ntcParams[index] = *newParams;
}

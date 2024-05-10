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
#define RESOLUTION 65535
#define NUM_TEMPERATURE 4
#define DAC_RESOLUTION  4095

/*@deprecated*/
static sscaleFactors_t factors;

static sphisicValueEx_t value ={0};
static sMinValueFromRange_t minValue = {0};
static sMaxValueFromRange_t maxValue = {0};
static sValueScale_t scale = {0};
static sValueCalibrationOffset_t offset = {0};
//volatile uint32_t dataAdc[10];
static ntcSchemeParameters_t ntcParams[NUM_TEMPERATURE];
BatteryTester_ConversionData_setDACCallback_t setDacCallback = 0;
static float valueRefOffsetInVolts = 2.7;

/****************how use***************************
 * Define the callback function
 * BatteryTester_ConversionData_initCallback_t
 * in the main function in which to configure the
 * necessary hardware.
 * Namely: ADC in DMA mode and DAC and call the decorator
 * function BatteryTester_ConversionData_initDecorator()
 * Next, in interruption from DMA, call the function
 * BatteryTester_ConversionData_calcPhisicValueFromAdcCodeEx()
 * Define a callback to set the DAC value
 * BatteryTester_ConversionData_setDacDecorator()
 * The following DAC parameters are accepted here:
 * 			VREF = 3.0
 * 			DAC_RESOLUTION = 4095
 * for other parameters you need to change the values.
 */

/*
 * @deprecated
 * */
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
		volatile uint32_t* rawData, uint32_t sizeDataEx){
	const unsigned int sizeData = sizeof(sphisicValueEx_t) / sizeof(float) - 1;
	if(!rawData || sizeDataEx < sizeData){
		return;
	}
	const unsigned int size = sizeof(sValueScale_t) / sizeof(float);
	for(int i = 0; i < size; i++ ){
		*(((float*)&value) + i) = rawData[i] * *(((float*)&scale) + i) + *(((float*)&offset) + i);
	}
	BatteryTester_ConversionData_calcPhisicTemperatureEx(rawData, sizeDataEx);
	value.AvaregeTemps =
			(value.temp1IndegC +
			value.temp2IndegC +
			value.temp3IndegC +
			value.temp4IndegC) / 4.0;
}

/*@brief:
 * Initializes internal scaling structures.
 * So far, pseudo-initialization for testing
 * has been performed here.
 */

void BatteryTester_ConversionData_initScaleFactorsStruct(){
	/*uint32_t size = sizeof(factors) / sizeof(float);
	for(int i = 0; i < size; i++ ){
		*(((float*)&factors) + i) = VREF / RESOLUTION;
		}*/
	/*minValue.minTemp1InDegC =
			minValue.minTemp2InDegC =
					minValue.minTemp3InDegC =
							minValue.minTemp4InDegC = -30.0;*/

	maxValue.ch1_maxCurrentInAmp =
			maxValue.ch2_maxCurrentInAmp = 18.0;
	maxValue.ch1_maxVoltageInVolt =
			maxValue.ch2_maxVoltageInVolt = 3.6;
	maxValue.maxBusVoltageInVolt = 24.5;
	/*maxValue.maxTemp1InDegC =
			maxValue.maxTemp2InDegC =
					maxValue.maxTemp3InDegC =
							maxValue.maxTemp4InDegC = 300.0;*/

	for(unsigned short i = 0; i < NUM_TEMPERATURE; i++){
		ntcParams[i].factorB = 3950.0; //ppm/degC
		ntcParams[i].resistanceOrigInOhm = 100000.0;
		ntcParams[i].resistanceUpInOhm = 10000.0;
		ntcParams[i].temperatureOrigIndegC = 25.0;
	}

}

void BatteryTester_ConversionData_calcScale(void){
	unsigned int size = sizeof(sValueScale_t) / sizeof(float);
	for(unsigned int i = 0; i < size; i++){
		*(((float*)&scale) + i) = (*(((float*)&maxValue) + i) - *(((float*)&minValue) + i)) / RESOLUTION;
	}
}

inline void BatteryTester_ConversionData_calcPhisicTemperatureEx(
		volatile uint32_t* pValue, uint32_t sizeDataEx){
	const unsigned int sizeData = sizeof(sphisicValueEx_t) / sizeof(float) - 1;
	if(!pValue || sizeDataEx < sizeData){// end item to be AvaregeTemps
		return;
	}

	const uint32_t size = sizeDataEx - NUM_TEMPERATURE;
	for(int i = --sizeDataEx, j = 0; i >= size; i--, j++ ){
		*(((float*)&value) + i) = 1 /
		(1 / ntcParams[j].temperatureOrigIndegC +
		1 / ntcParams[j].factorB *
		log(*(((float*)pValue) + i) /
		(RESOLUTION - *(pValue + i)) *
		ntcParams[j].resistanceUpInOhm / ntcParams[j].resistanceOrigInOhm));
		*(((float*)&value) + i) += *(((float*)&offset) + i);
	}
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
//	ntcParams[index] = *newParams;
	__BatteryTester_AuxiliaryFunction_copy(newParams, ntcParams  + index,
			sizeof(ntcSchemeParameters_t) / sizeof(float));
}

float BatteryTester_ConversionData_getRefOffsetInVolts(){
	return valueRefOffsetInVolts;
}

void BatteryTester_ConversionData_setRefOffsetInVolts(float newValue){
	valueRefOffsetInVolts = newValue;
	if(setDacCallback){
		setDacCallback(BatteryTester_ConversionData_calcRefOffsetDacCode(newValue));
	}
}

unsigned short BatteryTester_ConversionData_calcRefOffsetDacCode(float valueInVolts){
	return valueInVolts * DAC_RESOLUTION / VREF;
}

HAL_StatusTypeDef BatteryTester_ConversionData_initDecorator(
		BatteryTester_ConversionData_initCallback_t initCallback){
	HAL_StatusTypeDef res = BatteryTester_ConversionData_readDataFromEEPROM();
	if (res != HAL_OK){
//		return res;
		BatteryTester_ConversionData_initScaleFactorsStruct();
	}
	BatteryTester_ConversionData_calcScale();
	if(!initCallback){
		return HAL_ERROR;
	}
	res = initCallback();
	if(res != HAL_OK){
		return res;
	}
	return HAL_OK;
}

/*@brief:
 * @Todo: Implementation required
 */
HAL_StatusTypeDef BatteryTester_ConversionData_readDataFromEEPROM(){
	return HAL_OK;
}

void BatteryTester_ConversionData_setDacDecorator(BatteryTester_ConversionData_setDACCallback_t callback){
	setDacCallback = callback;
}

sMinValueFromRange_t BatteryTester_ConversionData_getMinValueFromRange(){
	return minValue;
}

void BatteryTester_ConversionData_setMinValueFromRange(sMinValueFromRange_t* newValue){
	if(!newValue){
		return;
	}
	//minValue = *newValue;
	__BatteryTester_AuxiliaryFunction_copy(newValue, &minValue,
			sizeof(sMinValueFromRange_t) / sizeof(float));
}

sMaxValueFromRange_t BatteryTester_ConversionData_getMaxValueFromRange(){
	return maxValue;
}

void BatteryTester_ConversionData_setMaxValueFromRange(sMaxValueFromRange_t* newValue){
	if(!newValue){
		return;
	}
	//maxValue = *newValue;
	__BatteryTester_AuxiliaryFunction_copy(newValue, &maxValue,
			sizeof(sMaxValueFromRange_t) / sizeof(float));
}

sValueCalibrationOffset_t BatteryTester_ConversionData_getValueCalibrationOffset(){
	return offset;
}

void BatteryTester_ConversionData_setValueCalibrationOffset(sValueCalibrationOffset_t* newValue){
	if(!newValue){
		return;
	}
	//offset = *newValue;
	__BatteryTester_AuxiliaryFunction_copy(newValue, &offset,
			sizeof(sValueCalibrationOffset_t) / sizeof(float));
}

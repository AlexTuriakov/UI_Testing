/*
 * conversion_data.c
 *
 *  Created on: Apr 8, 2024
 *      Author: Tyuryakov_OA
 */
#include <stdio.h>
#include "conversion_data.h"
#include "flash_operation.h"
#include <math.h>

#define TESTING
#define STEP_DECIMATION 100

/*****private typedef*****/
typedef struct{
	float RupDivRo;
	float reciprocalTo;
} sStanehartHart_t;

typedef struct{
	unsigned int temp1;
	unsigned int temp2;
	unsigned int temp3;
	unsigned int temp4;
	unsigned int counter;
} sTempAccumulator_t;

/*@deprecated*/
static sscaleFactors_t factors;

static sphisicValueEx_t value ={0};
static sMinValueFromRange_t minValue = {0};
static sMaxValueFromRange_t maxValue = {0};
static sValueScale_t scale = {0};
static sValueCalibrationOffset_t offset = {0};
static sTempAccumulator_t tempAccum = {0};
static sTempAccumulator_t accum = {0};
//volatile uint32_t dataAdc[10];
static ntcSchemeParameters_t ntcParams[NUM_TEMPERATURE];
static sStanehartHart_t shFactors[NUM_TEMPERATURE];
static BatteryTester_ConversionData_setDACCallback_t g_setDacCallback = 0;
static BatteryTester_ConversionData_initCallback_t  g_initCallback = 0;
static BatteryTester_ConversionData_stopCallback_t g_stopCallback = 0;
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

/*@deprecated
 * very slow
 */
sphisicValueEx_t BatteryTester_ConversionData_calcPhisicValueFromAdcCodeEx(
		volatile uint32_t* rawData, uint32_t sizeDataEx){
	const unsigned int sizeData = sizeof(sphisicValueEx_t) / sizeof(float) - 1;
	if(!rawData || sizeDataEx < sizeData){
		return value;
	}
	const unsigned int size = sizeof(sValueScale_t) / sizeof(float);
	for(int i = 0; i < size; i++ ){
		*(((float*)&value) + i) = rawData[i] * *(((float*)&scale) + i) + *(((float*)&offset) + i);
	}
	BatteryTester_ConversionData_calcPhisicTemperatureEx(rawData, sizeDataEx);
	value.AverageTemps =
			(value.temp1IndegC +
			value.temp2IndegC +
			value.temp3IndegC +
			value.temp4IndegC) / 4.0;
	return value;
}



/*@brief:
 * It didn't get any better.
 * Calculating 4 logarithms at a core frequency
 * of 168 MHz with hardware support FPU
 * takes from 60 to 100 microseconds.
 */
sphisicValueEx_t* BatteryTester_ConversionData__calcPhisicValueFromAdcCodeEx(
		volatile uint32_t* rawData, uint32_t sizeDataEx){

	value.ch1_CurrentInA = rawData[0] * scale.ch1_currentScale + offset.ch1_CurrentInA;
	value.ch2_CurrentInA = rawData[1] * scale.ch2_currentScale + offset.ch2_CurrentInA;
	value.ch1_VoltageInV = rawData[2] * scale.ch1_voltageScale + offset.ch1_VoltageInV;
	value.ch2_VoltageInV = rawData[3] * scale.ch2_voltageScale + offset.ch2_VoltageInV;
	value.busVoltageInV = rawData[4] * scale.busVoltageScale + offset.busVoltageInV;

	if(tempAccum.counter > STEP_DECIMATION){
		accum = tempAccum;
		tempAccum.temp1 = rawData[5];
		tempAccum.temp2 = rawData[6];
		tempAccum.temp3 = rawData[7];
		tempAccum.temp4 = rawData[8];
		tempAccum.counter = 1;
	}
	else{
		tempAccum.temp1 += rawData[5];
		tempAccum.temp2 += rawData[6];
		tempAccum.temp3 += rawData[7];
		tempAccum.temp4 += rawData[8];
		tempAccum.counter++;
	}

	return &value;
}

void BatteryTester_ConversionData_processTemperatureDecimator(){
	if(accum.counter > STEP_DECIMATION){
		float temp = shFactors[0].RupDivRo / (RESOLUTION / ((float)accum.temp1 / STEP_DECIMATION) - 1.0f);
		temp =log(temp) / ntcParams[0].factorB + shFactors[0].reciprocalTo;
		temp = 1.0f / temp + offset.temp1IndegC;
		value.temp1IndegC = (float)temp;
		float acc = temp;

		temp = shFactors[1].RupDivRo / (RESOLUTION / ((float)accum.temp2 / STEP_DECIMATION) - 1.0f);
		temp =log(temp) / ntcParams[1].factorB + shFactors[1].reciprocalTo;
		temp = 1.0f / temp + offset.temp2IndegC;
		value.temp2IndegC = (float)temp;
		acc += temp;

		temp = shFactors[2].RupDivRo / (RESOLUTION / ((float)accum.temp3 / STEP_DECIMATION) - 1.0f);
		temp =log(temp) / ntcParams[2].factorB + shFactors[2].reciprocalTo;
		temp = 1.0f / temp + offset.temp3IndegC;
		value.temp3IndegC = (float)temp;
		acc += temp;

		temp = shFactors[3].RupDivRo / (RESOLUTION / ((float)accum.temp4 / STEP_DECIMATION) - 1.0f);
		temp =log(temp) / ntcParams[3].factorB + shFactors[3].reciprocalTo;
		temp = 1.0f / temp + offset.temp4IndegC;
		value.temp4IndegC = (float)temp;
		acc += temp;

		value.AverageTemps = acc / 4.0f;

		accum.counter = 0;
	}
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
	minValue.ch1_minCurrentInAmp =
			minValue.ch2_minCurrentInAmp = -18.0;

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
		ntcParams[i].resistanceOrigInOhm = 10000.0;
		ntcParams[i].resistanceUpInOhm = 10000.0;
		ntcParams[i].temperatureOrigIndegC = 25.0;
	}

	/*offset.temp1IndegC =
			offset.temp2IndegC =
					offset.temp3IndegC =
							offset.temp4IndegC = 0.0;*/
}

void BatteryTester_ConversionData_calcScale(void){
	unsigned int size = sizeof(sValueScale_t) / sizeof(float);
	for(unsigned int i = 0; i < size; i++){
		*(((float*)&scale) + i) = (*(((float*)&maxValue) + i) - *(((float*)&minValue) + i)) / RESOLUTION;
	}
	for(int i = 0; i < NUM_TEMPERATURE; i++){
		shFactors[i].RupDivRo = ntcParams[i].resistanceUpInOhm / ntcParams[i].resistanceOrigInOhm;
		shFactors[i].reciprocalTo = 1.0f / ntcParams[i].temperatureOrigIndegC;
	}
}


/*@deprecated
 * very slow
 */
inline void BatteryTester_ConversionData_calcPhisicTemperatureEx(
		volatile uint32_t* pValue, uint32_t sizeDataEx){
	const unsigned int sizeData = sizeof(sphisicValueEx_t) / sizeof(float) - 1;
	if(!pValue || sizeDataEx < sizeData){// end item to be AvaregeTemps
		return;
	}
	float temp;
	const uint32_t size = sizeDataEx - NUM_TEMPERATURE;
	for(int i = --sizeDataEx, j = NUM_TEMPERATURE - 1; i >= size; i--, j-- ){
		temp = ntcParams[j].resistanceUpInOhm / ntcParams[j].resistanceOrigInOhm;
		temp /= (float)RESOLUTION / pValue[i] - 1.0f;
		temp = log(temp) / ntcParams[j].factorB + 1.0f / (ntcParams[j].temperatureOrigIndegC/* + 273.15f*/);
		temp = 1.0f / temp/* - 273.15f*/ + *(((float*)&offset) + i);
		*(((float*)&value) + i) = temp;
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
	if(newValue < 0 || newValue > VREF){
		return;
	}
	valueRefOffsetInVolts = newValue;
}

unsigned short BatteryTester_ConversionData_calcRefOffsetDacCode(float valueInVolts){
	return valueInVolts * DAC_RESOLUTION / VREF;
}

HAL_StatusTypeDef BatteryTester_ConversionData_initDecorator(
		BatteryTester_ConversionData_initCallback_t initCallback,
		BatteryTester_ConversionData_setDACCallback_t setDacCallback,
		 BatteryTester_ConversionData_stopCallback_t stopCallback){
	if(!BatteryTester_ConversionData_readDataFromEEPROM()){
		BatteryTester_ConversionData_initScaleFactorsStruct();
	}
	BatteryTester_ConversionData_calcScale();
	g_initCallback = initCallback;
	if(!g_initCallback){
		return HAL_ERROR;
	}
	g_initCallback();

	g_setDacCallback = setDacCallback;
	if(!g_setDacCallback){
		return HAL_ERROR;
	}
	//shutting down st-link?
	BatteryTester_ConversionData_setDACRefOffset(valueRefOffsetInVolts);
	g_stopCallback = stopCallback;
	return HAL_OK;
}

/*@brief:
 * @Todo: Implementation required
 */
eBool_t BatteryTester_ConversionData_readDataFromEEPROM(){
#ifdef TESTING

#endif
	return BatteryTester_EEPROM_readSetMeasurement(
			&minValue, &maxValue, &offset, ntcParams, &valueRefOffsetInVolts);
}

void BatteryTester_ConversionData_setDacDecorator(BatteryTester_ConversionData_setDACCallback_t callback){
	g_setDacCallback = callback;
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

void BatteryTester_ConversionData_startHardware(void){
	if(!g_initCallback){
		return;
	}
	g_initCallback();
}

void BatteryTester_ConversionData_stopHardware(void){
	if(!g_stopCallback){
		return;
	}
	g_stopCallback();
}

void BatteryTester_ConversionData_setDACRefOffset(float newValue){
	if(!g_setDacCallback){
		return;
	}
	BatteryTester_ConversionData_setRefOffsetInVolts(newValue);
	g_setDacCallback(BatteryTester_ConversionData_calcRefOffsetDacCode(newValue));
}

sphisicValueEx_t BatteryTester_ConversionData_getPhisicValues(){
	return value;
}

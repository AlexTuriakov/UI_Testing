/*
 * conversion_data.h
 *
 *  Created on: Apr 8, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CONVERSION_DATA_H_
#define INC_CONVERSION_DATA_H_

/*
 * @deprecated
 * */
typedef struct _sphisicValue_t{
	float val1;
	float val2;
	float val3;
	float val4;
	float val5;
	float val6;
	float val7;
	float val8;
	float val9;
	float val10;
} sphisicValue_t;


/*
 * @deprecated
 * */
typedef struct _sscaleFactors_t{
	float fac1;
	float fac2;
	float fac3;
	float fac4;
	float fac5;
	float fac6;
	float fac7;
	float fac8;
	float fac9;
	float fac10;
} sscaleFactors_t;

typedef struct _sphisicValueEx_t{
	float ch1_CurrentInA;
	float ch2_CurrentInA;
	float busVoltageInV;
	float ch1_VoltageInV;
	float ch2_VoltageInV;
	float temp1IndegC;
	float temp2IndegC;
	float temp3IndegC;
	float temp4IndegC;
	float AvaregeTemps;
} sphisicValueEx_t;

typedef struct _ntcSchemeParameters_t{
	float resistanceUpInOhm;
	float resistanceOrigInOhm;
	float temperatureOrigIndegC;
	float factorB;
} ntcSchemeParameters_t;

/*
 * @deprecated
 * */
typedef struct _sValue_t{
	float ch1_currentInAmp;
	float ch2_currentInAmp;
	float busVoltageInVolt;
	float ch1_voltageInVolt;
	float ch2_voltageInVolt;
	float temp1InDegC;
	float temp2InDegC;
	float temp3InDegC;
	float temp4InDegC;
} sValue_t;

typedef struct _sMinValueFromRange_t{
	float ch1_minCurrentInAmp;
	float ch2_minCurrentInAmp;
	float minBusVoltageInVolt;
	float ch1_minVoltageInVolt;
	float ch2_minVoltageInVolt;
	/*float minTemp1InDegC;
	float minTemp2InDegC;
	float minTemp3InDegC;
	float minTemp4InDegC;*/
} sMinValueFromRange_t;

typedef struct _sMaxValueFromRange_t{
	float ch1_maxCurrentInAmp;
	float ch2_maxCurrentInAmp;
	float maxBusVoltageInVolt;
	float ch1_maxVoltageInVolt;
	float ch2_maxVoltageInVolt;
	/*float maxTemp1InDegC;
	float maxTemp2InDegC;
	float maxTemp3InDegC;
	float maxTemp4InDegC;*/
} sMaxValueFromRange_t;

typedef struct _sValueScale_t{
	float ch1_currentScale;
	float ch2_currentScale;
	float busVoltageScale;
	float ch1_voltageScale;
	float ch2_voltageScale;
	/*float temp1Scale;
	float temp2Scale;
	float temp3Scale;
	float temp4Scale;*/
} sValueScale_t;

typedef struct _sValueCalibrationOffset{
	float ch1_CurrentInA;
	float ch2_CurrentInA;
	float busVoltageInV;
	float ch1_VoltageInV;
	float ch2_VoltageInV;
	float temp1IndegC;
	float temp2IndegC;
	float temp3IndegC;
	float temp4IndegC;
} sValueCalibrationOffset_t;

typedef HAL_StatusTypeDef (*BatteryTester_ConversionData_initCallback_t)(void);
typedef void (*BatteryTester_ConversionData_setDACCallback_t)(unsigned int);

void BatteryTester_ConversionData_setDacDecorator(BatteryTester_ConversionData_setDACCallback_t);
void BatteryTester_ConversionData_calcPhisicValueFromAdcCode(
		volatile uint32_t*, uint32_t, volatile sphisicValue_t*);
void BatteryTester_ConversionData_calcPhisicValueFromAdcCodeEx(
		volatile uint32_t*, uint32_t);
void BatteryTester_ConversionData_initScaleFactorsStruct();
void BatteryTester_ConversionData_calcPhisicTemperatureEx(
		volatile uint32_t*, uint32_t);
ntcSchemeParameters_t BatteryTester_ConversionData_getNtcSchemeParams(
		unsigned short);
void BatteryTester_ConversionData_setNtcSchemeParams(
		unsigned short,
		ntcSchemeParameters_t*);
float BatteryTester_ConversionData_getRefOffsetInVolts();
void BatteryTester_ConversionData_setRefOffsetInVolts(float);
unsigned short BatteryTester_ConversionData_calcRefOffsetDacCode(float);
void BatteryTester_ConversionData_calcScale(void);
HAL_StatusTypeDef BatteryTester_ConversionData_readDataFromEEPROM();
sMinValueFromRange_t BatteryTester_ConversionData_getMinValueFromRange();
void BatteryTester_ConversionData_setMinValueFromRange(sMinValueFromRange_t*);
sMaxValueFromRange_t BatteryTester_ConversionData_getMaxValueFromRange();
void BatteryTester_ConversionData_setMaxValueFromRange(sMaxValueFromRange_t*);
sValueCalibrationOffset_t BatteryTester_ConversionData_getValueCalibrationOffset();
void BatteryTester_ConversionData_setValueCalibrationOffset(sValueCalibrationOffset_t*);

#endif /* INC_CONVERSION_DATA_H_ */

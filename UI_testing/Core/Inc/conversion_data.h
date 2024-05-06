/*
 * conversion_data.h
 *
 *  Created on: Apr 8, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_CONVERSION_DATA_H_
#define INC_CONVERSION_DATA_H_

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

void BatteryTester_ConversionData_calcPhisicValueFromAdcCode(
		volatile uint32_t*, uint32_t, volatile sphisicValue_t*);

void BatteryTester_ConversionData_calcPhisicValueFromAdcCodeEx(
		volatile uint32_t*, uint32_t, volatile sphisicValueEx_t*);

void BatteryTester_ConversionData_initScaleFactorsStruct();

void BatteryTester_ConversionData_calcPhisicTemperatureEx(
		uint32_t,
		volatile sphisicValueEx_t*);

ntcSchemeParameters_t BatteryTester_ConversionData_getNtcSchemeParams(
		unsigned short);

void BatteryTester_ConversionData_setNtcSchemeParams(
		unsigned short,
		ntcSchemeParameters_t*);
float BatteryTester_ConversionData_getRefOffsetInVolts();
void BatteryTester_ConversionData_setRefOffsetInVolts(float);
unsigned short BatteryTester_ConversionData_calcRefOffsetDacCode(float);
#endif /* INC_CONVERSION_DATA_H_ */

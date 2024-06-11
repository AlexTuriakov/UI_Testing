/*
 * set_param.c
 *
 *  Created on: May 7, 2024
 *      Author: Tyuryakov_OA
 *
 *   This implementation involves arithmetic operations with the value,
 *   rather than simply changing the value of the digit.
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "set_param.h"
#include "lcd_wc1602a.h"
#include "state_events.h"

#define SET_PARAM_ACCURACY 6
#define SET_PARAM_SIZE 11

/*@brief^
 * using float gives an error in the low-order bits.
 * */
//static double settingParam;
static short currentOrder;
static sParamSets_t currentParameter;

static inline void _BatteryTester_SetParam_display();
static inline void _BatteryTester_moveCursor();

/*
 * @deprecated
 */
/*void BatteryTester_SetParam_init(float initParam){
	settingParam = initParam;
	currentOrder = -6;
	_BatteryTester_SetParam_display();
	BatteryTester_WC1602A_Setpos(1, 10);
	BatteryTester_WC1602A_onCursor();
}*/

void BatteryTester_SetParam_init(sParamSets_t init){
	currentParameter = init;
	currentOrder = -currentParameter.decimalOrder;
	_BatteryTester_SetParam_display();
	BatteryTester_WC1602A_Setpos(1,
			currentParameter.decimalOrder + currentParameter.integerOrder + 1);
	BatteryTester_WC1602A_onCursor();
}

float BatteryTester_SetParam_getParam(){
//	return settingParam;
	return currentParameter.value;
}

/*
 * @deprecated
 */
/*void BatteryTester_SetParam_setParam(float param){
	settingParam = param;
}*/

void BatteryTester_SetParam_setParam(sParamSets_t param){
	currentParameter = param;
}

void BatteryTester_SetParam_ok(){
	BatteryTester_WC1602A_offCursor();
	BatteryTester_WC1602A_clearDisplay();
	BatteryTester_State_returnFromState();
}

/*
 * @deprecated
 */
/*inline void _BatteryTester_SetParam_display(){
	char str[17];
	str[0]='\0';
	sprintf(str, "%+011.6f", settingParam);
	str[16] = '\0';
	BatteryTester_WC1602A_writeLine(1, str, strlen(str));
	_BatteryTester_moveCursor();
}*/

static inline void _BatteryTester_SetParam_display(){
	char str[20];// becase warnings compiler
	char buf[17];
	snprintf(str, 20, "%%+0%d.%df",
			(currentParameter.integerOrder + currentParameter.decimalOrder + 2),
			currentParameter.decimalOrder);
	snprintf(buf, 17, str, currentParameter.value);
	BatteryTester_WC1602A_writeLine(1, buf, strlen(buf));
	_BatteryTester_moveCursor();
}

static inline void _BatteryTester_moveCursor(){
	if(currentOrder <= 0){
		if(currentOrder == 0){
			BatteryTester_WC1602A_Setpos(1, currentParameter.integerOrder);
		}
		else{
			BatteryTester_WC1602A_Setpos(1,
					currentParameter.integerOrder - currentOrder + 1);
		}
	}
	else{
		if(currentOrder == currentParameter.integerOrder + 1){
				BatteryTester_WC1602A_Setpos(1, 0);
		}
		else{
			BatteryTester_WC1602A_Setpos(1,
					currentParameter.integerOrder - currentOrder);
		}
	}
}

void BatteryTester_SetParam_left(){
	if(currentParameter.sign){
		if(currentOrder < currentParameter.integerOrder){
			currentOrder++;
			_BatteryTester_moveCursor();
		}
	}
	else{
		if(currentOrder < currentParameter.integerOrder - 1){
			currentOrder++;
			_BatteryTester_moveCursor();
		}
	}

}

void BatteryTester_SetParam_right(){
	if(currentOrder > -currentParameter.decimalOrder){
		currentOrder--;
		_BatteryTester_moveCursor();
	}
}

void BatteryTester_SetParam_up(){
	double temp = currentParameter.value;
	if(currentOrder == currentParameter.integerOrder){
		temp = currentParameter.value;
		temp = -temp;
		if(temp >= currentParameter.minValue && temp <= currentParameter.maxValue)
			currentParameter.value = temp;
	}
	else{
		temp += pow(10, currentOrder);
		if(temp <= currentParameter.maxValue){
			currentParameter.value = temp;
		}
		else{
			currentParameter.value = currentParameter.maxValue;
		}
	}
	_BatteryTester_SetParam_display();
}

void BatteryTester_SetParam_down(){
	double temp = currentParameter.value;
	if(currentOrder == currentParameter.integerOrder){
		temp = currentParameter.value;
		temp = -temp;
		if(temp >= currentParameter.minValue && temp <= currentParameter.maxValue)
			currentParameter.value = temp;
	}
	else{
		temp -= pow(10, currentOrder);
		if(temp >= currentParameter.minValue){
			currentParameter.value = temp;
		}
		else{//reset error float value
			if(currentOrder == -currentParameter.decimalOrder)
				currentParameter.value = currentParameter.minValue;
		}
	}
	_BatteryTester_SetParam_display();
}

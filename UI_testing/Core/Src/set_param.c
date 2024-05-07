/*
 * set_param.c
 *
 *  Created on: May 7, 2024
 *      Author: Tyuryakov_OA
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "set_param.h"
#include "lcd_wc1602a.h"

#define SET_PARAM_ACCURACY 6
#define SET_PARAM_SIZE 11

static float settingParam;
static short currentOrder;

void BatteryTester_SetParam_init(float initParam){
	settingParam = initParam;
	currentOrder = -6;
	BatteryTester_SetParam_display();
	BatteryTester_WC1602A_Setpos(1, 10);
	BatteryTester_WC1602A_onCursor();
}

float BatteryTester_SetParam_getParam(){
	return settingParam;
}

void BatteryTester_SetParam_setParam(float param){
	settingParam = param;
}

void BatteryTester_SetParam_ok(){
	BatteryTester_WC1602A_offCursor();
	BatteryTester_WC1602A_clearDisplay();
	BatteryTester_State_returnFromState();
}

inline void BatteryTester_SetParam_display(){
	char str[17];
	str[0]='\0';
	sprintf(str, "%+011.6f", settingParam);
	str[16] = '\0';
	BatteryTester_WC1602A_writeLine(1, str, strlen(str));
	BatteryTester_moveCursor();
}

inline void BatteryTester_moveCursor(){
	if(currentOrder < 0){
		BatteryTester_WC1602A_Setpos(1, 4 - currentOrder - 1);
	}
	else{
		BatteryTester_WC1602A_Setpos(1, 4 - currentOrder);
	}
}

void BatteryTester_SetParam_left(){
	if(currentOrder < 3){
		currentOrder++;
		BatteryTester_moveCursor();
	}
	else{
		currentOrder = 4;
	}
}

void BatteryTester_SetParam_right(){
	if(currentOrder > -6){
		currentOrder--;
		BatteryTester_moveCursor();
	}
}

void BatteryTester_SetParam_up(){
	float temp = settingParam;
	if(currentOrder == 4){
		settingParam *= -1;
	}
	else{
		temp += pow(10, currentOrder);
		settingParam = (temp <= 9999.999999)? temp: settingParam;
	}
	BatteryTester_SetParam_display();
}

void BatteryTester_SetParam_down(){
	float temp = settingParam;
	if(currentOrder == 4){
		settingParam *= 1;
	}
	else{
		temp -= pow(10, currentOrder);
		settingParam = (temp >= -9999.999999)? temp: settingParam;
	}
	BatteryTester_SetParam_display();
}

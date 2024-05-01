/*
 * state_events.c
 *
 *  Created on: Apr 29, 2024
 *      Author: Tyuryakov_OA
 */
#include "state_events.h"
#include <string.h>
#include "auxiliary_function.h"


	static eStates_t currentState;
	static eEvents_t currentEvent;
	static void* currentParams;
	char strPostParam[SIZE_LINE_BUFFER_LCD] = "";
	float newSendParam = 0.0;
	static _handlerState_t handlerStates[NUM_STATES] = {
		BatteryTester_State_handlerMenuNavigate,
		BatteryTester_State_handlerSetParams,
		BatteryTester_State_handlerWork,
		BatteryTester_State_handlerAlarm
};

void BatteryTester_State_initState(){
	currentState = MENU_NAVIGATE;
	currentEvent = EVENT_NONE;
	currentParams = 0;
}

void BatteryTester_State_proccessEvents(){
	switch(currentState){
	case MENU_NAVIGATE:
		BatteryTester_State_handlerMenuNavigate(currentEvent , (void*)0);
		break;
	case SET_PARAMETERS:
		BatteryTester_State_handlerSetParams(currentEvent , (void*)0);
		break;
	case WORK:
		BatteryTester_State_handlerWork(currentEvent , (void*)0);
		break;
	case ALARM:
		BatteryTester_State_handlerAlarm(currentEvent , (void*)0);
		break;
	default:
	}
	currentEvent = EVENT_NONE;
}

void BatteryTester_State_handlerMenuNavigate(eEvents_t event, void* param){

}

void BatteryTester_State_handlerSetParams(eEvents_t event, void* param){

}

void BatteryTester_State_handlerWork(eEvents_t event, void* param){

}

void BatteryTester_State_handlerAlarm(eEvents_t event, void* param){

}

eStates_t BatteryTester_State_getCurrentState(){
	return currentState;
}

void BatteryTester_State_setCurrentState(eStates_t newState){
	currentState = newState;
}

eEvents_t BatteryTester_State_getCurrentEvent(){
	return currentEvent;
}

void BatteryTester_State_setCurrentEvent(eEvents_t newEvent){
	currentEvent = newEvent;
}

void* BatteryTester_State_getCurrentParams(){
	return currentParams;
}

void BatteryTester_State_setCurrentParams(void* newParams){
	currentParams = newParams;
}

void BatteryTester_State_postStrParamForState(const char* sParam){
	strncpy(strPostParam, sParam, SIZE_LINE_BUFFER_LCD);
}

float BatteryTester_State_sendNewParamFromState(){
	return newSendParam;
}

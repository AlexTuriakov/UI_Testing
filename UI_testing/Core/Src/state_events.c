/*
 * state_events.c
 *
 *  Created on: Apr 29, 2024
 *      Author: Tyuryakov_OA
 */

/**************How use***********************
 * Very simple.
 * Initialize by running .BatteryTester_State_initState()
 * And in the main cycle visit BatteryTester_State_processState()
 * */
#include "state_events.h"
#include <string.h>
#include "auxiliary_function.h"
#include "lcd_wc1602a.h"
#include "menu.h"
#include "keypad.h"
#include "set_param.h"
#include "work_status.h"


	static eStates_t currentState;
	static eStates_t stateFrom;
	static eEvents_t currentEvent;
	static char flagContinueEvent = 0;

	//char strPostParam[SIZE_LINE_BUFFER_LCD] = "";

	/*static _handlerState_t handlerStates[NUM_STATES] = {
		BatteryTester_State_handlerMenuNavigate,
		BatteryTester_State_handlerSetParams,
		BatteryTester_State_handlerWork,
		BatteryTester_State_handlerAlarm
};*/

void BatteryTester_State_initState(){
	currentState = MENU_NAVIGATE;
	currentEvent = EVENT_NONE;
	BatteryTester_Menu_init();
	BatteryTester_Menu_SetGenericWriteCallback(BatteryTester_State_writeMenuItem);
	BUTTON_Init();
}

void BatteryTester_State_proccessEvents(){
	//handlerStates[currentState](currentEvent , (void*)0);

	switch(currentState){
	case MENU_NAVIGATE:
		BatteryTester_State_handlerMenuNavigate(currentEvent , (void*)0);
		//currentEvent = EVENT_NONE;
		break;
	case SET_PARAMETERS:
		BatteryTester_State_handlerSetParams(currentEvent , (void*)0);
		break;
	case WORK_STATUS:
		BatteryTester_State_handlerWorkStatus(currentEvent , (void*)0);
		break;
	case ALARM:
		BatteryTester_State_handlerAlarm(currentEvent , (void*)0);
		break;
	default:
	}
}

void BatteryTester_State_handlerMenuNavigate(eEvents_t event, void* param){
	switch(event){
	case EVENT_NONE:
		return;
	case EVENT_KEY_OK:
		BatteryTester_Menu_EnterCurrentItem();
		return;
	case EVENT_KEY_UP:
		BatteryTester_Menu_Navigate(MENU_PREVIOUS);
		return;
	case EVENT_KEY_DOWN:
		BatteryTester_Menu_Navigate(MENU_NEXT);
		return;
	case EVENT_KEY_RIGHT:
		BatteryTester_Menu_Navigate(MENU_CHILD);
		return;
	case EVENT_KEY_LEFT:
		BatteryTester_Menu_Navigate(MENU_PARENT);
		return;
	default:
		return;
	}
}

void BatteryTester_State_handlerSetParams(eEvents_t event, void* param){
	switch(event){
		case EVENT_NONE:
			return;
		case EVENT_KEY_OK:
			BatteryTester_SetParam_ok();
			BatteryTester_State_continueEvent(EVENT_KEY_OK);
			return;
		case EVENT_KEY_UP:
			BatteryTester_SetParam_up();
			return;
		case EVENT_KEY_DOWN:
			BatteryTester_SetParam_down();
			return;
		case EVENT_KEY_RIGHT:
			BatteryTester_SetParam_right();
			return;
		case EVENT_KEY_LEFT:
			BatteryTester_SetParam_left();
			return;
		default:
			return;
		}
}

void BatteryTester_State_handlerWorkStatus(eEvents_t event, void* param){
	switch(event){
	case EVENT_NONE:
		BatteryTester_WorkStatus_updateStatus((void*) param);
		break;
	case EVENT_KEY_UP:
		BatteryTester_WorkStatus_upStatus((void*) param);
		break;
	case EVENT_KEY_DOWN:
		BatteryTester_WorkStatus_downStatus((void*) param);
		break;
	case EVENT_KEY_LEFT:
		BatteryTester_WorkStatus_returnToPreviousState((void*) param);
		break;
	default:
		break;
	}
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

void BatteryTester_State_continueEvent(eEvents_t newEvent){ //continueEvent
	BatteryTester_State_setCurrentEvent(newEvent);
	flagContinueEvent = 1;
}

/*void BatteryTester_State_postStrParamForState(const char* sParam){
	strncpy(strPostParam, sParam, SIZE_LINE_BUFFER_LCD);
}*/

void BatteryTester_State_postParamForSet(float param){
	 BatteryTester_SetParam_init(param);
}

float BatteryTester_State_sendNewParamFromState(){
	return BatteryTester_SetParam_getParam();
}

void BatteryTester_State_writeMenuItem(const char* text){
	BatteryTester_WC1602A_clearDisplay();
	char str[17] = "";
	unsigned int size = strlen(text);
	if(size > 16){
		strncpy(str, text, 16);
		str[16] = '\0';
		BatteryTester_WC1602A_writeLine(0, str, 16);
		size = (size - 16 >= 16)? 16: size - 16;
		strncpy(str, text + 16, size);
		str[16] = '\0';
		BatteryTester_WC1602A_writeLine(1, str, size);
	}
	else{
		BatteryTester_WC1602A_writeLine(0, text, size);
	}
}

void BatteryTester_State_moveFromToState(eStates_t from, eStates_t to){
	currentState = to;
	stateFrom = from;
}

void BatteryTester_State_returnFromState(){
	eStates_t temp = currentState;
	currentState = stateFrom;
	stateFrom = temp;
}

void BatteryTester_State_processInputs(){
	BUTTON_ResetActions();
	BUTTON_Process();
	if (BUTTON_GetAction(BUTTON_UP) == BUTTON_VERY_LONG_PRESS)
	{
		//currentEvent = EVENT_KEY_UP;
		currentEvent = EVENT_KEY_OK; // only for testing
		return;
	}
	if (BUTTON_GetAction(BUTTON_DOWN) == BUTTON_VERY_LONG_PRESS)
	{
		currentEvent = EVENT_KEY_DOWN;
		return;
	}
	if (BUTTON_GetAction(BUTTON_RIGHT) == BUTTON_VERY_LONG_PRESS)
	{
		currentEvent = EVENT_KEY_RIGHT;
		return;
	}
	if (BUTTON_GetAction(BUTTON_LEFT) == BUTTON_VERY_LONG_PRESS)
	{
		currentEvent = EVENT_KEY_LEFT;
		return;
	}
	if(!flagContinueEvent){
		currentEvent = EVENT_NONE;
	}
	else{
		flagContinueEvent = 0;
	}

}

void BatteryTester_State_processState(){
	BatteryTester_State_processInputs();
	BatteryTester_State_proccessEvents();
}

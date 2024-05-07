/*
 * state_events.h
 *
 *  Created on: Apr 29, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_STATE_EVENTS_H_
#define INC_STATE_EVENTS_H_

typedef enum {
	MENU_NAVIGATE,
	SET_PARAMETERS,
	WORK,
	ALARM,
	NUM_STATES
} eStates_t;

typedef enum{
	EVENT_NONE,
	EVENT_KEY_OK,
	EVENT_KEY_UP,
	EVENT_KEY_DOWN,
	EVENT_KEY_RIGHT,
	EVENT_KEY_LEFT,
	NUM_EVENTS
} eEvents_t;

typedef void (*_handlerState_t)(eEvents_t, void*);

void BatteryTester_State_initState();
void BatteryTester_State_proccessEvents();
void BatteryTester_State_handlerMenuNavigate(eEvents_t, void*);
void BatteryTester_State_handlerSetParams(eEvents_t, void*);
void BatteryTester_State_handlerWork(eEvents_t, void*);
void BatteryTester_State_handlerAlarm(eEvents_t, void*);
eStates_t BatteryTester_State_getCurrentState();
void BatteryTester_State_setCurrentState(eStates_t);
eEvents_t BatteryTester_State_getCurrentEvent();
void BatteryTester_State_setCurrentEvent(eEvents_t);
void* BatteryTester_State_getCurrentParams();
void BatteryTester_State_setCurrentParams(void*);
//void BatteryTester_State_postStrParamForState(const char*);
void BatteryTester_State_postParamForSet(float);
float BatteryTester_State_sendNewParamFromState();
void BatteryTester_State_writeMenuItem(const char*);
void BatteryTester_State_moveFromToState(eStates_t, eStates_t);
void BatteryTester_State_returnFromState();
void BatteryTester_State_processInputs();
void BatteryTester_State_processState();

#endif /* INC_STATE_EVENTS_H_ */

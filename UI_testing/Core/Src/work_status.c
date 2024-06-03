/*
 * work_status.c
 *
 *  Created on: May 17, 2024
 *      Author: Tyuryakov_OA
 */

/*********declaration of variables***********/


/**********functional implementation**************/
/*Ch1, ch2 buck and boost content status:	Thermostat:		Measuring:		Fault:
 * =============  							=============	=============	================
 * mode: on/off								mode: on/off	Ich1: ..., A	OvV/OvC hardware
 * volt: ..., V								Tavg: ..., oC	Vch1: ..., V	fault: on/off
 * =============							=============	=============	================
 * =============							=============	=============
 * sp: ..., A								sp: ..., A		Ich2: ..., A
 * fb: ..., A								fb: ..., A		Vch2: ..., V
 * =============							=============	=============
 * =============							=============	=============
 * bus: ..., V								T1: ...,oC		Vbus: ..., V
 * prot: on/off								T2: ..., oC		Tavg: ..., oC
 * =============							=============	=============
 * =============							=============	=============
 * fault: on/off							T3: ..., oC		T1: ..., oC
 *											T4: ..., oC		T2: ..., oC
 * =============							=============	=============
 * 															=============
 *															T3: ..., oC
 * 															T4: ..., oC
 * 															=============
*/
#include "work_status.h"
#include "menu.h"
#include "stm32f0xx_hal.h"


static unsigned int tick = 0;


void BatteryTester_WorkStatus_updateStatus(void* param){
	//start timer
	//current menu navigate
	if(HAL_GetTick() - tick >= 500){
		tick = HAL_GetTick();
		BatteryTester_Menu_Navigate(BatteryTester_Menu_GetCurrentMenu());
	}

}

void BatteryTester_WorkStatus_returnToPreviousState(void* param){
	// navigate menu currentmenu -> parent
	BatteryTester_State_returnFromState();
	BatteryTester_Menu_Navigate(MENU_PARENT);
}

void BatteryTester_WorkStatus_upStatus(void* param){
	//navigate menu current menu -> previous
	BatteryTester_Menu_Navigate(MENU_PREVIOUS);
}

void BatteryTester_WorkStatus_downStatus(void* param){
	// navigate menu current menu -> next
	BatteryTester_Menu_Navigate(MENU_NEXT);
}

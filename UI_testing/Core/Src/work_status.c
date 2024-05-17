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
void BatteryTester_WorkStatus_updateStatus(void* param){

}

void BatteryTester_WorkStatus_returnToPreviousState(void* param){

}

void BatteryTester_WorkStatus_upStatus(void* param){

}

void BatteryTester_WorkStatus_downStatus(void* param){

}

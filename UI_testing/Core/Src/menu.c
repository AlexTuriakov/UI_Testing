/* MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.*/
#include "menu.h"
#include "lcd_wc1602a.h"
#include "dessipator_control.h"
#include <stdio.h>
#include <string.h>
#include "auxiliary_function.h"
#include "climat_regulator.h"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t  NULL_MENU = {0};
/*MAIN MENU*/
MENU_ITEM(menuSettings, menuLogs, menuStart, NULL_MENU, menuSetsRefOffset ,
		0, 0, "Settings...");
MENU_ITEM(menuLogs, menuStart, menuSettings, NULL_MENU, NULL_MENU,
		0, 0, "Logs...");
MENU_ITEM(menuStart, menuSettings, menuLogs, NULL_MENU, menuStartMod1,
		0, 0, "Start...");
/*SUBMENU Settings... LEVEL 1*/
MENU_ITEM(menuSetsRefOffset, menuSetsChannel1, menuSetsThermometr, menuSettings, NULL_MENU,
		0, BatteryTester_Menu_enterRefOffset, "Ref. Offset");
MENU_ITEM(menuSetsChannel1, menuSetsChannel2, menuSetsRefOffset, menuSettings, menuCh1Buck,
		0, 0, "Ch1 settings... ");
MENU_ITEM(menuSetsChannel2, menuSetsDessipator, menuSetsChannel1, menuSettings, menuCh1BuckKp,
		0, 0, "Ch2 settings...");
MENU_ITEM(menuSetsDessipator, menuSetsClimat, menuSetsChannel2, menuSettings, menuDissipMin,
		0, 0, "Dessipator...");
MENU_ITEM(menuSetsClimat, menuSetsThermometr, menuSetsDessipator, menuSettings, menuClimReg,
		0, 0, "Thermostat...");
MENU_ITEM(menuSetsThermometr, menuSetsRefOffset, menuSetsClimat, menuSettings, menuT1,
		0, 0, "Thermometers...");
/*SUBMENU Start... LEVEL 1*/
MENU_ITEM(menuStartCh1, menuStartCh2, menuResetAlarm, menuStart, menuStartCh1Sp,
		0, 0, "Ch1 start...");
MENU_ITEM(menuStartCh2, menuStartThermostat, menuStartCh1, menuStart, menuStartCh2Sp,
		0, 0, "Ch2 start...");
MENU_ITEM(menuStartThermostat, menuStartDessipator, menuStartCh2, menuStart, menuStartTstatSp,
		0, 0, "Tstat. start...");
MENU_ITEM(menuStartDessipator, menuResetAlarm, menuStartThermostat, menuStart, menuStartDessipatorOn,
		0, 0, "Dessip. start...");
MENU_ITEM(menuResetAlarm, menuStartCh1, menuStartDessipator, menuStart, menuAlarmState,
		0, 0, "Reset Alarm...");
/*SUBMENU Ch1 settings...  LEVEL 2*/
MENU_ITEM(menuCh1Buck, menuCh1Boost, menuCh1VRange, menuSetsChannel1, menuCh1BuckKp,
		0, 0, "Ch1 buck sets...");
MENU_ITEM(menuCh1Boost, menuCh1BuckPwm, menuCh1Buck, menuSetsChannel1, menuCh1BoostKp,
		0, 0, "Ch1 boost sets...");
MENU_ITEM(menuCh1BuckPwm, menuCh1BoostPwm, menuCh1Boost, menuSetsChannel1, menuCh1BuckPwmMinDuty,
		0, 0, "Ch1 buck pwm...");
MENU_ITEM(menuCh1BoostPwm, menuCh1VRange, menuCh1BuckPwm, menuSetsChannel1, NULL_MENU,
		0, 0, "Ch1 boost pwm...");
MENU_ITEM(menuCh1VRange, menuCh1Buck, menuCh1BoostPwm, menuSetsChannel1, NULL_MENU,
		0, 0, "Ch1 volt rng...");
/*SUBMENU Ch2 settings...  LEVEL 2*/
MENU_ITEM(menuCh2Buck, menuCh2Boost, menuCh2BoostPwm, menuSetsChannel2, NULL_MENU,
		0, 0, "Ch2 buck sets...");
MENU_ITEM(menuCh2Boost, menuCh2BuckPwm, menuCh2Buck, menuSetsChannel2, NULL_MENU,
		0, 0, "Ch2 boost sets...");
MENU_ITEM(menuCh2BuckPwm, menuCh2BoostPwm, menuCh2Boost, menuSetsChannel2, NULL_MENU,
		0, 0, "Ch2 buck pwm...");
MENU_ITEM(menuCh2BoostPwm, menuCh2Buck, menuCh2BuckPwm, menuSetsChannel2, NULL_MENU,
		0, 0, "Ch2 boost pwm...");
MENU_ITEM(menuCh2VRange, menuCh2Buck, menuCh2BoostPwm, menuSetsChannel2, NULL_MENU,
		0, 0, "Ch2 volt rng...");
/*SUBMENU Dessipator...  LEVEL 2*/
MENU_ITEM(menuDissipMin, menuDissipMax, menuDissipMax, menuSetsDessipator, menuSetDessipMin,
		0, 0, "Dessip. min");
MENU_ITEM(menuDissipMax, menuDissipMin, menuDissipMin, menuSetsDessipator, menuSetDessipMax,
		0, 0, "Dessip. max");
/*SUBMENU Thermostat...  LEVEL 2*/
MENU_ITEM(menuClimReg, menuClimPwm, menuClimPwm, menuSetsClimat, NULL_MENU,
		0, 0, "Tstat. reg...");
MENU_ITEM(menuClimPwm, menuClimReg, menuClimReg, menuSetsClimat, NULL_MENU,
		0, 0, "Tstat. pwm...");
/*SUBMENU Thermometers...  LEVEL 2*/
MENU_ITEM(menuT1, menuT2, menuT4, menuSetsThermometr, NULL_MENU,
		0, 0, "Temperature 1...");
MENU_ITEM(menuT2, menuT3, menuT1, menuSetsThermometr, NULL_MENU,
		0, 0, "Temperature 2...");
MENU_ITEM(menuT3, menuT4, menuT2, menuSetsThermometr, NULL_MENU,
		0, 0, "Temperature 3...");
MENU_ITEM(menuT4, menuT1, menuT3, menuSetsThermometr, NULL_MENU,
		0, 0, "Temperature 4...");
/*SUBMENU Ch1 buck sets...  LEVEL 3*/
MENU_ITEM(menuCh1BuckKp, menuCh1BuckKi, menuCh1BuckMaxLim, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Buck Kp");
MENU_ITEM(menuCh1BuckKi, menuCh1BuckKd, menuCh1BuckKp, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Buck Ki");
MENU_ITEM(menuCh1BuckKd, menuCh1BuckDt, menuCh1BuckKi, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Buck Kd");
MENU_ITEM(menuCh1BuckDt, menuCh1BuckSp, menuCh1BuckKd, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Buck dt");
MENU_ITEM(menuCh1BuckSp, menuCh1BuckMinLim, menuCh1BuckDt, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Buck Sp");
MENU_ITEM(menuCh1BuckMinLim, menuCh1BuckMaxLim, menuCh1BuckSp, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Bu minLim");
MENU_ITEM(menuCh1BuckMaxLim, menuCh1BuckKp, menuCh1BuckMinLim, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Bu maxLim");
/*SUBMENU Ch1 boost sets...  LEVEL 3*/
MENU_ITEM(menuCh1BoostKp, menuCh1BoostKi, menuCh1BoostMaxLim, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost Kp");
MENU_ITEM(menuCh1BoostKi, menuCh1BoostKd, menuCh1BoostKp, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost Ki");
MENU_ITEM(menuCh1BoostKd, menuCh1BoostDt, menuCh1BoostKi, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost Kd");
MENU_ITEM(menuCh1BoostDt, menuCh1BoostSp, menuCh1BoostKd, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost dt");
MENU_ITEM(menuCh1BoostSp, menuCh1BoostMinLim, menuCh1BoostDt, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost Sp");
MENU_ITEM(menuCh1BoostMinLim, menuCh1BoostMaxLim, menuCh1BoostSp, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost minLim");
MENU_ITEM(menuCh1BoostMaxLim, menuCh1BoostKp, menuCh1BoostMinLim, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost maxLim");
/*SUBMENU Ch1 buck pwm...  LEVEL 3*/
MENU_ITEM(menuCh1BuckPwmMinDuty, menuCh1BuckPwmMaxDuty, menuCh1BuckPwmDeadTime, menuCh1BuckPwm, NULL_MENU,
		0, 0, "Ch1 Buck minDuty");
MENU_ITEM(menuCh1BuckPwmMaxDuty, menuCh1BuckPwmPeriod, menuCh1BuckPwmMinDuty, menuCh1BuckPwm, NULL_MENU,
		0, 0, "Ch1 Buck maxDuty");
MENU_ITEM(menuCh1BuckPwmPeriod, menuCh1BuckPwmDeadTime, menuCh1BuckPwmMaxDuty, menuCh1BuckPwm, NULL_MENU,
		0, 0, "Ch1 Bck Pwm per");
MENU_ITEM(menuCh1BuckPwmDeadTime, menuCh1BuckPwmMinDuty, menuCh1BuckPwmPeriod, menuCh1BuckPwm, NULL_MENU,
		0, 0, "Ch1 Bck Pwm dead");
/*SUBMENU Ch1 boost pwm...  LEVEL 3*/
MENU_ITEM(menuCh1BoostPwmMinDuty, menuCh1BoostPwmMaxDuty, menuCh1BoostPwmDeadTime, menuCh1BoostPwm, NULL_MENU,
		0, 0, "Ch1 Boost minDut");
MENU_ITEM(menuCh1BoostPwmMaxDuty, menuCh1BoostPwmPeriod, menuCh1BoostPwmMinDuty, menuCh1BoostPwm, NULL_MENU,
		0, 0, "Ch1 Boost maxDut");
MENU_ITEM(menuCh1BoostPwmPeriod, menuCh1BoostPwmDeadTime, menuCh1BoostPwmMaxDuty, menuCh1BoostPwm, NULL_MENU,
		0, 0, "Ch1 Bst Pwm per");
MENU_ITEM(menuCh1BoostPwmDeadTime, menuCh1BuckPwmMinDuty, menuCh1BoostPwmPeriod, menuCh1BoostPwm, NULL_MENU,
		0, 0, "Ch1 Bst Pwm dead");
/*SUBMENU Ch1 volt rng...  LEVEL 3*/
MENU_ITEM(menuCh1CellVoltCtrlMin, menuCh1CellVoltCtrlMax, menuCh1CellVoltCtrlMax, menuCh1VRange, NULL_MENU,
		0, 0, "Ch1 Voltctrl min");
MENU_ITEM(menuCh1CellVoltCtrlMax, menuCh1CellVoltCtrlMin, menuCh1CellVoltCtrlMin, menuCh1VRange, NULL_MENU,
		0, 0, "Ch1 Voltctrl max");
/*SUBMENU Ch2 buck sets...  LEVEL 3*/
MENU_ITEM(menuCh2BuckKp, menuCh2BuckKi, menuCh2BuckMaxLim, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Buck Kp");
MENU_ITEM(menuCh2BuckKi, menuCh2BuckKd, menuCh2BuckKp, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Buck Ki");
MENU_ITEM(menuCh2BuckKd, menuCh2BuckDt, menuCh2BuckKi, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Buck Kd");
MENU_ITEM(menuCh2BuckDt, menuCh2BuckSp, menuCh2BuckKd, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Buck dt");
MENU_ITEM(menuCh2BuckSp, menuCh2BuckMinLim, menuCh2BuckDt, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Buck Sp");
MENU_ITEM(menuCh2BuckMinLim, menuCh2BuckMaxLim, menuCh2BuckSp, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Bu minLim");
MENU_ITEM(menuCh2BuckMaxLim, menuCh2BuckKp, menuCh2BuckMinLim, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Bu maxLim");
/*SUBMENU Ch2 boost sets...  LEVEL 3*/
MENU_ITEM(menuCh2BoostKp, menuCh2BoostKi, menuCh2BoostMaxLim, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost Kp");
MENU_ITEM(menuCh2BoostKi, menuCh2BoostKd, menuCh2BoostKp, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost Ki");
MENU_ITEM(menuCh2BoostKd, menuCh2BoostDt, menuCh2BoostKi, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost Kd");
MENU_ITEM(menuCh2BoostDt, menuCh2BoostSp, menuCh2BoostKd, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost dt");
MENU_ITEM(menuCh2BoostSp, menuCh2BoostMinLim, menuCh2BoostDt, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost Sp");
MENU_ITEM(menuCh2BoostMinLim, menuCh2BoostMaxLim, menuCh2BoostSp, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost minLim");
MENU_ITEM(menuCh2BoostMaxLim, menuCh2BoostKp, menuCh2BoostMinLim, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost maxLim");
/*SUBMENU Ch2 buck pwm...  LEVEL 3*/
MENU_ITEM(menuCh2BuckPwmMinDuty, menuCh2BuckPwmMaxDuty, menuCh2BuckPwmDeadTime, menuCh2BuckPwm, NULL_MENU,
		0, 0, "Ch2 Buck minDuty");
MENU_ITEM(menuCh2BuckPwmMaxDuty, menuCh2BuckPwmPeriod, menuCh2BuckPwmMinDuty, menuCh2BuckPwm, NULL_MENU,
		0, 0, "Ch2 Buck maxDuty");
MENU_ITEM(menuCh2BuckPwmPeriod, menuCh2BuckPwmDeadTime, menuCh2BuckPwmMaxDuty, menuCh2BuckPwm, NULL_MENU,
		0, 0, "Ch2 Bck Pwm per");
MENU_ITEM(menuCh2BuckPwmDeadTime, menuCh2BuckPwmMinDuty, menuCh2BuckPwmPeriod, menuCh2BuckPwm, NULL_MENU,
		0, 0, "Ch2 Bck Pwm dead");
/*SUBMENU Ch2 boost pwm...  LEVEL 3*/
MENU_ITEM(menuCh2BoostPwmMinDuty, menuCh2BoostPwmMaxDuty, menuCh2BoostPwmDeadTime, menuCh2BoostPwm, NULL_MENU,
		0, 0, "Ch2 Boost minDut");
MENU_ITEM(menuCh2BoostPwmMaxDuty, menuCh2BoostPwmPeriod, menuCh2BoostPwmMinDuty, menuCh2BoostPwm, NULL_MENU,
		0, 0, "Ch2 Boost maxDut");
MENU_ITEM(menuCh2BoostPwmPeriod, menuCh2BoostPwmDeadTime, menuCh2BoostPwmMaxDuty, menuCh2BoostPwm, NULL_MENU,
		0, 0, "Ch2 Bst Pwm per");
MENU_ITEM(menuCh2BoostPwmDeadTime, menuCh2BuckPwmMinDuty, menuCh2BoostPwmPeriod, menuCh2BoostPwm, NULL_MENU,
		0, 0, "Ch2 Bst Pwm dead");
/*SUBMENU Ch2 volt rng...  LEVEL 3*/
MENU_ITEM(menuCh2CellVoltCtrlMin, menuCh2CellVoltCtrlMax, menuCh2CellVoltCtrlMax, menuCh2VRange, NULL_MENU,
		0, 0, "Ch2 Voltctrl min");
MENU_ITEM(menuCh2CellVoltCtrlMax, menuCh2CellVoltCtrlMin, menuCh2CellVoltCtrlMin, menuCh2VRange, NULL_MENU,
		0, 0, "Ch2 Voltctrl max");
/*SUBMENU Start Ch1... LEVEL 3*/
MENU_ITEM(menuStartCh1Sp, menuStartCh1On, menuStartCh1State, menuStartCh1, NULL_MENU,
		0, 0, "Ch1 setpoint");
MENU_ITEM(menuStartCh1On, menuStartCh1State, menuStartCh1Sp, menuStartCh1, NULL_MENU,
		0, 0, "Ch1 on/off");
MENU_ITEM(menuStartCh1State, menuStartCh1Sp, menuStartCh1On, menuStartCh1, NULL_MENU,
		0, 0, "Ch1 state");
/*SUBMENU Start Ch2... LEVEL 3*/
MENU_ITEM(menuStartCh2Sp, menuStartCh2On, menuStartCh2State, menuStartCh2, NULL_MENU,
		0, 0, "Ch2 setpoint");
MENU_ITEM(menuStartCh2On, menuStartCh2State, menuStartCh2Sp, menuStartCh2, NULL_MENU,
		0, 0, "Ch2 on/off");
MENU_ITEM(menuStartCh2State, menuStartCh2Sp, menuStartCh2On, menuStartCh2, NULL_MENU,
		0, 0, "Ch2 state");
/*SUBMENU Start Tstat... LEVEL 3*/
MENU_ITEM(menuStartTstatSp, menuStartTstatOn, menuStartTstatState, menuStartThermostat, NULL_MENU,
		0, 0, "Tstat setpoint");
MENU_ITEM(menuStartTstatOn, menuStartTstatState, menuStartTstatSp, menuStartThermostat, NULL_MENU,
		0, 0, "Tstat on/off");
MENU_ITEM(menuStartTstatState, menuStartTstatSp, menuStartTstatOn, menuStartThermostat, NULL_MENU,
		0, 0, "Tstat state");
/*SUBMENU Start Dessipator... LEVEL 3*/
MENU_ITEM(menuStartDessipatorOn, menuStartDessipatorState, menuStartDessipatorState, menuStartDessipator, NULL_MENU,
		0, 0, "Dessip. on/off");
MENU_ITEM(menuStartDessipatorState, menuStartDessipatorOn, menuStartDessipatorOn, menuStartDessipator, NULL_MENU,
		0, 0, "Dessip. state");
/*SUBMENU Reset Alarm... LEVEL 3*/
MENU_ITEM(menuAlarmState, menuAlarmReset, menuAlarmReset, menuResetAlarm, NULL_MENU,
		0, 0, "Alarm state");
MENU_ITEM(menuAlarmReset, menuAlarmState, menuAlarmState, menuResetAlarm, NULL_MENU,
		0, BatteryTester_Menu_resetAlarm, "Alarm reset");
/*SUBMENU Reset Alarm... LEVEL 3*/
MENU_ITEM(menuSetDessipMin, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectDessipatorMinThreshold,
		BatteryTester_Menu_enterDessipatorMinThreshold, "");
MENU_ITEM(menuSetDessipMax, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectDessipatorMaxThreshold,
		BatteryTester_Menu_enterDessipatorMaxThreshold, "");
/*SUBMENU  Tstat. reg...  LEVEL 3*/
MENU_ITEM(menuThermostatKp, menuThermostatKi, menuThermostatMaxLim, menuClimReg, menuSetThermostatKp,
		0, 0, "Tstat Kp");
MENU_ITEM(menuThermostatKi, menuThermostatKd, menuThermostatKp, menuClimReg, menuSetThermostatKi,
		0, 0, "Tstat Ki");
MENU_ITEM(menuThermostatKd, menuThermostatDt, menuThermostatKi, menuClimReg, menuSetThermostatKd,
		0, 0, "Tstat Kd");
MENU_ITEM(menuThermostatDt, menuThermostatSp, menuThermostatKd, menuClimReg, menuSetThermostatDt,
		0, 0, "Tstat dt");
MENU_ITEM(menuThermostatSp, menuThermostatMinLim, menuThermostatDt, menuClimReg, menuSetThermostatSp,
		0, 0, "Tstat Sp");
MENU_ITEM(menuThermostatMinLim, menuThermostatMaxLim, menuThermostatSp, menuClimReg, menuSetThermostatMinLim,
		0, 0, "Tstat minLim");
MENU_ITEM(menuThermostatMaxLim, menuThermostatKp, menuThermostatMinLim, menuClimReg, menuSetThermostatMaxLim,
		0, 0, "Tstat maxLim");
/*SUBMENU  Tstat Kp  LEVEL 4*/
MENU_ITEM(menuSetThermostatKp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatKp, BatteryTester_Menu_enterSetThermostatKp, "");
/*SUBMENU  Tstat Ki  LEVEL 4*/
MENU_ITEM(menuSetThermostatKi, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatKi, BatteryTester_Menu_enterSetThermostatKi, "");
/*SUBMENU  Tstat Kd  LEVEL 4*/
MENU_ITEM(menuSetThermostatKd, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatKd, BatteryTester_Menu_enterSetThermostatKd, "");
/*SUBMENU  Tstat dt  LEVEL 4*/
MENU_ITEM(menuSetThermostatDt, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatDt, BatteryTester_Menu_enterSetThermostatDt, "");
/*SUBMENU  Tstat Sp  LEVEL 4*/
MENU_ITEM(menuSetThermostatSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatSp, BatteryTester_Menu_enterSetThermostatSp, "");
/*SUBMENU  Tstat minLim  LEVEL 4*/
MENU_ITEM(menuSetThermostatMinLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatMinLim, BatteryTester_Menu_enterSetThermostatMinLim, "");
/*SUBMENU  Tstat maxLim  LEVEL 4*/
MENU_ITEM(menuSetThermostatMaxLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatMaxLim, BatteryTester_Menu_enterSetThermostatMaxLim, "");

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
static void (*MenuWriteFunc)(const char* Text) = 0;

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;


Menu_Item_t* BatteryTester_Menu_GetCurrentMenu(void)
{
	return CurrentMenuItem;
}

void BatteryTester_Menu_Navigate(Menu_Item_t* const NewMenu)
{
	if ((NewMenu == &NULL_MENU) || (NewMenu == 0))
		return;

	CurrentMenuItem = NewMenu;

	if (MenuWriteFunc)
		MenuWriteFunc(CurrentMenuItem->Text);

//	void (*SelectCallback)(void) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->SelectCallback);
	void (*SelectCallback)(void) = CurrentMenuItem->SelectCallback;

	if (SelectCallback)
		SelectCallback();
}

void BatteryTester_Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text))
{
	MenuWriteFunc = WriteFunc;
	BatteryTester_Menu_Navigate(CurrentMenuItem);
}

void BatteryTester_Menu_EnterCurrentItem(void)
{
	if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == 0))
		return;

	void (*EnterCallback)(void) = CurrentMenuItem->EnterCallback;

	if (EnterCallback)
		EnterCallback();
	else
		BatteryTester_Menu_Navigate(MENU_CHILD); // Modify me
}


void BatteryTester_Menu_initMenu(){

}
/*void BatteryTester_Menu_onOk(void*);
void BatteryTester_Menu_onUp(void*);
void BatteryTester_Menu_onDown(void*);
void BatteryTester_Menu_onRight(void*);
void BatteryTester_Menu_onLeft(void*);*/
void BatteryTester_Menu_proccessEvents(eEvents_t){

}

void BatteryTester_Menu_enterRefOffset(void){

}

void BatteryTester_Menu_resetAlarm(void){

}

void BatteryTester_Menu_selectDessipatorMinThreshold(void){
	BatteryTester_Menu_selectSetNewValue("Dessip. Min, V",
			BatteryTester_DessipatorControl_getHeaterControlRange().minVoltageInVolts,
			1);
}

void BatteryTester_Menu_enterDessipatorMinThreshold(void){
	BatteryTester_DessipatorControl_setHeaterControlMin(
			BatteryTester_State_sendNewParamFromState());
	BatteryTester_Menu_returnInMenu(&menuDissipMin);
}

void BatteryTester_Menu_selectDessipatorMaxThreshold(void){
	BatteryTester_Menu_selectSetNewValue("Dessip. Max, V",
			BatteryTester_DessipatorControl_getHeaterControlRange().maxVoltageInVolts,
			1);
}

void BatteryTester_Menu_enterDessipatorMaxThreshold(void){
	BatteryTester_DessipatorControl_setHeaterControlMax(
			BatteryTester_State_sendNewParamFromState());
	BatteryTester_Menu_returnInMenu(&menuDissipMax);
}

void BatteryTester_Menu_selectSetThermostatKp(void){
	BatteryTester_Menu_selectSetNewValue("Thermostat Kp, u",
			BatteryTester_ClimatRegulator_getRegulatorSettings().Kp,
			4);
}

void BatteryTester_Menu_enterSetThermostatKp(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().Kp =
					BatteryTester_State_sendNewParamFromState()));
	BatteryTester_Menu_returnInMenu(&menuThermostatKp);
}

void BatteryTester_Menu_selectSetThermostatKi(void){
	BatteryTester_Menu_selectSetNewValue("Thermostat Ki, u",
			BatteryTester_ClimatRegulator_getRegulatorSettings().Ki,
			4);
}

void BatteryTester_Menu_enterSetThermostatKi(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().Ki =
					BatteryTester_State_sendNewParamFromState()));
	BatteryTester_Menu_returnInMenu(&menuThermostatKi);
}

void BatteryTester_Menu_selectSetThermostatKd(void){
	BatteryTester_Menu_selectSetNewValue("Thermostat Kd, u",
			BatteryTester_ClimatRegulator_getRegulatorSettings().Kd,
			4);
}

void BatteryTester_Menu_enterSetThermostatKd(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().Kd =
					BatteryTester_State_sendNewParamFromState()));
	BatteryTester_Menu_returnInMenu(&menuThermostatKd);
}

void BatteryTester_Menu_selectSetThermostatDt(void){
	BatteryTester_Menu_selectSetNewValue("Thermost. dt, mc",
			BatteryTester_ClimatRegulator_getRegulatorSettings().dt * 1000,
			3);
}

void BatteryTester_Menu_enterSetThermostatDt(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().dt =
					BatteryTester_State_sendNewParamFromState() / 1000));
	BatteryTester_Menu_returnInMenu(&menuThermostatDt);
}

void BatteryTester_Menu_selectSetThermostatSp(void){
	BatteryTester_Menu_selectSetNewValue("Thermost. Sp, dC",
			BatteryTester_ClimatRegulator_getRegulatorSettings().setpoint,
			1);
}

void BatteryTester_Menu_enterSetThermostatSp(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().setpoint =
					BatteryTester_State_sendNewParamFromState()));
	BatteryTester_Menu_returnInMenu(&menuThermostatSp);
}

void BatteryTester_Menu_selectSetThermostatMinLim(void){
	BatteryTester_Menu_selectSetNewValue("Tstat. min, dC",
			BatteryTester_ClimatRegulator_getRegulatorSettings().minLimit,
			1);
}

void BatteryTester_Menu_enterSetThermostatMinLim(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().minLimit =
					BatteryTester_State_sendNewParamFromState()));
	BatteryTester_Menu_returnInMenu(&menuThermostatMinLim);
}

void BatteryTester_Menu_selectSetThermostatMaxLim(void){
	BatteryTester_Menu_selectSetNewValue("Tstat. max, dC",
			BatteryTester_ClimatRegulator_getRegulatorSettings().maxLimit,
			1);
}

void BatteryTester_Menu_enterSetThermostatMaxLim(void){
	BatteryTester_ClimatRegulator_setRegulatorSettings(
			&(BatteryTester_ClimatRegulator_getRegulatorSettings().maxLimit =
					BatteryTester_State_sendNewParamFromState()));
	BatteryTester_Menu_returnInMenu(&menuThermostatMaxLim);
}

inline void BatteryTester_Menu_selectSetNewValue(const char* header, float oldValue, unsigned short accuracy){
	if(accuracy > 6){
		accuracy = 6;
	}
	unsigned int size = strlen(header);
	if(size > SIZE_LINE_BUFFER_LCD - 1){
		size = 16;
	}
	BatteryTester_WC1602A_writeLine(0, header, size);
	char strVal[17];
	snprintf(strVal, sizeof(strVal), "%.*f", accuracy, oldValue);
	BatteryTester_WC1602A_writeLine(1, strVal, strlen(strVal));
	BatteryTester_WC1602A_Setpos(1, strlen(strVal) - 1);
	BatteryTester_WC1602A_onCursor();
	BatteryTester_State_setCurrentState(SET_PARAMETERS);
	BatteryTester_State_setCurrentEvent(EVENT_NONE);
	BatteryTester_State_postStrParamForState(strVal);
}

inline void BatteryTester_Menu_returnInMenu(Menu_Item_t* retItemMenu){
	BatteryTester_WC1602A_offCursor();
	BatteryTester_Menu_Navigate(retItemMenu);
	BatteryTester_State_setCurrentState(MENU_NAVIGATE);
	BatteryTester_State_setCurrentEvent(EVENT_NONE);
}
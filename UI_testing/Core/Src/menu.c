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
#include "conversion_data.h"
#include "regulator_cell_one.h"
#include "regulator_cell_two.h"
#include "cells_voltcontrol.h"

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
MENU_ITEM(menuStart, menuSettings, menuLogs, NULL_MENU, menuStartCh1,
		0, 0, "Start...");
/*SUBMENU Settings... LEVEL 1*/
MENU_ITEM(menuSetsConversionData, menuSetsRefOffset, menuSetsThermometr, menuSettings, menuSetRefOffset,
		0, 0, "Measur. sets...");
MENU_ITEM(menuSetsRefOffset, menuSetsChannel1, menuSetsConversionData, menuSettings, menuSetRefOffset,
		0, 0, "Ref. offset...");
MENU_ITEM(menuSetsChannel1, menuSetsChannel2, menuSetsRefOffset, menuSettings, menuCh1Buck,
		0, 0, "Ch1 settings... ");
MENU_ITEM(menuSetsChannel2, menuSetsDessipator, menuSetsChannel1, menuSettings, menuCh2BuckKp,
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
MENU_ITEM(menuStartThermostat, menuStatusDessipator, menuStartCh2, menuStart, menuStartTstatSp,
		0, 0, "Thermostat      start...");
MENU_ITEM(menuStatusDessipator, menuResetAlarm, menuStartThermostat, menuStart, menuDessipatorState,
		0, 0, "Dessipator      status...");
MENU_ITEM(menuResetAlarm, menuStartCh1, menuStatusDessipator, menuStart, menuAlarmState,
		0, 0, "Reset Alarm...");
/*SUBMENU Ch1 settings...  LEVEL 2*/
MENU_ITEM(menuCh1Buck, menuCh1Boost, menuCh1VRange, menuSetsChannel1, menuCh1BuckKp,
		0, 0, "Ch1 buck regul. settings...");
MENU_ITEM(menuCh1Boost, menuCh1BuckPwm, menuCh1Buck, menuSetsChannel1, menuCh1BoostKp,
		0, 0, "Ch1 boost regul.settings...");
MENU_ITEM(menuCh1BuckPwm, menuCh1BoostPwm, menuCh1Boost, menuSetsChannel1, menuCh1BuckPwmMinDuty,
		0, 0, "Ch1 buck pwm    settings...");
MENU_ITEM(menuCh1BoostPwm, menuCh1VRange, menuCh1BuckPwm, menuSetsChannel1, menuCh1BoostPwmMinDuty,
		0, 0, "Ch1 boost pwm   settings...");
MENU_ITEM(menuCh1VRange, menuCh1Buck, menuCh1BoostPwm, menuSetsChannel1, menuCh1CellVoltCtrlMin,
		0, 0, "Ch1 voltcontrol range set...");
/*SUBMENU Ch2 settings...  LEVEL 2*/
MENU_ITEM(menuCh2Buck, menuCh2Boost, menuCh2BoostPwm, menuSetsChannel2, menuCh2BuckKp,
		0, 0, "Ch2 buck regul. settings...");
MENU_ITEM(menuCh2Boost, menuCh2BuckPwm, menuCh2Buck, menuSetsChannel2, menuCh2BoostKp,
		0, 0, "Ch2 boost regul.settings...");
MENU_ITEM(menuCh2BuckPwm, menuCh2BoostPwm, menuCh2Boost, menuSetsChannel2, menuCh2BuckPwmMinDuty,
		0, 0, "Ch2 buck pwm    settings...");
MENU_ITEM(menuCh2BoostPwm, menuCh2Buck, menuCh2BuckPwm, menuSetsChannel2, menuCh2BoostPwmMinDuty,
		0, 0, "Ch2 boost pwm   settings...");
MENU_ITEM(menuCh2VRange, menuCh2Buck, menuCh2BoostPwm, menuSetsChannel2, menuCh2CellVoltCtrlMin,
		0, 0, "Ch2 voltcontrol range set...");
/*SUBMENU Dessipator...  LEVEL 2*/
MENU_ITEM(menuDissipMin, menuDissipMax, menuDissipMax, menuSetsDessipator, menuSetDessipMin,
		0, 0, "Dessipator off  min...");
MENU_ITEM(menuDissipMax, menuDissipMin, menuDissipMin, menuSetsDessipator, menuSetDessipMax,
		0, 0, "Dessipator on   max...");
/*SUBMENU Thermostat...  LEVEL 2*/
MENU_ITEM(menuClimReg, menuClimPwm, menuClimPwm, menuSetsClimat, menuThermostatKp,
		0, 0, "Thermostat regulsettings...");
MENU_ITEM(menuClimPwm, menuClimReg, menuClimReg, menuSetsClimat, menuThermostatMinDutyCycle,
		0, 0, "Thermostat pwm  settings...");
/*SUBMENU Thermometers...  LEVEL 2*/
MENU_ITEM(menuT1, menuT2, menuT4, menuSetsThermometr, menuTemp1ResistanceUpperParam,
		0, 0, "Sens temperat. 1settings...");
MENU_ITEM(menuT2, menuT3, menuT1, menuSetsThermometr, menuTemp2ResistanceUpperParam,
		0, 0, "Sens temperat. 2settings...");
MENU_ITEM(menuT3, menuT4, menuT2, menuSetsThermometr, menuTemp3ResistanceUpperParam,
		0, 0, "Sens temperat. 3settings...");
MENU_ITEM(menuT4, menuT1, menuT3, menuSetsThermometr, menuTemp4ResistanceUpperParam,
		0, 0, "Sens temperat. 4settings...");
/*SUBMENU Ch1 buck sets...  LEVEL 3*/
MENU_ITEM(menuCh1BuckKp, menuCh1BuckKi, menuCh1BuckMaxLim, menuCh1Buck, menuSetCh1BuckKp,
		0, 0, "Ch1 buck regul. factor Kp...");
MENU_ITEM(menuCh1BuckKi, menuCh1BuckKd, menuCh1BuckKp, menuCh1Buck, menuSetCh1BuckKi,
		0, 0, "Ch1 buck regul. factor Ki...");
MENU_ITEM(menuCh1BuckKd, /*menuCh1BuckDt*/menuCh1BuckSp, menuCh1BuckKi, menuCh1Buck, menuSetCh1BuckKd,
		0, 0, "Ch1 buck regul. factor Kd...");
/*MENU_ITEM(menuCh1BuckDt, menuCh1BuckSp, menuCh1BuckKd, menuCh1Buck, NULL_MENU,
		0, 0, "Ch1 Buck dt");*/
MENU_ITEM(menuCh1BuckSp, menuCh1BuckMinLim, /*menuCh1BuckDt*/menuCh1BuckKd, menuCh1Buck, menuSetCh1BuckSp,
		0, 0, "Ch1 buck regul. setpoint...");
MENU_ITEM(menuCh1BuckMinLim, menuCh1BuckMaxLim, menuCh1BuckSp, menuCh1Buck, menuSetCh1BuckMinLim,
		0, 0, "Ch1 buck regul. constrain min...");
MENU_ITEM(menuCh1BuckMaxLim, menuCh1BuckKp, menuCh1BuckMinLim, menuCh1Buck, menuSetCh1BuckMaxLim,
		0, 0, "Ch1 buck regul. constrain max...");
/*SUBMENU Ch1 boost sets...  LEVEL 3*/
MENU_ITEM(menuCh1BoostKp, menuCh1BoostKi, menuCh1BoostMaxLim, menuCh1Boost, menuSetCh1BoostKp,
		0, 0, "Ch1 boost regul.factor Kp...");
MENU_ITEM(menuCh1BoostKi, menuCh1BoostKd, menuCh1BoostKp, menuCh1Boost, menuSetCh1BoostKi,
		0, 0, "Ch1 boost regul.factor Ki...");
MENU_ITEM(menuCh1BoostKd, /*menuCh1BoostDt*/menuCh1BoostSp, menuCh1BoostKi, menuCh1Boost, menuSetCh1BoostKd,
		0, 0, "Ch1 boost regul.factor Kd...");
/*MENU_ITEM(menuCh1BoostDt, menuCh1BoostSp, menuCh1BoostKd, menuCh1Boost, NULL_MENU,
		0, 0, "Ch1 Boost dt");*/
MENU_ITEM(menuCh1BoostSp, menuCh1BoostMinLim, /*menuCh1BoostDt*/menuCh1BoostKd, menuCh1Boost, menuSetCh1BoostSp,
		0, 0, "Ch1 boost regul.setpoint...");
MENU_ITEM(menuCh1BoostMinLim, menuCh1BoostMaxLim, menuCh1BoostSp, menuCh1Boost, menuSetCh1BoostMinLim,
		0, 0, "Ch1 boost regul.constrain min...");
MENU_ITEM(menuCh1BoostMaxLim, menuCh1BoostKp, menuCh1BoostMinLim, menuCh1Boost, menuSetCh1BoostMaxLim,
		0, 0, "Ch1 boost regul.constrain max...");
/*SUBMENU Ch1 buck pwm...  LEVEL 3*/
MENU_ITEM(menuCh1BuckPwmMinDuty, menuCh1BuckPwmMaxDuty, /*menuCh1BuckPwmDeadTime*/menuCh1BuckPwmMaxDuty, menuCh1BuckPwm, menuSetCh1BuckMinDutyCycle,
		0, 0, "Ch1 buck pwm    min dutycycle...");
MENU_ITEM(menuCh1BuckPwmMaxDuty, /*menuCh1BuckPwmPeriod*/menuCh1BuckPwmMinDuty, menuCh1BuckPwmMinDuty, menuCh1BuckPwm, menuSetCh1BuckMaxDutyCycle,
		0, 0, "Ch1 buck pwm    max dutycycle...");
/*MENU_ITEM(menuCh1BuckPwmPeriod, menuCh1BuckPwmDeadTime, menuCh1BuckPwmMaxDuty, menuCh1BuckPwm, NULL_MENU,
		0, 0, "Ch1 Bck Pwm per");
MENU_ITEM(menuCh1BuckPwmDeadTime, menuCh1BuckPwmMinDuty, menuCh1BuckPwmPeriod, menuCh1BuckPwm, NULL_MENU,
		0, 0, "Ch1 Bck Pwm dead");*/
/*SUBMENU Ch1 boost pwm...  LEVEL 3*/
MENU_ITEM(menuCh1BoostPwmMinDuty, menuCh1BoostPwmMaxDuty, /*menuCh1BoostPwmDeadTime*/menuCh1BoostPwmMaxDuty, menuCh1BoostPwm, menuSetCh1BoostMinDutyCycle,
		0, 0, "Ch1 boost pwm   min dutycycle...");
MENU_ITEM(menuCh1BoostPwmMaxDuty, /*menuCh1BoostPwmPeriod*/menuCh1BoostPwmMinDuty, menuCh1BoostPwmMinDuty, menuCh1BoostPwm, menuSetCh1BoostMaxDutyCycle,
		0, 0, "Ch1 boost pwm   max dutycycle...");
/*MENU_ITEM(menuCh1BoostPwmPeriod, menuCh1BoostPwmDeadTime, menuCh1BoostPwmMaxDuty, menuCh1BoostPwm, NULL_MENU,
		0, 0, "Ch1 Bst Pwm per");
MENU_ITEM(menuCh1BoostPwmDeadTime, menuCh1BuckPwmMinDuty, menuCh1BoostPwmPeriod, menuCh1BoostPwm, NULL_MENU,
		0, 0, "Ch1 Bst Pwm dead");*/
/*SUBMENU Ch1 volt rng...  LEVEL 3*/
MENU_ITEM(menuCh1CellVoltCtrlMin, menuCh1CellVoltCtrlMax, menuCh1CellVoltCtrlMax, menuCh1VRange, menuSetCh1CellVoltCtrlMin,
		0, 0, "Ch1 voltcontrol min value off...");
MENU_ITEM(menuCh1CellVoltCtrlMax, menuCh1CellVoltCtrlMin, menuCh1CellVoltCtrlMin, menuCh1VRange, menuSetCh1CellVoltCtrlMax,
		0, 0, "Ch1 voltcontrol max value off...");
/*SUBMENU Ch2 buck sets...  LEVEL 3*/
MENU_ITEM(menuCh2BuckKp, menuCh2BuckKi, menuCh2BuckMaxLim, menuCh2Buck, menuSetCh2BuckKp,
		0, 0, "Ch2 buck regul. factor Kp...");
MENU_ITEM(menuCh2BuckKi, menuCh2BuckKd, menuCh2BuckKp, menuCh2Buck, menuSetCh2BuckKi,
		0, 0, "Ch2 buck regul. factor Ki...");
MENU_ITEM(menuCh2BuckKd, /*menuCh2BuckDt*/menuCh2BuckSp, menuCh2BuckKi, menuCh2Buck, menuSetCh2BuckKd,
		0, 0, "Ch2 buck regul. factor Kd...");
/*MENU_ITEM(menuCh2BuckDt, menuCh2BuckSp, menuCh2BuckKd, menuCh2Buck, NULL_MENU,
		0, 0, "Ch2 Buck dt");*/
MENU_ITEM(menuCh2BuckSp, menuCh2BuckMinLim, /*menuCh2BuckDt*/menuCh2BuckKd, menuCh2Buck, menuSetCh2BuckSp,
		0, 0, "Ch2 buck regul. setpoint...");
MENU_ITEM(menuCh2BuckMinLim, menuCh2BuckMaxLim, menuCh2BuckSp, menuCh2Buck, menuSetCh2BuckMinLim,
		0, 0, "Ch2 buck regul. constrain min...");
MENU_ITEM(menuCh2BuckMaxLim, menuCh2BuckKp, menuCh2BuckMinLim, menuCh2Buck, menuSetCh2BuckMaxLim,
		0, 0, "Ch2 buck regul. constrain max...");
/*SUBMENU Ch2 boost sets...  LEVEL 3*/
MENU_ITEM(menuCh2BoostKp, menuCh2BoostKi, menuCh2BoostMaxLim, menuCh2Boost, menuSetCh2BoostKp,
		0, 0, "Ch2 boost regul.factor Kp...");
MENU_ITEM(menuCh2BoostKi, menuCh2BoostKd, menuCh2BoostKp, menuCh2Boost, menuSetCh2BoostKi,
		0, 0, "Ch2 boost regul.factor Ki...");
MENU_ITEM(menuCh2BoostKd, /*menuCh2BoostDt*/menuCh2BoostSp, menuCh2BoostKi, menuCh2Boost, menuSetCh2BoostKd,
		0, 0, "Ch2 boost regul.factor Kd...");
/*MENU_ITEM(menuCh2BoostDt, menuCh2BoostSp, menuCh2BoostKd, menuCh2Boost, NULL_MENU,
		0, 0, "Ch2 Boost dt");*/
MENU_ITEM(menuCh2BoostSp, menuCh2BoostMinLim, /*menuCh2BoostDt*/menuCh2BoostKd, menuCh2Boost, menuSetCh2BoostSp,
		0, 0, "Ch2 boost regul.setpoint...");
MENU_ITEM(menuCh2BoostMinLim, menuCh2BoostMaxLim, menuCh2BoostSp, menuCh2Boost, menuSetCh2BoostMinLim,
		0, 0, "Ch2 boost regul.constrain min...");
MENU_ITEM(menuCh2BoostMaxLim, menuCh2BoostKp, menuCh2BoostMinLim, menuCh2Boost, menuSetCh2BoostMaxLim,
		0, 0, "Ch2 boost regul.constrain max...");
/*SUBMENU Ch2 buck pwm...  LEVEL 3*/
MENU_ITEM(menuCh2BuckPwmMinDuty, menuCh2BuckPwmMaxDuty, /*menuCh2BuckPwmDeadTime*/menuCh2BuckPwmMaxDuty, menuCh2BuckPwm, menuSetCh2BuckMinDutyCycle,
		0, 0, "Ch2 buck pwm    min dutycycle...");
MENU_ITEM(menuCh2BuckPwmMaxDuty, /*menuCh2BuckPwmPeriod*/menuCh2BuckPwmMinDuty, menuCh2BuckPwmMinDuty, menuCh2BuckPwm, menuSetCh2BuckMaxDutyCycle,
		0, 0, "Ch2 buck pwm    max dutycycle...");
/*MENU_ITEM(menuCh2BuckPwmPeriod, menuCh2BuckPwmDeadTime, menuCh2BuckPwmMaxDuty, menuCh2BuckPwm, NULL_MENU,
		0, 0, "Ch2 Bck Pwm per");
MENU_ITEM(menuCh2BuckPwmDeadTime, menuCh2BuckPwmMinDuty, menuCh2BuckPwmPeriod, menuCh2BuckPwm, NULL_MENU,
		0, 0, "Ch2 Bck Pwm dead");*/
/*SUBMENU Ch2 boost pwm...  LEVEL 3*/
MENU_ITEM(menuCh2BoostPwmMinDuty, menuCh2BoostPwmMaxDuty, /*menuCh2BoostPwmDeadTime*/menuCh2BoostPwmMaxDuty, menuCh2BoostPwm, menuSetCh2BoostMinDutyCycle,
		0, 0, "Ch2 boost pwm   min dutycycle...");
MENU_ITEM(menuCh2BoostPwmMaxDuty, /*menuCh2BoostPwmPeriod*/menuCh2BoostPwmMinDuty, menuCh2BoostPwmMinDuty, menuCh2BoostPwm, menuSetCh2BoostMaxDutyCycle,
		0, 0, "Ch2 Boost pwm   max dutycycle...");
/*MENU_ITEM(menuCh2BoostPwmPeriod, menuCh2BoostPwmDeadTime, menuCh2BoostPwmMaxDuty, menuCh2BoostPwm, NULL_MENU,
		0, 0, "Ch2 Bst Pwm per");
MENU_ITEM(menuCh2BoostPwmDeadTime, menuCh2BuckPwmMinDuty, menuCh2BoostPwmPeriod, menuCh2BoostPwm, NULL_MENU,
		0, 0, "Ch2 Bst Pwm dead");*/
/*SUBMENU Ch2 volt rng...  LEVEL 3*/
MENU_ITEM(menuCh2CellVoltCtrlMin, menuCh2CellVoltCtrlMax, menuCh2CellVoltCtrlMax, menuCh2VRange, menuSetCh2CellVoltCtrlMin,
		0, 0, "Ch2 Voltcontrol min value off...");
MENU_ITEM(menuCh2CellVoltCtrlMax, menuCh2CellVoltCtrlMin, menuCh2CellVoltCtrlMin, menuCh2VRange, menuSetCh2CellVoltCtrlMax,
		0, 0, "Ch2 Voltcontrol max value off...");
/*SUBMENU Start Ch1... LEVEL 3*/
MENU_ITEM(menuStartCh1Sp, menuStartCh1On, menuStartCh1State, menuStartCh1, menuSetCh1Sp,
		0, 0, "Start channel 1 setpoint...");
MENU_ITEM(menuStartCh1On, menuStartCh1State, menuStartCh1Sp, menuStartCh1, NULL_MENU,
		BatteryTester_Menu_selectToggleRunCh1, BatteryTester_Menu_enterToggleRunCh1,
		"");
MENU_ITEM(menuStartCh1State, menuStartCh1Sp, menuStartCh1On, menuStartCh1, NULL_MENU,
		0, 0, "Start channel 1 status...");
/*SUBMENU Start Ch2... LEVEL 3*/
MENU_ITEM(menuStartCh2Sp, menuStartCh2On, menuStartCh2State, menuStartCh2, menuSetCh2Sp,
		0, 0, "Start channel 2 setpoint...");
MENU_ITEM(menuStartCh2On, menuStartCh2State, menuStartCh2Sp, menuStartCh2, NULL_MENU,
		BatteryTester_Menu_selectToggleRunCh2, BatteryTester_Menu_enterToggleRunCh2, "");
MENU_ITEM(menuStartCh2State, menuStartCh2Sp, menuStartCh2On, menuStartCh2, NULL_MENU,
		0, 0, "Start channel 2 status...");
/*SUBMENU Start Tstat... LEVEL 3*/
MENU_ITEM(menuStartTstatSp, menuStartTstatOn, menuStartTstatState, menuStartThermostat, menuSetTstatSp,
		0, 0, "Start thermostatsetpoint...");
MENU_ITEM(menuStartTstatOn, menuStartTstatState, menuStartTstatSp, menuStartThermostat, NULL_MENU,
		BatteryTester_Menu_selectToggleThermostat, BatteryTester_Menu_enterToggleThermostat, "");
MENU_ITEM(menuStartTstatState, menuStartTstatSp, menuStartTstatOn, menuStartThermostat, NULL_MENU,
		0, 0, "Start thermostatstatus...");
/*SUBMENU Start Dessipator... LEVEL 3*/
/*MENU_ITEM(menuStartDessipatorOn, menuStartDessipatorState, menuStartDessipatorState, menuStartDessipator, NULL_MENU,
		0, 0, "Start dessipatoron/off...");*/
MENU_ITEM(menuDessipatorState, NULL_MENU, NULL_MENU, menuStatusDessipator, NULL_MENU,
		0, 0, "Sorry, dessipator status not release");
/*SUBMENU Reset Alarm... LEVEL 3*/
MENU_ITEM(menuAlarmState, menuAlarmReset, menuAlarmReset, menuResetAlarm, NULL_MENU,
		0, 0, "Hardware alarm  status...");
MENU_ITEM(menuAlarmReset, menuAlarmState, menuAlarmState, menuResetAlarm, NULL_MENU,
		0, BatteryTester_Menu_resetAlarm,
			  "!Hardware alarm reset!");
/*SUBMENU Reset Alarm... LEVEL 3*/
MENU_ITEM(menuSetDessipMin, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectDessipatorMinThreshold,
		BatteryTester_Menu_enterDessipatorMinThreshold, "");
MENU_ITEM(menuSetDessipMax, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectDessipatorMaxThreshold,
		BatteryTester_Menu_enterDessipatorMaxThreshold, "");
/*SUBMENU  Tstat. reg...  LEVEL 3*/
MENU_ITEM(menuThermostatKp, menuThermostatKi, menuThermostatMaxLim, menuClimReg, menuSetThermostatKp,
		0, 0, "Thermostat regulfactor Kp...");
MENU_ITEM(menuThermostatKi, menuThermostatKd, menuThermostatKp, menuClimReg, menuSetThermostatKi,
		0, 0, "Thermostat regulfactor Ki...");
MENU_ITEM(menuThermostatKd, /*menuThermostatDt*/menuThermostatSp, menuThermostatKi, menuClimReg, menuSetThermostatKd,
		0, 0, "Thermostat regulfactor Kd...");
/*MENU_ITEM(menuThermostatDt, menuThermostatSp, menuThermostatKd, menuClimReg, menuSetThermostatDt,
		0, 0, "Tstat dt");*/
MENU_ITEM(menuThermostatSp, menuThermostatMinLim, /*menuThermostatDt*/menuThermostatKd, menuClimReg, menuSetThermostatSp,
		0, 0, "Thermostat regulsetpoint...");
MENU_ITEM(menuThermostatMinLim, menuThermostatMaxLim, menuThermostatSp, menuClimReg, menuSetThermostatMinLim,
		0, 0, "Thermostat regulconstrain min...");
MENU_ITEM(menuThermostatMaxLim, menuThermostatKp, menuThermostatMinLim, menuClimReg, menuSetThermostatMaxLim,
		0, 0, "Thermostat regulconstrain max...");
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
/*MENU_ITEM(menuSetThermostatDt, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatDt, BatteryTester_Menu_enterSetThermostatDt, "");*/
/*SUBMENU  Tstat Sp  LEVEL 4*/
MENU_ITEM(menuSetThermostatSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatSp, BatteryTester_Menu_enterSetThermostatSp, "");
/*SUBMENU  Tstat minLim  LEVEL 4*/
MENU_ITEM(menuSetThermostatMinLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatMinLim, BatteryTester_Menu_enterSetThermostatMinLim, "");
/*SUBMENU  Tstat maxLim  LEVEL 4*/
MENU_ITEM(menuSetThermostatMaxLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatMaxLim, BatteryTester_Menu_enterSetThermostatMaxLim, "");
/*SUBMENU  Tstat. pwm...  LEVEL 3*/
MENU_ITEM(menuThermostatMinDutyCycle, menuThermostatMaxDutyCycle, /*menuThermostatDeadTime*/menuThermostatMaxDutyCycle, menuClimPwm, menuSetThermostatMinDutyCycle,
		0, 0, "Thermostat pwm  min dutycycle...");
MENU_ITEM(menuThermostatMaxDutyCycle, /*menuThermostatPeriod*/menuThermostatMinDutyCycle, menuThermostatMinDutyCycle, menuClimPwm, menuSetThermostatMaxDutyCycle,
		0, 0, "Thermostat pwm  max dutycycle...");
/*MENU_ITEM(menuThermostatPeriod, menuThermostatDeadTime, menuThermostatMaxDutyCycle, menuClimPwm, NULL_MENU,
		0, 0, "Tstat Period");
MENU_ITEM(menuThermostatDeadTime, menuThermostatMinDutyCycle, menuThermostatPeriod, menuClimPwm, NULL_MENU,
		0, 0, "Tstat Dead T.");*/
/*SUBMENU  Tstat min Duty  LEVEL 4*/
MENU_ITEM(menuSetThermostatMinDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatMinDutyCycle, BatteryTester_Menu_enterSetThermostatMinDutyCycle, "");
/*SUBMENU  Tstat max Duty  LEVEL 4*/
MENU_ITEM(menuSetThermostatMaxDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatMaxDutyCycle, BatteryTester_Menu_enterSetThermostatMaxDutyCycle, "");
/*SUBMENU  Tstat Period LEVEL 4*/
/*MENU_ITEM(menuSetThermostatPeriod, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatPeriod, BatteryTester_Menu_enterSetThermostatPeriod, "");
SUBMENU  Tstat Dead T. LEVEL 4
MENU_ITEM(menuSetThermostatDeadTime, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatDeadTime, BatteryTester_Menu_enterSetThermostatDeadTime, "");*/
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*SUBMENU  Temperature 1... LEVEL 2*/
MENU_ITEM(menuTemp1ResistanceUpperParam, menuTemp1ResistanceOrigParam, menuTemp1FactorBParam, menuT1, menuSetTemp1ResistanceUpper,
		0, 0, "Thermometer 1   resistor Rup...");
MENU_ITEM(menuTemp1ResistanceOrigParam, menuTemp1TemperatureOrigParam, menuTemp1ResistanceUpperParam, menuT1, menuSetTemp1ResistanceOrig,
		0, 0, "Thermometer 1   resistor Ro...");
MENU_ITEM(menuTemp1TemperatureOrigParam, menuTemp1FactorBParam, menuTemp1ResistanceOrigParam, menuT1, menuSetTemp1TemperatureOrig,
		0, 0, "Thermometer 1   temperature To..");
MENU_ITEM(menuTemp1FactorBParam, menuTemp1ResistanceUpperParam, menuTemp1TemperatureOrigParam, menuT1, menuSetTemp1FactorB,
		0, 0, "Thermometer 1   factor B...");
/*SUBMENU  T1. set Rup  LEVEL 3*/
MENU_ITEM(menuSetTemp1ResistanceUpper, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp1ResistanceUpper, BatteryTester_Menu_enterSetTemp1ResistanceUpper, "");
/*SUBMENU  T1. set Ro  LEVEL 3*/
MENU_ITEM(menuSetTemp1ResistanceOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp1ResistanceOrig, BatteryTester_Menu_enterSetTemp1ResistanceOrig, "");
/*SUBMENU  T1. set To  LEVEL 3*/
MENU_ITEM(menuSetTemp1TemperatureOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp1TemperatureOrig, BatteryTester_Menu_enterSetTemp1TemperatureOrig, "");
/*SUBMENU  T1. set B  LEVEL 3*/
MENU_ITEM(menuSetTemp1FactorB, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp1FactorB, BatteryTester_Menu_enterSetTemp1FactorB, "");
/*SUBMENU  Temperature 2... LEVEL 2*/
MENU_ITEM(menuTemp2ResistanceUpperParam, menuTemp2ResistanceOrigParam, menuTemp2FactorBParam, menuT2, menuSetTemp2ResistanceUpper,
		0, 0, "Thermometer 2 setresistor Rup...");
MENU_ITEM(menuTemp2ResistanceOrigParam, menuTemp2TemperatureOrigParam, menuTemp2ResistanceUpperParam, menuT2, menuSetTemp2ResistanceOrig,
		0, 0, "Thermometer 2 setresistor Ro...");
MENU_ITEM(menuTemp2TemperatureOrigParam, menuTemp2FactorBParam, menuTemp2ResistanceOrigParam, menuT2, menuSetTemp2TemperatureOrig,
		0, 0, "Thermometer 2 settemperature  To...");
MENU_ITEM(menuTemp2FactorBParam, menuTemp2ResistanceUpperParam, menuTemp2TemperatureOrigParam, menuT2, menuSetTemp2FactorB,
		0, 0, "Thermometer 2 setfactor B...");
/*SUBMENU  T2. set Rup  LEVEL 3*/
MENU_ITEM(menuSetTemp2ResistanceUpper, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp2ResistanceUpper, BatteryTester_Menu_enterSetTemp2ResistanceUpper, "");
/*SUBMENU  T2. set Ro  LEVEL 3*/
MENU_ITEM(menuSetTemp2ResistanceOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp2ResistanceOrig, BatteryTester_Menu_enterSetTemp2ResistanceOrig, "");
/*SUBMENU  T2. set To  LEVEL 3*/
MENU_ITEM(menuSetTemp2TemperatureOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp2TemperatureOrig, BatteryTester_Menu_enterSetTemp2TemperatureOrig, "");
/*SUBMENU  T2. set B  LEVEL 3*/
MENU_ITEM(menuSetTemp2FactorB, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp2FactorB, BatteryTester_Menu_enterSetTemp2FactorB, "");
/*SUBMENU  Temperature 3... LEVEL 2*/
MENU_ITEM(menuTemp3ResistanceUpperParam, menuTemp3ResistanceOrigParam, menuTemp3FactorBParam, menuT3, menuSetTemp3ResistanceUpper,
		0, 0, "Thermometer 3 setresistor Rup...");
MENU_ITEM(menuTemp3ResistanceOrigParam, menuTemp3TemperatureOrigParam, menuTemp3ResistanceUpperParam, menuT3, menuSetTemp3ResistanceOrig,
		0, 0, "Thermometer 3 setresistor Ro...");
MENU_ITEM(menuTemp3TemperatureOrigParam, menuTemp3FactorBParam, menuTemp3ResistanceOrigParam, menuT3, menuSetTemp3TemperatureOrig,
		0, 0, "Thermometer 3 settemperature  To...");
MENU_ITEM(menuTemp3FactorBParam, menuTemp3ResistanceUpperParam, menuTemp3TemperatureOrigParam, menuT3, menuSetTemp3FactorB,
		0, 0, "Thermometer 3 setfactor B...");
/*SUBMENU  T3. set Rup  LEVEL 3*/
MENU_ITEM(menuSetTemp3ResistanceUpper, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp3ResistanceUpper, BatteryTester_Menu_enterSetTemp3ResistanceUpper, "");
/*SUBMENU  T3. set Ro  LEVEL 3*/
MENU_ITEM(menuSetTemp3ResistanceOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp3ResistanceOrig, BatteryTester_Menu_enterSetTemp3ResistanceOrig, "");
/*SUBMENU  T3. set To  LEVEL 3*/
MENU_ITEM(menuSetTemp3TemperatureOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp3TemperatureOrig, BatteryTester_Menu_enterSetTemp3TemperatureOrig, "");
/*SUBMENU  T3. set B  LEVEL 3*/
MENU_ITEM(menuSetTemp3FactorB, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp3FactorB, BatteryTester_Menu_enterSetTemp3FactorB, "");
/*SUBMENU  Temperature 4... LEVEL 2*/
MENU_ITEM(menuTemp4ResistanceUpperParam, menuTemp4ResistanceOrigParam, menuTemp4FactorBParam, menuT4, menuSetTemp4ResistanceUpper,
		0, 0, "Thermometer 4 setresistor Rup...");
MENU_ITEM(menuTemp4ResistanceOrigParam, menuTemp4TemperatureOrigParam, menuTemp4ResistanceUpperParam, menuT4, menuSetTemp4ResistanceOrig,
		0, 0, "Thermometer 4 setresistor Ro...");
MENU_ITEM(menuTemp4TemperatureOrigParam, menuTemp4FactorBParam, menuTemp4ResistanceOrigParam, menuT4, menuSetTemp4TemperatureOrig,
		0, 0, "Thermometer 4 settemperature  To...");
MENU_ITEM(menuTemp4FactorBParam, menuTemp4ResistanceUpperParam, menuTemp4TemperatureOrigParam, menuT4, menuSetTemp4FactorB,
		0, 0, "Thermometer 4 setfactor B...");
/*SUBMENU  T4. set Rup  LEVEL 3*/
MENU_ITEM(menuSetTemp4ResistanceUpper, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp4ResistanceUpper, BatteryTester_Menu_enterSetTemp4ResistanceUpper, "");
/*SUBMENU  T4. set Ro  LEVEL 3*/
MENU_ITEM(menuSetTemp4ResistanceOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp4ResistanceOrig, BatteryTester_Menu_enterSetTemp4ResistanceOrig, "");
/*SUBMENU  T4. set To  LEVEL 3*/
MENU_ITEM(menuSetTemp4TemperatureOrig, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp4TemperatureOrig, BatteryTester_Menu_enterSetTemp4TemperatureOrig, "");
/*SUBMENU  T4. set B  LEVEL 3*/
MENU_ITEM(menuSetTemp4FactorB, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetTemp4FactorB, BatteryTester_Menu_enterSetTemp4FactorB, "");
//////////////////////////////////////////////////////////////////////////////////////////
/*SUBMENU  menuCh1BoostKp LEVEL 4*/
MENU_ITEM(menuSetCh1BoostKp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostKp, BatteryTester_Menu_enterSetCh1BoostKp, "");
/*SUBMENU  Ch1Boost Ki  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostKi, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostKi, BatteryTester_Menu_enterSetCh1BoostKi, "");
/*SUBMENU  Ch1Boost Kd  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostKd, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostKd, BatteryTester_Menu_enterSetCh1BoostKd, "");
/*SUBMENU  Ch1Boost dt  LEVEL 4*/
/*MENU_ITEM(menuSetCh1BoostDt, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostDt, BatteryTester_Menu_enterSetCh1BoostDt, "");*/
/*SUBMENU  Ch1Boost Sp  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostSp, BatteryTester_Menu_enterSetCh1BoostSp, "");
/*SUBMENU  Ch1Boost minLim  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostMinLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostMinLim, BatteryTester_Menu_enterSetCh1BoostMinLim, "");
/*SUBMENU  Ch1Boost maxLim  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostMaxLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostMaxLim, BatteryTester_Menu_enterSetCh1BoostMaxLim, "");
/*SUBMENU  Ch1Boost min Duty  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostMinDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostMinDutyCycle, BatteryTester_Menu_enterSetCh1BoostMinDutyCycle, "");
/*SUBMENU  Ch1Boost max Duty  LEVEL 4*/
MENU_ITEM(menuSetCh1BoostMaxDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BoostMaxDutyCycle, BatteryTester_Menu_enterSetCh1BoostMaxDutyCycle, "");
/*SUBMENU  menuCh1BuckKp LEVEL 4*/
MENU_ITEM(menuSetCh1BuckKp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckKp, BatteryTester_Menu_enterSetCh1BuckKp, "");
/*SUBMENU  Ch1Buck Ki  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckKi, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckKi, BatteryTester_Menu_enterSetCh1BuckKi, "");
/*SUBMENU  Ch1Buck Kd  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckKd, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckKd, BatteryTester_Menu_enterSetCh1BuckKd, "");
/*SUBMENU  Ch1Buck Sp  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckSp, BatteryTester_Menu_enterSetCh1BuckSp, "");
/*SUBMENU  Ch1Buck minLim  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckMinLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckMinLim, BatteryTester_Menu_enterSetCh1BuckMinLim, "");
/*SUBMENU  Ch1Buck maxLim  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckMaxLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckMaxLim, BatteryTester_Menu_enterSetCh1BuckMaxLim, "");
/*SUBMENU  Ch1Buck min Duty  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckMinDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckMinDutyCycle, BatteryTester_Menu_enterSetCh1BuckMinDutyCycle, "");
/*SUBMENU  Ch1Buck max Duty  LEVEL 4*/
MENU_ITEM(menuSetCh1BuckMaxDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1BuckMaxDutyCycle, BatteryTester_Menu_enterSetCh1BuckMaxDutyCycle, "");
/*SUBMENU Set Ch1 volt rng...  LEVEL 4*/
MENU_ITEM(menuSetCh1CellVoltCtrlMin, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1VoltCtrlMin, BatteryTester_Menu_enterSetCh1VoltCtrlMin, "");
MENU_ITEM(menuSetCh1CellVoltCtrlMax, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1VoltCtrlMax, BatteryTester_Menu_enterSetCh1VoltCtrlMax, "");
/////////////////////////////////////////////////////////////////////////////////////////////////
/*SUBMENU  menuCh2BoostKp LEVEL 4*/
MENU_ITEM(menuSetCh2BoostKp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostKp, BatteryTester_Menu_enterSetCh2BoostKp, "");
/*SUBMENU  Ch2Boost Ki  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostKi, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostKi, BatteryTester_Menu_enterSetCh2BoostKi, "");
/*SUBMENU  Ch2Boost Kd  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostKd, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostKd, BatteryTester_Menu_enterSetCh2BoostKd, "");
/*SUBMENU  Ch2Boost Sp  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostSp, BatteryTester_Menu_enterSetCh2BoostSp, "");
/*SUBMENU  Ch2Boost minLim  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostMinLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostMinLim, BatteryTester_Menu_enterSetCh2BoostMinLim, "");
/*SUBMENU  Ch2Boost maxLim  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostMaxLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostMaxLim, BatteryTester_Menu_enterSetCh2BoostMaxLim, "");
/*SUBMENU  Ch2Boost min Duty  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostMinDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostMinDutyCycle, BatteryTester_Menu_enterSetCh2BoostMinDutyCycle, "");
/*SUBMENU  Ch2Boost max Duty  LEVEL 4*/
MENU_ITEM(menuSetCh2BoostMaxDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BoostMaxDutyCycle, BatteryTester_Menu_enterSetCh2BoostMaxDutyCycle, "");
/*SUBMENU  menuCh2BuckKp LEVEL 4*/
MENU_ITEM(menuSetCh2BuckKp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckKp, BatteryTester_Menu_enterSetCh2BuckKp, "");
/*SUBMENU  Ch2Buck Ki  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckKi, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckKi, BatteryTester_Menu_enterSetCh2BuckKi, "");
/*SUBMENU  Ch2Buck Kd  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckKd, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckKd, BatteryTester_Menu_enterSetCh2BuckKd, "");
/*SUBMENU  Ch2Buck Sp  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckSp, BatteryTester_Menu_enterSetCh2BuckSp, "");
/*SUBMENU  Ch2Buck minLim  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckMinLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckMinLim, BatteryTester_Menu_enterSetCh2BuckMinLim, "");
/*SUBMENU  Ch2Buck maxLim  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckMaxLim, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckMaxLim, BatteryTester_Menu_enterSetCh2BuckMaxLim, "");
/*SUBMENU  Ch2Buck min Duty  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckMinDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckMinDutyCycle, BatteryTester_Menu_enterSetCh2BuckMinDutyCycle, "");
/*SUBMENU  Ch2Buck max Duty  LEVEL 4*/
MENU_ITEM(menuSetCh2BuckMaxDutyCycle, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2BuckMaxDutyCycle, BatteryTester_Menu_enterSetCh2BuckMaxDutyCycle, "");
/*SUBMENU Set Ch2 volt rng...  LEVEL 4*/
MENU_ITEM(menuSetCh2CellVoltCtrlMin, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2VoltCtrlMin, BatteryTester_Menu_enterSetCh2VoltCtrlMin, "");
MENU_ITEM(menuSetCh2CellVoltCtrlMax, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2VoltCtrlMax, BatteryTester_Menu_enterSetCh2VoltCtrlMax, "");
/*SUBMENU Set Ch2 volt rng...  LEVEL 4*/
MENU_ITEM(menuSetRefOffset, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetRefOffset, BatteryTester_Menu_enterSetRefOffset, "");
/*SUBMENU Start channel 1 setpoint... LEVEL 4*/
MENU_ITEM(menuSetCh1Sp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh1Setpoint, BatteryTester_Menu_enterSetCh1Setpoint, "");

/*SUBMENU Start channel 1 setpoint... LEVEL 4*/
MENU_ITEM(menuSetCh2Sp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetCh2Setpoint, BatteryTester_Menu_enterSetCh2Setpoint, "");

/*SUBMENU Start channel 1 setpoint... LEVEL 4*/
MENU_ITEM(menuSetTstatSp, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU,
		BatteryTester_Menu_selectSetThermostatSetpoint, BatteryTester_Menu_enterSetThermostatSetpoint, "");
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

void BatteryTester_Menu_resetAlarm(void){

}

void BatteryTester_Menu_selectDessipatorMinThreshold(void){
	BatteryTester_Menu_selectSetNewValue("Dessip. Min, V",
			BatteryTester_DessipatorControl_getHeaterControlRange().minVoltageInVolts);//, 1);
}

void BatteryTester_Menu_enterDessipatorMinThreshold(void){
	BatteryTester_DessipatorControl_setHeaterControlMin(
			BatteryTester_State_sendNewParamFromState());
	BatteryTester_Menu_returnInMenu(&menuDissipMin);
}

void BatteryTester_Menu_selectDessipatorMaxThreshold(void){
	BatteryTester_Menu_selectSetNewValue("Dessip. Max, V",
			BatteryTester_DessipatorControl_getHeaterControlRange().maxVoltageInVolts);//, 1);
}

void BatteryTester_Menu_enterDessipatorMaxThreshold(void){
	BatteryTester_DessipatorControl_setHeaterControlMax(
			BatteryTester_State_sendNewParamFromState());
	BatteryTester_Menu_returnInMenu(&menuDissipMax);
}

void BatteryTester_Menu_selectSetThermostatKp(void){
	BatteryTester_Menu_selectSetNewValue("Thermostat Kp, r",
			BatteryTester_ClimatRegulator_getRegulatorSettings().Kp);//, 4);
}

void BatteryTester_Menu_enterSetThermostatKp(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.Kp = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatKp);
}

void BatteryTester_Menu_selectSetThermostatKi(void){
	BatteryTester_Menu_selectSetNewValue("Thermostat Ki, r",
			BatteryTester_ClimatRegulator_getRegulatorSettings().Ki);//, 4);
}

void BatteryTester_Menu_enterSetThermostatKi(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.Ki = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatKi);
}

void BatteryTester_Menu_selectSetThermostatKd(void){
	BatteryTester_Menu_selectSetNewValue("Thermostat Kd, r",
			BatteryTester_ClimatRegulator_getRegulatorSettings().Kd);//, 4);
}

void BatteryTester_Menu_enterSetThermostatKd(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.Kd = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatKd);
}

/*void BatteryTester_Menu_selectSetThermostatDt(void){
	BatteryTester_Menu_selectSetNewValue("Thermost. dt, mc",
			BatteryTester_ClimatRegulator_getRegulatorSettings().dt * 1000,
			3);
}

void BatteryTester_Menu_enterSetThermostatDt(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.dt = BatteryTester_State_sendNewParamFromState() / 1000;
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatDt);
}*/

void BatteryTester_Menu_selectSetThermostatSp(void){
	BatteryTester_Menu_selectSetNewValue("Tstat. Sp, \xdf\x43",
			BatteryTester_ClimatRegulator_getRegulatorSettings().setpoint);//,1);
}

void BatteryTester_Menu_enterSetThermostatSp(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.setpoint = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatSp);
}

void BatteryTester_Menu_selectSetThermostatMinLim(void){
	BatteryTester_Menu_selectSetNewValue("Tstat. min, \xdf\x43",
			BatteryTester_ClimatRegulator_getRegulatorSettings().minLimit);//,1);
}

void BatteryTester_Menu_enterSetThermostatMinLim(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.minLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatMinLim);
}

void BatteryTester_Menu_selectSetThermostatMaxLim(void){
	BatteryTester_Menu_selectSetNewValue("Tstat. max, \xdf\x43",
			BatteryTester_ClimatRegulator_getRegulatorSettings().maxLimit);//,1);
}

void BatteryTester_Menu_enterSetThermostatMaxLim(void){
	sPIDController_t settings = BatteryTester_ClimatRegulator_getRegulatorSettings();
	settings.maxLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatMaxLim);
}

void BatteryTester_Menu_selectSetTemp1ResistanceUpper(void){
	BatteryTester_Menu_selectSetNewValue("T1 Rup, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(0).resistanceUpInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp1ResistanceUpper(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(0);
	settings.resistanceUpInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(0, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp1ResistanceUpperParam);
}

void BatteryTester_Menu_selectSetTemp1ResistanceOrig(void){
	BatteryTester_Menu_selectSetNewValue("T1 Ro, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(0).resistanceOrigInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp1ResistanceOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(0);
	settings.resistanceOrigInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(0, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp1ResistanceOrigParam);
}

void BatteryTester_Menu_selectSetTemp1TemperatureOrig(void){
	BatteryTester_Menu_selectSetNewValue("T1 To, \xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(0).temperatureOrigIndegC);//,1);
}

void BatteryTester_Menu_enterSetTemp1TemperatureOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(0);
	settings.temperatureOrigIndegC = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ConversionData_setNtcSchemeParams(0, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp1TemperatureOrigParam);
}

void BatteryTester_Menu_selectSetTemp1FactorB(void){
	BatteryTester_Menu_selectSetNewValue("T1 f.B, kppm/\xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(0).factorB / 1000);//,3);
}

void BatteryTester_Menu_enterSetTemp1FactorB(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(0);
	settings.factorB = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(0, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp1FactorBParam);
}

void BatteryTester_Menu_selectSetTemp2ResistanceUpper(void){
	BatteryTester_Menu_selectSetNewValue("T2 Rup, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(1).resistanceUpInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp2ResistanceUpper(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(1);
	settings.resistanceUpInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(1, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp2ResistanceUpperParam);
}

void BatteryTester_Menu_selectSetTemp2ResistanceOrig(void){
	BatteryTester_Menu_selectSetNewValue("T2 Ro, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(1).resistanceOrigInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp2ResistanceOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(1);
	settings.resistanceOrigInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(1, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp2ResistanceOrigParam);
}

void BatteryTester_Menu_selectSetTemp2TemperatureOrig(void){
	BatteryTester_Menu_selectSetNewValue("T2 To, \xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(1).temperatureOrigIndegC);//,1);
}

void BatteryTester_Menu_enterSetTemp2TemperatureOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(1);
	settings.temperatureOrigIndegC = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ConversionData_setNtcSchemeParams(1, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp2TemperatureOrigParam);
}

void BatteryTester_Menu_selectSetTemp2FactorB(void){
	BatteryTester_Menu_selectSetNewValue("T2 f.B, kppm/\xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(1).factorB / 1000);//,3);
}

void BatteryTester_Menu_enterSetTemp2FactorB(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(1);
	settings.factorB = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(1, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp2FactorBParam);
}

void BatteryTester_Menu_selectSetTemp3ResistanceUpper(void){
	BatteryTester_Menu_selectSetNewValue("T3 Rup, k\xf4",
			((ntcSchemeParameters_t)BatteryTester_ConversionData_getNtcSchemeParams(2)).resistanceUpInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp3ResistanceUpper(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(2);
	settings.resistanceUpInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(2, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp3ResistanceUpperParam);
}

void BatteryTester_Menu_selectSetTemp3ResistanceOrig(void){
	BatteryTester_Menu_selectSetNewValue("T3 Ro, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(2).resistanceOrigInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp3ResistanceOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(2);
	settings.resistanceOrigInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(2, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp3ResistanceOrigParam);
}

void BatteryTester_Menu_selectSetTemp3TemperatureOrig(void){
	BatteryTester_Menu_selectSetNewValue("T3 To, \xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(2).temperatureOrigIndegC);//,1);
}

void BatteryTester_Menu_enterSetTemp3TemperatureOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(2);
	settings.temperatureOrigIndegC = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ConversionData_setNtcSchemeParams(3, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp3TemperatureOrigParam);
}

void BatteryTester_Menu_selectSetTemp3FactorB(void){
	BatteryTester_Menu_selectSetNewValue("T3 f.B, kppm/\xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(2).factorB / 1000);//,	3);
}

void BatteryTester_Menu_enterSetTemp3FactorB(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(2);
	settings.factorB = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(2, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp3FactorBParam);
}

void BatteryTester_Menu_selectSetTemp4ResistanceUpper(void){
	BatteryTester_Menu_selectSetNewValue("T4 Rup, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(3).resistanceUpInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp4ResistanceUpper(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(3);
	settings.resistanceUpInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(3, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp4ResistanceUpperParam);
}

void BatteryTester_Menu_selectSetTemp4ResistanceOrig(void){
	BatteryTester_Menu_selectSetNewValue("T4 Ro, k\xf4",
			BatteryTester_ConversionData_getNtcSchemeParams(3).resistanceOrigInOhm / 1000);//,4);
}

void BatteryTester_Menu_enterSetTemp4ResistanceOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(3);
	settings.resistanceOrigInOhm = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(3, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp4ResistanceOrigParam);
}

void BatteryTester_Menu_selectSetTemp4TemperatureOrig(void){
	BatteryTester_Menu_selectSetNewValue("T4 To, \xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(3).temperatureOrigIndegC);//,1);
}

void BatteryTester_Menu_enterSetTemp4TemperatureOrig(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(3);
	settings.temperatureOrigIndegC = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ConversionData_setNtcSchemeParams(3, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp4TemperatureOrigParam);
}

void BatteryTester_Menu_selectSetTemp4FactorB(void){
	BatteryTester_Menu_selectSetNewValue("T4 f.B, kppm/\xdf\x43",
			BatteryTester_ConversionData_getNtcSchemeParams(3).factorB / 1000);//,3);
}

void BatteryTester_Menu_enterSetTemp4FactorB(void){
	ntcSchemeParameters_t settings = BatteryTester_ConversionData_getNtcSchemeParams(3);
	settings.factorB = BatteryTester_State_sendNewParamFromState() * 1000;
	BatteryTester_ConversionData_setNtcSchemeParams(3, &settings);
	BatteryTester_Menu_returnInMenu(&menuTemp4FactorBParam);
}

void BatteryTester_Menu_selectSetThermostatMinDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Tstat min D, rel",
			BattetyTester_ClimatRegulator_getPWMSettings().minDutyCycle);//,0);
}

void BatteryTester_Menu_enterSetThermostatMinDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_ClimatRegulator_getPWMSettings();
	settings.minDutyCycle = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatMinDutyCycle);
}

void BatteryTester_Menu_selectSetThermostatMaxDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Tstat Dmax, %",
			BattetyTester_ClimatRegulator_getPWMSettings().maxDutyCycle);//,0);
}
void BatteryTester_Menu_enterSetThermostatMaxDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_ClimatRegulator_getPWMSettings();
	float newMaxDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(newMaxDutyCycle < 0.0 || newMaxDutyCycle > 100.0){
		BatteryTester_Menu_selectSetThermostatMaxDutyCycle();
		return;
	}
	settings.maxDutyCycle = newMaxDutyCycle;
	BatteryTester_ClimatRegulator_setPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatMaxDutyCycle);
}

/*
void BatteryTester_Menu_selectSetThermostatPeriod(void){
	BatteryTester_Menu_selectSetNewValue("Tstat Period",
			BattetyTester_ClimatRegulator_getPWMSettings().periodPwm,
					0);
}

void BatteryTester_Menu_enterSetThermostatPeriod(void){
	sPWMSettings_t settings = BattetyTester_ClimatRegulator_getPWMSettings();
	settings.periodPwm = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatPeriod);
}

void BatteryTester_Menu_selectSetThermostatDeadTime(void){
	BatteryTester_Menu_selectSetNewValue("Tstat Dead T.",
				BattetyTester_ClimatRegulator_getPWMSettings().deadTime,
						0);
}
void BatteryTester_Menu_enterSetThermostatDeadTime(void){
	sPWMSettings_t settings = BattetyTester_ClimatRegulator_getPWMSettings();
	settings.deadTime = BatteryTester_State_sendNewParamFromState();
	BatteryTester_ClimatRegulator_setPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuThermostatDeadTime);
}
*/

void BatteryTester_Menu_selectSetCh1BoostKp(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 boost Kp, r",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().Kp);//,4);
}

void BatteryTester_Menu_enterSetCh1BoostKp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.Kp = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostKp);
}

void BatteryTester_Menu_selectSetCh1BoostKi(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 boost Ki, r",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().Ki);//,4);
}

void BatteryTester_Menu_enterSetCh1BoostKi(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.Ki = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostKi);
}

void BatteryTester_Menu_selectSetCh1BoostKd(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 boost Kd, r",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().Kd);//,4);
}

void BatteryTester_Menu_enterSetCh1BoostKd(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.Kd = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostKd);
}

/*void BatteryTester_Menu_selectSetCh1BoostDt(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 Boost dt, ms",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().dt * 1000,
						3);
}
void BatteryTester_Menu_enterSetCh1BoostDt(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.dt = BatteryTester_State_sendNewParamFromState() / 1000;
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostDt);
}*/

void BatteryTester_Menu_selectSetCh1BoostSp(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 boost Sp, A",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().setpoint);//,3);
}

void BatteryTester_Menu_enterSetCh1BoostSp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.setpoint = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostSp);
}

void BatteryTester_Menu_selectSetCh1BoostMinLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 bst Lmin, A",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().minLimit);//,3);
}

void BatteryTester_Menu_enterSetCh1BoostMinLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.minLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostMinLim);
}

void BatteryTester_Menu_selectSetCh1BoostMaxLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 bst Lmax, A",
			BatteryTester_RegulatorCellOne_getBoostRegulatorSettings().maxLimit);//,3);
}

void BatteryTester_Menu_enterSetCh1BoostMaxLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBoostRegulatorSettings();
	settings.maxLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostMaxLim);
}

void BatteryTester_Menu_selectSetCh1BoostMinDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 bst Dmin, %",
			BattetyTester_RegulatorCellOne_getBoostPWMSettings().minDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh1BoostMinDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellOne_getBoostPWMSettings();
	float minDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(minDutyCycle < 0.0 || minDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh1BoostMinDutyCycle();
		return;
	}
	settings.minDutyCycle = minDutyCycle;
	BatteryTester_RegulatorCellOne_setBoostPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostPwmMinDuty);
}

void BatteryTester_Menu_selectSetCh1BoostMaxDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 bst Dmax, %",
			BattetyTester_RegulatorCellOne_getBoostPWMSettings().maxDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh1BoostMaxDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellOne_getBoostPWMSettings();
	float maxDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(maxDutyCycle < 0.0 || maxDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh1BoostMaxDutyCycle();
		return;
	}
	settings.maxDutyCycle = maxDutyCycle;
	BatteryTester_RegulatorCellOne_setBoostPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BoostPwmMaxDuty);
}
/*void BatteryTester_Menu_selectSetCh1BoostPeriod(void);
void BatteryTester_Menu_enterSetCh1BoostPeriod(void);
void BatteryTester_Menu_selectSetCh1BoostDeadTime(void);
void BatteryTester_Menu_enterSetCh1BoostDeadTime(void);*/
void BatteryTester_Menu_selectSetCh1BuckKp(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Kp, r",
			BatteryTester_RegulatorCellOne_getBuckRegulatorSettings().Kp);//,4);
}

void BatteryTester_Menu_enterSetCh1BuckKp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	settings.Kp = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckKp);
}

void BatteryTester_Menu_selectSetCh1BuckKi(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Ki, r",
			BatteryTester_RegulatorCellOne_getBuckRegulatorSettings().Ki);//,4);
}

void BatteryTester_Menu_enterSetCh1BuckKi(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	settings.Ki = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckKi);
}

void BatteryTester_Menu_selectSetCh1BuckKd(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Kd, r",
			BatteryTester_RegulatorCellOne_getBuckRegulatorSettings().Kd);//,4);
}

void BatteryTester_Menu_enterSetCh1BuckKd(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	settings.Kd = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckKd);
}

void BatteryTester_Menu_selectSetCh1BuckSp(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Sp, A",
			BatteryTester_RegulatorCellOne_getBuckRegulatorSettings().setpoint);//,	3);
}

void BatteryTester_Menu_enterSetCh1BuckSp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	settings.setpoint = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckSp);
}

void BatteryTester_Menu_selectSetCh1BuckMinLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Lmin, A",
			BatteryTester_RegulatorCellOne_getBuckRegulatorSettings().minLimit);//,	3);
}

void BatteryTester_Menu_enterSetCh1BuckMinLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	settings.minLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckMinLim);
}

void BatteryTester_Menu_selectSetCh1BuckMaxLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Lmax, A",
			BatteryTester_RegulatorCellOne_getBuckRegulatorSettings().maxLimit);//,	3);
}

void BatteryTester_Menu_enterSetCh1BuckMaxLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellOne_getBuckRegulatorSettings();
	settings.maxLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellOne_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckMaxLim);
}

void BatteryTester_Menu_selectSetCh1BuckMinDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Dmin, %",
			BattetyTester_RegulatorCellOne_getBuckPWMSettings().minDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh1BuckMinDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellOne_getBuckPWMSettings();
	float minDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(minDutyCycle < 0.0 || minDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh1BuckMinDutyCycle();
		return;
	}
	settings.minDutyCycle = minDutyCycle;
	BatteryTester_RegulatorCellOne_setBuckPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckPwmMinDuty);
}

void BatteryTester_Menu_selectSetCh1BuckMaxDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 buck Dmax, %",
			BattetyTester_RegulatorCellOne_getBuckPWMSettings().maxDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh1BuckMaxDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellOne_getBuckPWMSettings();
	float maxDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(maxDutyCycle < 0.0 || maxDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh1BuckMaxDutyCycle();
		return;
	}
	settings.maxDutyCycle = maxDutyCycle;
	BatteryTester_RegulatorCellOne_setBuckPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh1BuckPwmMaxDuty);
}

void BatteryTester_Menu_selectSetCh1VoltCtrlMax(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 Vmax, V",
			BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_ONE).maxVoltageInVolts);//,2);
}

void BatteryTester_Menu_enterSetCh1VoltCtrlMax(void){
	sVoltRange_t settings = BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_ONE);
	settings.maxVoltageInVolts = BatteryTester_State_sendNewParamFromState();
	BatteryTester_CellsVoltcontrol_setVoltRangeCellx(CELL_ONE, &settings);
	BatteryTester_Menu_returnInMenu(&menuCh1CellVoltCtrlMax);
}

void BatteryTester_Menu_selectSetCh1VoltCtrlMin(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 Vmin, V",
			BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_ONE).minVoltageInVolts);//,2);
}

void BatteryTester_Menu_enterSetCh1VoltCtrlMin(void){
	sVoltRange_t settings = BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_ONE);
	settings.minVoltageInVolts = BatteryTester_State_sendNewParamFromState();
	BatteryTester_CellsVoltcontrol_setVoltRangeCellx(CELL_ONE, &settings);
	BatteryTester_Menu_returnInMenu(&menuCh1CellVoltCtrlMin);
}
//////////////////////////////////////////////////////////////////////////////////////
void BatteryTester_Menu_selectSetCh2BoostKp(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 boost Kp, r",
			BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings().Kp);//,4);
}

void BatteryTester_Menu_enterSetCh2BoostKp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	settings.Kp = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostKp);
}

void BatteryTester_Menu_selectSetCh2BoostKi(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 boost Ki, r",
			BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings().Ki);//,4);
}

void BatteryTester_Menu_enterSetCh2BoostKi(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	settings.Ki = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostKi);
}

void BatteryTester_Menu_selectSetCh2BoostKd(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 boost Kd, r",
			BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings().Kd);//,4);
}

void BatteryTester_Menu_enterSetCh2BoostKd(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	settings.Kd = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostKd);
}

void BatteryTester_Menu_selectSetCh2BoostSp(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 boost Sp, A",
			BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings().setpoint);//,3);
}

void BatteryTester_Menu_enterSetCh2BoostSp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	settings.setpoint = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostSp);
}

void BatteryTester_Menu_selectSetCh2BoostMinLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 bst Lmin, A",
			BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings().minLimit);//,3);
}

void BatteryTester_Menu_enterSetCh2BoostMinLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	settings.minLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostMinLim);
}

void BatteryTester_Menu_selectSetCh2BoostMaxLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 bst Lmax, A",
			BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings().maxLimit);//,3);
}

void BatteryTester_Menu_enterSetCh2BoostMaxLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBoostRegulatorSettings();
	settings.maxLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBoostRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostMaxLim);
}

void BatteryTester_Menu_selectSetCh2BoostMinDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 bst Dmin, %",
			BattetyTester_RegulatorCellTwo_getBoostPWMSettings().minDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh2BoostMinDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellTwo_getBoostPWMSettings();
	float minDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(minDutyCycle < 0.0 || minDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh2BoostMinDutyCycle();
		return;
	}
	settings.minDutyCycle = minDutyCycle;
	BatteryTester_RegulatorCellTwo_setBoostPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostPwmMinDuty);
}

void BatteryTester_Menu_selectSetCh2BoostMaxDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 bst Dmax, %",
			BattetyTester_RegulatorCellTwo_getBoostPWMSettings().maxDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh2BoostMaxDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellTwo_getBoostPWMSettings();
	float maxDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(maxDutyCycle < 0.0 || maxDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh2BoostMaxDutyCycle();
		return;
	}
	settings.maxDutyCycle = maxDutyCycle;
	BatteryTester_RegulatorCellTwo_setBoostPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BoostPwmMaxDuty);
}

void BatteryTester_Menu_selectSetCh2BuckKp(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Kp, r",
			BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings().Kp);//,4);
}

void BatteryTester_Menu_enterSetCh2BuckKp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	settings.Kp = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckKp);
}

void BatteryTester_Menu_selectSetCh2BuckKi(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Ki, r",
			BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings().Ki);//,4);
}

void BatteryTester_Menu_enterSetCh2BuckKi(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	settings.Ki = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckKi);
}

void BatteryTester_Menu_selectSetCh2BuckKd(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Kd, r",
			BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings().Kd);//,4);
}

void BatteryTester_Menu_enterSetCh2BuckKd(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	settings.Kd = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckKd);
}

void BatteryTester_Menu_selectSetCh2BuckSp(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Sp, A",
			BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings().setpoint);//,3);
}

void BatteryTester_Menu_enterSetCh2BuckSp(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	settings.setpoint = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckSp);
}

void BatteryTester_Menu_selectSetCh2BuckMinLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Lmin, A",
			BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings().minLimit);//,3);
}

void BatteryTester_Menu_enterSetCh2BuckMinLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	settings.minLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckMinLim);
}

void BatteryTester_Menu_selectSetCh2BuckMaxLim(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Lmax, A",
			BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings().maxLimit);//,3);
}

void BatteryTester_Menu_enterSetCh2BuckMaxLim(void){
	sPIDController_t settings = BatteryTester_RegulatorCellTwo_getBuckRegulatorSettings();
	settings.maxLimit = BatteryTester_State_sendNewParamFromState();
	BatteryTester_RegulatorCellTwo_setBuckRegulatorSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckMaxLim);
}

void BatteryTester_Menu_selectSetCh2BuckMinDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Dmin, %",
			BattetyTester_RegulatorCellTwo_getBuckPWMSettings().minDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh2BuckMinDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellTwo_getBuckPWMSettings();
	float minDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(minDutyCycle < 0.0 || minDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh2BuckMinDutyCycle();
		return;
	}
	settings.minDutyCycle = minDutyCycle;
	BatteryTester_RegulatorCellTwo_setBuckPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckPwmMinDuty);
}

void BatteryTester_Menu_selectSetCh2BuckMaxDutyCycle(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 buck Dmax, %",
			BattetyTester_RegulatorCellTwo_getBuckPWMSettings().maxDutyCycle);//,3);
}

void BatteryTester_Menu_enterSetCh2BuckMaxDutyCycle(void){
	sPWMSettings_t settings = BattetyTester_RegulatorCellTwo_getBuckPWMSettings();
	float maxDutyCycle = BatteryTester_State_sendNewParamFromState();
	if(maxDutyCycle < 0.0 || maxDutyCycle > 100.0){
		BatteryTester_Menu_selectSetCh2BuckMaxDutyCycle();
		return;
	}
	settings.maxDutyCycle = maxDutyCycle;
	BatteryTester_RegulatorCellTwo_setBuckPWMSettings(&settings);
	BatteryTester_Menu_returnInMenu(&menuCh2BuckPwmMaxDuty);
}

void BatteryTester_Menu_selectSetCh2VoltCtrlMax(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 Vmax, V",
			BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_TWO).maxVoltageInVolts);//,2);
}

void BatteryTester_Menu_enterSetCh2VoltCtrlMax(void){
	sVoltRange_t settings = BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_TWO);
	settings.maxVoltageInVolts = BatteryTester_State_sendNewParamFromState();
	BatteryTester_CellsVoltcontrol_setVoltRangeCellx(CELL_TWO, &settings);
	BatteryTester_Menu_returnInMenu(&menuCh2CellVoltCtrlMax);
}

void BatteryTester_Menu_selectSetCh2VoltCtrlMin(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 Vmin, V",
			BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_TWO).minVoltageInVolts);//, 2);
}

void BatteryTester_Menu_enterSetCh2VoltCtrlMin(void){
	sVoltRange_t settings = BatteryTester_CellsVoltcontrol_getVoltRangeCellx(CELL_TWO);
	settings.minVoltageInVolts = BatteryTester_State_sendNewParamFromState();
	BatteryTester_CellsVoltcontrol_setVoltRangeCellx(CELL_TWO, &settings);
	BatteryTester_Menu_returnInMenu(&menuCh2CellVoltCtrlMin);
}

void BatteryTester_Menu_selectSetRefOffset(void){
	BatteryTester_Menu_selectSetNewValue("Ref. offset, V",
			BatteryTester_ConversionData_getRefOffsetInVolts());//,	2);
}

void BatteryTester_Menu_enterSetRefOffset(void){
	BatteryTester_ConversionData_setRefOffsetInVolts(
			BatteryTester_State_sendNewParamFromState());
	BatteryTester_Menu_returnInMenu(&menuSetsRefOffset);
}

void BatteryTester_Menu_selectSetCh1Setpoint(void){
	BatteryTester_Menu_selectSetNewValue("Ch1 Sp, A",
			BatteryTester_RegulatorCellOne_getSetpoint());
}

void BatteryTester_Menu_enterSetCh1Setpoint(void){
	BatteryTester_RegulatorCellOne_setSetpoint(
				BatteryTester_State_sendNewParamFromState());
		BatteryTester_Menu_returnInMenu(&menuStartCh1Sp);
}

void BatteryTester_Menu_selectToggleRunCh1(void){
	if (MenuWriteFunc){
		if(BatteryTester_RegulatorCellOne_getRunStatus()){
			MenuWriteFunc("Switch off      channel 1");
		}
		else{
			MenuWriteFunc("Switch on       channel 1");
		}
	}
}

void BatteryTester_Menu_enterToggleRunCh1(void){
	BatteryTester_RegulatorCellOne_toggleRunMode();
//	BatteryTester_Menu_returnInMenu(&menuStartCh1On);
	BatteryTester_Menu_returnInMenu(&menuStartCh1State);
}

void BatteryTester_Menu_selectSetCh2Setpoint(void){
	BatteryTester_Menu_selectSetNewValue("Ch2 Sp, A",
			BatteryTester_RegulatorCellTwo_getSetpoint());
}

void BatteryTester_Menu_enterSetCh2Setpoint(void){
	BatteryTester_RegulatorCellTwo_setSetpoint(
			BatteryTester_State_sendNewParamFromState());
	BatteryTester_Menu_returnInMenu(&menuStartCh2Sp);
}

void BatteryTester_Menu_selectToggleRunCh2(void){
	if (MenuWriteFunc){
		if(BatteryTester_RegulatorCellTwo_getRunStatus()){
			MenuWriteFunc("Switch off      channel 2");
		}
		else{
			MenuWriteFunc("Switch on       channel 2");
		}
	}
}

void BatteryTester_Menu_enterToggleRunCh2(void){
	BatteryTester_RegulatorCellTwo_toggleRunMode();
//	BatteryTester_Menu_returnInMenu(&menuStartCh2On);
	BatteryTester_Menu_returnInMenu(&menuStartCh2State);
}

void BatteryTester_Menu_selectSetThermostatSetpoint(void){
	BatteryTester_Menu_selectSetNewValue("Thermost. Sp, \xdf\x43",
			BatteryTester_ClimatRegulator_getSetpoint());
}

void BatteryTester_Menu_enterSetThermostatSetpoint(void){
	BatteryTester_ClimatRegulator_setSetpoint(
				BatteryTester_State_sendNewParamFromState());
		BatteryTester_Menu_returnInMenu(&menuStartTstatSp);
}

void BatteryTester_Menu_selectToggleThermostat(void){
	if (MenuWriteFunc){
		if(BatteryTester_ClimatRegulator_getRunStatus()){
			MenuWriteFunc("Switch off      thermostat");
		}
		else{
			MenuWriteFunc("Switch on       thermostat");
		}
	}
}

void BatteryTester_Menu_enterToggleThermostat(void){
	BatteryTester_ClimatRegulator_toggleRunMode();
//	BatteryTester_Menu_returnInMenu(&menuStartTstatOn);
	BatteryTester_Menu_returnInMenu(&menuStartTstatState);
}
/*@brief:
 * Fill in the top line of the display.
 * Change the state and pass the parameter.
 * The rest is controlled by the parameter setting handler
 * */
inline void BatteryTester_Menu_selectSetNewValue(const char* header, float oldValue/*, unsigned short accuracy*/){
	/*if(accuracy > 6){
		accuracy = 6;
	}*/
	unsigned int size = strlen(header);
	if(size > SIZE_LINE_BUFFER_LCD - 1){
		size = 16;
	}
	BatteryTester_WC1602A_writeLine(0, header, size);
	BatteryTester_State_postParamForSet(oldValue);
	BatteryTester_State_moveFromToState(MENU_NAVIGATE, SET_PARAMETERS);
	BatteryTester_State_setCurrentEvent(EVENT_NONE);
}

inline void BatteryTester_Menu_returnInMenu(Menu_Item_t* retItemMenu){
	BatteryTester_WC1602A_offCursor();
	BatteryTester_Menu_Navigate(retItemMenu);
	BatteryTester_State_setCurrentState(MENU_NAVIGATE);
	BatteryTester_State_setCurrentEvent(EVENT_NONE);
}

void BatteryTester_Menu_init(){
	BatteryTester_Menu_Navigate(&menuStart);
}

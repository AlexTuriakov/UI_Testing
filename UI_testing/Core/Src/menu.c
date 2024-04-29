/* MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.*/
#include "menu.h"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t  NULL_MENU = {0};
/*MAIN MENU*/
MENU_ITEM(menuSettings, menuLogs, menuStart, NULL_MENU, menuSetsRefOffset , 0, 0, "Settings...");
MENU_ITEM(menuLogs, menuStart, menuSettings, NULL_MENU, NULL_MENU, 0, 0, "Logs...");
MENU_ITEM(menuStart, menuSettings, menuLogs, NULL_MENU, menuStartMod1, 0, 0, "Start...");
/*SUBMENU Settings... LEVEL 1*/
MENU_ITEM(menuSetsRefOffset, menuSetsChannel1, menuSetsThermometr, menuSettings, NULL_MENU,
		0, BatteryTester_Menu_enterRefOffset, "Ref. Offset");
MENU_ITEM(menuSetsChannel1, menuSetsChannel2, menuSetsRefOffset, menuSettings, menuCh1Buck, 0, 0, "Ch1 settings... ");
MENU_ITEM(menuSetsChannel2, menuSetsDessipator, menuSetsChannel1, menuSettings, NULL_MENU, 0, 0, "Ch2 settings...");
MENU_ITEM(menuSetsDessipator, menuSetsClimat, menuSetsChannel2, menuSettings, NULL_MENU, 0, 0, "Dessipator...");
MENU_ITEM(menuSetsClimat, menuSetsThermometr, menuSetsDessipator, menuSettings, NULL_MENU, 0, 0, "Thermostat...");
MENU_ITEM(menuSetsThermometr, menuSetsRefOffset, menuSetsClimat, menuSettings, NULL_MENU, 0, 0, "Thermometers...");
/*SUBMENU Start... LEVEL 1*/
MENU_ITEM(menuStartMod1, menuStartMod2, menuStartMod3, menuStart, NULL_MENU,
		0, BatteryTester_Menu_startMode1, "Ch1+Therm strt");
MENU_ITEM(menuStartMod2, menuStartMod3, menuStartMod1, menuStart, NULL_MENU,
		0, BatteryTester_Menu_startMode2, "Ch2+Therm strt");
MENU_ITEM(menuStartMod3, menuStartMod1, menuStartMod2, menuStart, NULL_MENU,
		0, BatteryTester_Menu_startMode1, "Ch1,2+Therm strt");
/*SUBMENU Ch1 settings...  LEVEL 2*/
MENU_ITEM(menuCh1Buck, menuCh1Boost, menuCh1VRange, menuSetsChannel1, NULL_MENU, 0, 0, "Ch1 buck sets...");
MENU_ITEM(menuCh1Boost, menuCh1BuckPwm, menuCh1Buck, menuSetsChannel1, NULL_MENU, 0, 0, "Ch1 boost sets...");
MENU_ITEM(menuCh1BuckPwm, menuCh1BoostPwm, menuCh1Boost, menuSetsChannel1, NULL_MENU, 0, 0, "Ch1 buck pwm...");
MENU_ITEM(menuCh1BoostPwm, menuCh1VRange, menuCh1BuckPwm, menuSetsChannel1, NULL_MENU, 0, 0, "Ch1 boost pwm...");
MENU_ITEM(menuCh1VRange, menuCh1Buck, menuCh1BoostPwm, menuSetsChannel1, NULL_MENU, 0, 0, "Ch1 volt rng...");
/*SUBMENU Ch2 settings...  LEVEL 2*/
MENU_ITEM(menuCh2Buck, menuCh2Boost, menuCh2BoostPwm, menuSetsChannel2, NULL_MENU, 0, 0, "Ch2 buck sets...");
MENU_ITEM(menuCh2Boost, menuCh2BuckPwm, menuCh2Buck, menuSetsChannel2, NULL_MENU, 0, 0, "Ch2 boost sets...");
MENU_ITEM(menuCh2BuckPwm, menuCh2BoostPwm, menuCh2Boost, menuSetsChannel2, NULL_MENU, 0, 0, "Ch2 buck pwm...");
MENU_ITEM(menuCh2BoostPwm, menuCh2Buck, menuCh2BuckPwm, menuSetsChannel2, NULL_MENU, 0, 0, "Ch2 boost pwm...");
MENU_ITEM(menuCh2VRange, menuCh2Buck, menuCh2BoostPwm, menuSetsChannel2, NULL_MENU, 0, 0, "Ch1 volt rng...");
/*SUBMENU Dessipator...  LEVEL 2*/
MENU_ITEM(menuDissipMin, menuDissipMax, menuDissipMax, menuSetsDessipator, NULL_MENU,
		0, BatteryTester_Menu_setDissipatorMin, "Dissip. min");
MENU_ITEM(menuDissipMax, menuDissipMin, menuDissipMin, menuSetsDessipator, NULL_MENU,
		0, BatteryTester_Menu_setDissipatorMax, "Dissip. max");
/*SUBMENU Thermostat...  LEVEL 2*/
MENU_ITEM(menuClimReg, menuClimPwm, menuClimPwm, menuSetsClimat, NULL_MENU, 0, 0, "Therm. reg...");
MENU_ITEM(menuClimPwm, menuClimReg, menuClimReg, menuSetsClimat, NULL_MENU, 0, 0, "Therm. pwm...");
/*SUBMENU Thermometers...  LEVEL 2*/
MENU_ITEM(menuT1, menuT2, menuT4, menuSetsThermometr, NULL_MENU, 0, 0, "Temperature 1...");
MENU_ITEM(menuT2, menuT3, menuT1, menuSetsThermometr, NULL_MENU, 0, 0, "Temperature 2...");
MENU_ITEM(menuT3, menuT4, menuT2, menuSetsThermometr, NULL_MENU, 0, 0, "Temperature 3...");
MENU_ITEM(menuT4, menuT1, menuT3, menuSetsThermometr, NULL_MENU, 0, 0, "Temperature 4...");


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

void BatteryTester_Menu_startMode1(void){

}

void BatteryTester_Menu_startMode2(void){

}

void BatteryTester_Menu_startMode3(void){

}

void BatteryTester_Menu_setDissipatorMin(void){

}

void BatteryTester_Menu_setDissipatorMax(void){

}

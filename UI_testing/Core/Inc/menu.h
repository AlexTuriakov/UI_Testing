/* MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.*/

#ifndef INC_MENU_H_
#define INC_MENU_H_
#include "state_events.h"
#include "set_param.h"
#include <stddef.h>
#include <stdint.h>

/**************type declaration****************/
	/** Type define for a menu item. Menu items should be initialized via the helper
	 *  macro \ref MENU_ITEM(), not created from this type directly in user-code.
	 */
	typedef const struct Menu_Item {
		const struct Menu_Item *Next; /**< Pointer to the next menu item of this menu item */
		const struct Menu_Item *Previous; /**< Pointer to the previous menu item of this menu item */
		const struct Menu_Item *Parent; /**< Pointer to the parent menu item of this menu item */
		const struct Menu_Item *Child; /**< Pointer to the child menu item of this menu item */
		void (*SelectCallback)(void); /**< Pointer to the optional menu-specific select callback of this menu item */
		void (*EnterCallback)(void); /**< Pointer to the optional menu-specific enter callback of this menu item */
		const char Text[]; /**< Menu item text to pass to the menu display callback function */
	} Menu_Item_t;

	/** Creates a new menu item entry with the specified links and callbacks.
	 *
	 *  \param[in] Name      Name of the menu entry, must be unique.
	 *  \param[in] Next      Name of the next linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] Previous  Name of the previous linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] Parent    Name of the parent linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] Child     Name of the child linked menu item, or \ref NULL_MENU if no menu link.
	 *  \param[in] SelectFunc  Function callback to execute when the menu item is selected, or \c NULL for no callback.
	 *  \param[in] EnterFunc   Function callback to execute when the menu item is entered, or \c NULL for no callback.
	 */
#define MENU_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, Text) \
	extern Menu_Item_t Next;     \
	extern Menu_Item_t Previous; \
	extern Menu_Item_t Parent;   \
	extern Menu_Item_t Child;  \
	Menu_Item_t Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, Text}

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
	#define MENU_PARENT         BatteryTester_Menu_GetCurrentMenu()->Parent

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
	#define MENU_CHILD          BatteryTester_Menu_GetCurrentMenu()->Child

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
	#define MENU_NEXT           BatteryTester_Menu_GetCurrentMenu()->Next

	/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
	#define MENU_PREVIOUS       BatteryTester_Menu_GetCurrentMenu()->Previous

	/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
	extern Menu_Item_t NULL_MENU;


	/****************Function declaration****************/
	/** Retrieves the currently selected meny item.
	 *
	 *  \return Pointer to the currently selected meny item.
	 */
	Menu_Item_t* BatteryTester_Menu_GetCurrentMenu(void);

	/** Navigates to an absolute or relative menu entry.
	 *
	 * \param[in] NewMenu  Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
	 *                     \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
	 */
	void BatteryTester_Menu_Navigate(Menu_Item_t* const NewMenu);

	/** Configures the menu text write callback function, fired for all menu items. Within this callback
	 *  function the user should implement code to display the current menu text stored in \ref MENU_ITEM_STORAGE
	 *  memory space.
	 *
	 *  \ref WriteFunc  Pointer to a callback function to execute for each selected menu item.
	 */
	void BatteryTester_Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text));

	/** Enters the currently selected menu item, running its configured callback function (if any). */
	void BatteryTester_Menu_EnterCurrentItem(void);

	/*Handlers block extend*/
	void BatteryTester_Menu_enterRefOffset(void);
	void BatteryTester_Menu_resetAlarm(void);
	void BatteryTester_Menu_selectDessipatorMinThreshold(void);
	void BatteryTester_Menu_enterDessipatorMinThreshold(void);
	void BatteryTester_Menu_selectDessipatorMaxThreshold(void);
	void BatteryTester_Menu_enterDessipatorMaxThreshold(void);
	void BatteryTester_Menu_selectSetThermostatKp(void);
	void BatteryTester_Menu_enterSetThermostatKp(void);
	void BatteryTester_Menu_selectSetNewValue(const char* header, sParamSets_t oldValue/*, unsigned short accuracy*/);
	void BatteryTester_Menu_returnInMenu(Menu_Item_t* retItemMenu);
	void BatteryTester_Menu_selectSetThermostatKi(void);
	void BatteryTester_Menu_enterSetThermostatKi(void);
	void BatteryTester_Menu_selectSetThermostatKd(void);
	void BatteryTester_Menu_enterSetThermostatKd(void);
	//void BatteryTester_Menu_selectSetThermostatDt(void);
	//void BatteryTester_Menu_enterSetThermostatDt(void);
	void BatteryTester_Menu_selectSetThermostatSp(void);
	void BatteryTester_Menu_enterSetThermostatSp(void);
	void BatteryTester_Menu_selectSetThermostatMinLim(void);
	void BatteryTester_Menu_enterSetThermostatMinLim(void);
	void BatteryTester_Menu_selectSetThermostatMaxLim(void);
	void BatteryTester_Menu_enterSetThermostatMaxLim(void);
	void BatteryTester_Menu_selectSetTemp1ResistanceUpper(void);
	void BatteryTester_Menu_enterSetTemp1ResistanceUpper(void);
	void BatteryTester_Menu_selectSetTemp1ResistanceOrig(void);
	void BatteryTester_Menu_enterSetTemp1ResistanceOrig(void);
	void BatteryTester_Menu_selectSetTemp1TemperatureOrig(void);
	void BatteryTester_Menu_enterSetTemp1TemperatureOrig(void);
	void BatteryTester_Menu_selectSetTemp1FactorB(void);
	void BatteryTester_Menu_enterSetTemp1FactorB(void);
	void BatteryTester_Menu_selectSetTemp2ResistanceUpper(void);
	void BatteryTester_Menu_enterSetTemp2ResistanceUpper(void);
	void BatteryTester_Menu_selectSetTemp2ResistanceOrig(void);
	void BatteryTester_Menu_enterSetTemp2ResistanceOrig(void);
	void BatteryTester_Menu_selectSetTemp2TemperatureOrig(void);
	void BatteryTester_Menu_enterSetTemp2TemperatureOrig(void);
	void BatteryTester_Menu_selectSetTemp2FactorB(void);
	void BatteryTester_Menu_enterSetTemp2FactorB(void);
	void BatteryTester_Menu_selectSetTemp3ResistanceUpper(void);
	void BatteryTester_Menu_enterSetTemp3ResistanceUpper(void);
	void BatteryTester_Menu_selectSetTemp3ResistanceOrig(void);
	void BatteryTester_Menu_enterSetTemp3ResistanceOrig(void);
	void BatteryTester_Menu_selectSetTemp3TemperatureOrig(void);
	void BatteryTester_Menu_enterSetTemp3TemperatureOrig(void);
	void BatteryTester_Menu_selectSetTemp3FactorB(void);
	void BatteryTester_Menu_enterSetTemp3FactorB(void);
	void BatteryTester_Menu_selectSetTemp4ResistanceUpper(void);
	void BatteryTester_Menu_enterSetTemp4ResistanceUpper(void);
	void BatteryTester_Menu_selectSetTemp4ResistanceOrig(void);
	void BatteryTester_Menu_enterSetTemp4ResistanceOrig(void);
	void BatteryTester_Menu_selectSetTemp4TemperatureOrig(void);
	void BatteryTester_Menu_enterSetTemp4TemperatureOrig(void);
	void BatteryTester_Menu_selectSetTemp4FactorB(void);
	void BatteryTester_Menu_enterSetTemp4FactorB(void);
	void BatteryTester_Menu_selectSetThermostatMinDutyCycle(void);
	void BatteryTester_Menu_enterSetThermostatMinDutyCycle(void);
	void BatteryTester_Menu_selectSetThermostatMaxDutyCycle(void);
	void BatteryTester_Menu_enterSetThermostatMaxDutyCycle(void);
	//void BatteryTester_Menu_selectSetThermostatPeriod(void);
	//void BatteryTester_Menu_enterSetThermostatPeriod(void);
	//void BatteryTester_Menu_selectSetThermostatDeadTime(void);
	//void BatteryTester_Menu_enterSetThermostatDeadTime(void);
	void BatteryTester_Menu_selectSetCh1BoostKp(void);
	void BatteryTester_Menu_enterSetCh1BoostKp(void);
	void BatteryTester_Menu_selectSetCh1BoostKi(void);
	void BatteryTester_Menu_enterSetCh1BoostKi(void);
	void BatteryTester_Menu_selectSetCh1BoostKd(void);
	void BatteryTester_Menu_enterSetCh1BoostKd(void);
	//void BatteryTester_Menu_selectSetCh1BoostDt(void);
	//void BatteryTester_Menu_enterSetCh1BoostDt(void);
	void BatteryTester_Menu_selectSetCh1BoostSp(void);
	void BatteryTester_Menu_enterSetCh1BoostSp(void);
	void BatteryTester_Menu_selectSetCh1BoostMinLim(void);
	void BatteryTester_Menu_enterSetCh1BoostMinLim(void);
	void BatteryTester_Menu_selectSetCh1BoostMaxLim(void);
	void BatteryTester_Menu_enterSetCh1BoostMaxLim(void);
	void BatteryTester_Menu_selectSetCh1BoostMinDutyCycle(void);
	void BatteryTester_Menu_enterSetCh1BoostMinDutyCycle(void);
	void BatteryTester_Menu_selectSetCh1BoostMaxDutyCycle(void);
	void BatteryTester_Menu_enterSetCh1BoostMaxDutyCycle(void);
	//void BatteryTester_Menu_selectSetCh1BoostPeriod(void);
	//void BatteryTester_Menu_enterSetCh1BoostPeriod(void);
	//void BatteryTester_Menu_selectSetCh1BoostDeadTime(void);
	//void BatteryTester_Menu_enterSetCh1BoostDeadTime(void);
	void BatteryTester_Menu_selectSetCh1BuckKp(void);
	void BatteryTester_Menu_enterSetCh1BuckKp(void);
	void BatteryTester_Menu_selectSetCh1BuckKi(void);
	void BatteryTester_Menu_enterSetCh1BuckKi(void);
	void BatteryTester_Menu_selectSetCh1BuckKd(void);
	void BatteryTester_Menu_enterSetCh1BuckKd(void);
	void BatteryTester_Menu_selectSetCh1BuckSp(void);
	void BatteryTester_Menu_enterSetCh1BuckSp(void);
	void BatteryTester_Menu_selectSetCh1BuckMinLim(void);
	void BatteryTester_Menu_enterSetCh1BuckMinLim(void);
	void BatteryTester_Menu_selectSetCh1BuckMaxLim(void);
	void BatteryTester_Menu_enterSetCh1BuckMaxLim(void);
	void BatteryTester_Menu_selectSetCh1BuckMinDutyCycle(void);
	void BatteryTester_Menu_enterSetCh1BuckMinDutyCycle(void);
	void BatteryTester_Menu_selectSetCh1BuckMaxDutyCycle(void);
	void BatteryTester_Menu_enterSetCh1BuckMaxDutyCycle(void);
	void BatteryTester_Menu_selectSetCh1VoltCtrlMax(void);
	void BatteryTester_Menu_enterSetCh1VoltCtrlMax(void);
	void BatteryTester_Menu_selectSetCh1VoltCtrlMin(void);
	void BatteryTester_Menu_enterSetCh1VoltCtrlMin(void);
	////////////////////////////////////////////////////
	void BatteryTester_Menu_selectSetCh2BoostKp(void);
	void BatteryTester_Menu_enterSetCh2BoostKp(void);
	void BatteryTester_Menu_selectSetCh2BoostKi(void);
	void BatteryTester_Menu_enterSetCh2BoostKi(void);
	void BatteryTester_Menu_selectSetCh2BoostKd(void);
	void BatteryTester_Menu_enterSetCh2BoostKd(void);
	void BatteryTester_Menu_selectSetCh2BoostSp(void);
	void BatteryTester_Menu_enterSetCh2BoostSp(void);
	void BatteryTester_Menu_selectSetCh2BoostMinLim(void);
	void BatteryTester_Menu_enterSetCh2BoostMinLim(void);
	void BatteryTester_Menu_selectSetCh2BoostMaxLim(void);
	void BatteryTester_Menu_enterSetCh2BoostMaxLim(void);
	void BatteryTester_Menu_selectSetCh2BoostMinDutyCycle(void);
	void BatteryTester_Menu_enterSetCh2BoostMinDutyCycle(void);
	void BatteryTester_Menu_selectSetCh2BoostMaxDutyCycle(void);
	void BatteryTester_Menu_enterSetCh2BoostMaxDutyCycle(void);
	void BatteryTester_Menu_selectSetCh2BuckKp(void);
	void BatteryTester_Menu_enterSetCh2BuckKp(void);
	void BatteryTester_Menu_selectSetCh2BuckKi(void);
	void BatteryTester_Menu_enterSetCh2BuckKi(void);
	void BatteryTester_Menu_selectSetCh2BuckKd(void);
	void BatteryTester_Menu_enterSetCh2BuckKd(void);
	void BatteryTester_Menu_selectSetCh2BuckSp(void);
	void BatteryTester_Menu_enterSetCh2BuckSp(void);
	void BatteryTester_Menu_selectSetCh2BuckMinLim(void);
	void BatteryTester_Menu_enterSetCh2BuckMinLim(void);
	void BatteryTester_Menu_selectSetCh2BuckMaxLim(void);
	void BatteryTester_Menu_enterSetCh2BuckMaxLim(void);
	void BatteryTester_Menu_selectSetCh2BuckMinDutyCycle(void);
	void BatteryTester_Menu_enterSetCh2BuckMinDutyCycle(void);
	void BatteryTester_Menu_selectSetCh2BuckMaxDutyCycle(void);
	void BatteryTester_Menu_enterSetCh2BuckMaxDutyCycle(void);
	void BatteryTester_Menu_selectSetCh2VoltCtrlMax(void);
	void BatteryTester_Menu_enterSetCh2VoltCtrlMax(void);
	void BatteryTester_Menu_selectSetCh2VoltCtrlMin(void);
	void BatteryTester_Menu_enterSetCh2VoltCtrlMin(void);
	void BatteryTester_Menu_selectSetRefOffset(void);
	void BatteryTester_Menu_enterSetRefOffset(void);
	void BatteryTester_Menu_selectSetCh1Setpoint(void);
	void BatteryTester_Menu_enterSetCh1Setpoint(void);
	void BatteryTester_Menu_enterToggleRunCh1(void);
	void BatteryTester_Menu_selectToggleRunCh1(void);
	void BatteryTester_Menu_selectSetCh2Setpoint(void);
	void BatteryTester_Menu_enterSetCh2Setpoint(void);
	void BatteryTester_Menu_selectToggleRunCh2(void);
	void BatteryTester_Menu_enterToggleRunCh2(void);
	void BatteryTester_Menu_selectSetThermostatSetpoint(void);
	void BatteryTester_Menu_enterSetThermostatSetpoint(void);
	void BatteryTester_Menu_selectToggleThermostat(void);
	void BatteryTester_Menu_enterToggleThermostat(void);
	void BatteryTester_Menu_selectState1Ch1(void);
	void BatteryTester_Menu_selectState2Ch1(void);
	void BatteryTester_Menu_selectState3Ch1(void);
	void BatteryTester_Menu_selectState1Ch2(void);
	void BatteryTester_Menu_selectState2Ch2(void);
	void BatteryTester_Menu_selectState1Tstat(void);
	void BatteryTester_Menu_selectState2Tstat(void);
	void BatteryTester_Menu_selectState3Tstat(void);
	void BatteryTester_Menu_selectState4Tstat(void);
	void BatteryTester_Menu_selectMeasuringState1(void);
	void BatteryTester_Menu_selectMeasuringState2(void);
	void BatteryTester_Menu_selectMeasuringState3(void);
	void BatteryTester_Menu_selectMeasuringState4(void);
	void BatteryTester_Menu_selectMeasuringState5(void);
	void BatteryTester_Menu_updateState1Ch1(void);
	void BatteryTester_Menu_updateState2Ch1(void);
	void BatteryTester_Menu_updateState3Ch1(void);
	void BatteryTester_Menu_updateState1Ch2(void);
	void BatteryTester_Menu_updateState2Ch2(void);
	void BatteryTester_Menu_updateState1Tstat(void);
	void BatteryTester_Menu_updateState2Tstat(void);
	void BatteryTester_Menu_updateState3Tstat(void);
	void BatteryTester_Menu_updateState4Tstat(void);
	void BatteryTester_Menu_updateMeasuringState1(void);
	void BatteryTester_Menu_updateMeasuringState2(void);
	void BatteryTester_Menu_updateMeasuringState3(void);
	void BatteryTester_Menu_updateMeasuringState4(void);
	void BatteryTester_Menu_updateMeasuringState5(void);
	void BatteryTester_Menu_updateDessipatorState(void);
	void BatteryTester_Menu_selectDessipatorState(void);
	void BatteryTester_Menu_saveAllSettings(void);
	void BatteryTester_Menu_init();
#endif /* INC_MENU_H_ */

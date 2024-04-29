/* MICRO-MENU V2

          (C) Dean Camera, 2012
        www.fourwalledcubicle.com
     dean [at] fourwalledcubicle.com

        Royalty-free for all uses.*/

#ifndef INC_MENU_H_
#define INC_MENU_H_
#include "state_events.h"
#include <stddef.h>
#include <stdint.h>

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
	void BatteryTester_Menu_startMode1(void);
	void BatteryTester_Menu_startMode2(void);
	void BatteryTester_Menu_startMode3(void);
	void BatteryTester_Menu_setDissipatorMin(void);
	void BatteryTester_Menu_setDissipatorMax(void);

#endif /* INC_MENU_H_ */

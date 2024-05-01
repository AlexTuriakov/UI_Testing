/*
 * led_wc1602a.h
 *
 *  Created on: Apr 19, 2024
 *      Author: not i am (Tyuryakov_OA)
 */

#ifndef INC_LCD_WC1602A_H_
#define INC_LCD_WC1602A_H_

/*
 * ls.h
 *
 *  Created on: Sep 11, 2023
 *      Author: Jeka
 */


//#include "stm32f4xx_hal.h"
#include "stm32f0xx_hal.h"
#include "main.h"


#ifdef LCD_8_BIT
#define FUNCTION_SET  0b00111100
#else
#define FUNCTION_SET  0b00101100
#endif
#define DISPLAY_CONTROL 0b00001111
#define DISPLAY_CLEAR 0b00000001
#define ENTRY_MODE_SET 0b00000110
#define CURSOR_ON	0b00001111
#define CURSOR_OFF 0b00001100


void BatteryTester_WC1602A_Delay(unsigned int);
void BatteryTester_WC1602A_init();
void BatteryTester_WC1602A_sendCommand(GPIO_PinState, unsigned short);
void BatteryTester_WC1602A_writeLine(unsigned short, const char*, unsigned short);
void BatteryTester_WC1602A_writeInPos(unsigned short, unsigned short, char*, unsigned short);
void BatteryTester_WC1602A_enableSend();
void BatteryTester_WC1602A_setDataOnBus(unsigned short);
void BatteryTester_WC1602A_Setpos(unsigned short, unsigned short);
void BatteryTester_WC1602A_writeChar(char);
void BatteryTester_WC1602A_onCursor();
void BatteryTester_WC1602A_offCursor();
void BatteryTester_WC1602A_clearDisplay();

#endif /* INC_LCD_WC1602A_H_ */

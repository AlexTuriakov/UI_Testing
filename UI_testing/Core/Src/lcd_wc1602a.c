/*
 * led_wc1602a.c
 *
 *  Created on: Apr 19, 2024
 *      Author: not i am author (Tyuryakov_OA)
 */


#include <lcd_wc1602a.h>

#ifdef CoreDebug_DEMCR_TRCENA_Msk
#ifdef DWT_CTRL_CYCCNTENA_Msk
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC
#endif
#endif

typedef enum{
	RS_ON = 1,
	RS_OFF = 0
} eRSState_t;

void BatteryTester_WC1602A_Delay(unsigned int us)
{
	int32_t us_count_tick =  us * (SystemCoreClock/1000000);
#ifdef CoreDebug_DEMCR_TRCENA_Msk
#ifdef DWT_CTRL_CYCCNTENA_Msk
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT_CYCCNT  = 0;
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;
	while(DWT_CYCCNT < us_count_tick);
	DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk;
#else
	while(us_count_tick--) {
		asm("nop");
	}
#endif
#else
	while(us_count_tick--) {
		asm("nop");
	}
#endif
}

void BatteryTester_WC1602A_init(){
	BatteryTester_WC1602A_Delay(30001);
	BatteryTester_WC1602A_sendCommand(RS_OFF, FUNCTION_SET);
	BatteryTester_WC1602A_Delay(40);
	BatteryTester_WC1602A_sendCommand(RS_OFF, DISPLAY_CONTROL);
	BatteryTester_WC1602A_Delay(40);
	BatteryTester_WC1602A_sendCommand(RS_OFF, DISPLAY_CLEAR);
	BatteryTester_WC1602A_Delay(1531);
	BatteryTester_WC1602A_sendCommand(RS_OFF, ENTRY_MODE_SET);
	BatteryTester_WC1602A_Delay(40);
	BatteryTester_WC1602A_writeLine(0, "+<NESS GROUP", 12);
	BatteryTester_WC1602A_writeLine(1, "RnD CENTER", 10);
}

inline void BatteryTester_WC1602A_setDataOnBus(unsigned short data){
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & (1 << 7)));
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & (1 << 6)));
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & (1 << 5)));
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & (1 << 4)));
#ifdef LCD_8_BIT
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (data & (1 << 3)));
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (data & (1 << 2)));
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (data & (1 << 1)));
	HAL_GPIO_WritePin(LCD_DO_GPIO_Port, LCD_DO_Pin, (data & 1));
#endif
}

inline void BatteryTester_WC1602A_enableSend(){
	HAL_GPIO_WritePin(LCD_Enable_GPIO_Port,	LCD_Enable_Pin, GPIO_PIN_SET);
	BatteryTester_WC1602A_Delay(1);
	HAL_GPIO_WritePin(LCD_Enable_GPIO_Port,	LCD_Enable_Pin,	GPIO_PIN_RESET);
	BatteryTester_WC1602A_Delay(1);
}

void BatteryTester_WC1602A_sendCommand(GPIO_PinState rs, unsigned short data){
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, rs);
	BatteryTester_WC1602A_setDataOnBus(data);
	BatteryTester_WC1602A_enableSend();
#ifndef LCD_8_BIT
	BatteryTester_WC1602A_Delay(44);
	BatteryTester_WC1602A_setDataOnBus(data << 4);
	BatteryTester_WC1602A_enableSend();
#endif
}

void BatteryTester_WC1602A_writeLine(unsigned short row, const char* buf, unsigned short size){
	if(row > 1){
		return;
	}
	if(size > 16){
		size = 16;
	}
	BatteryTester_WC1602A_Setpos(row, 0);

	for(unsigned short i = 0; i < size; i++){
		BatteryTester_WC1602A_writeChar(buf[i]);
	}
}

void BatteryTester_WC1602A_Setpos(unsigned short row, unsigned short col){
	if(col > 15 || row > 1){
		return;
	}
	row = (row == 0)? 0x80: 0xC0;
	BatteryTester_WC1602A_sendCommand(RS_OFF, row | col);
	BatteryTester_WC1602A_Delay(40);
}

void BatteryTester_WC1602A_writeInPos(
		unsigned short row,
		unsigned short col,
		char* buf,
		unsigned short size){
	if(col > 15 || row > 1){
		return;
	}
	if(size > 15 - col){
		size = 15 - col + 1;
	}
	BatteryTester_WC1602A_Setpos(row, col);
	for(unsigned short i = 0; i < size; i++){
		BatteryTester_WC1602A_writeChar(buf[i]);
	}
}

inline void BatteryTester_WC1602A_writeChar(char ch){
		BatteryTester_WC1602A_sendCommand(RS_ON, ch);
		BatteryTester_WC1602A_Delay(45);
}

void BatteryTester_WC1602A_onCursor(){
	BatteryTester_WC1602A_sendCommand(RS_OFF, CURSOR_ON);
	BatteryTester_WC1602A_Delay(40);
}

void BatteryTester_WC1602A_offCursor(){
	BatteryTester_WC1602A_sendCommand(RS_OFF, CURSOR_OFF);
	BatteryTester_WC1602A_Delay(40);
}

void BatteryTester_WC1602A_clearDisplay(){
	BatteryTester_WC1602A_sendCommand(RS_OFF, DISPLAY_CLEAR);
	BatteryTester_WC1602A_Delay(1531);
}

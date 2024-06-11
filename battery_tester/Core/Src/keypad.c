/*
 * keypad.c
 *
 *  Created on: Apr 24, 2024
 *      Author: not me (Tyuryakov_OA)
 */

/**
  ******************************************************************************
  * @file           : button.c
  * @brief          : Buttons driver
  * @author         : TQFP (for https://microtechnics.ru/user-blogs/)
  ******************************************************************************
  */
/*
 * ************HOW USE***************
 * 1. настроить таймер на срабатывание один раз в 1 мс.
 * 2. прописать кнопку в файле .h в перечисление ButtonID
 * 3. настраиваем задержки:
 * 		#define BUTTONS_LONG_PRESS_MS                                1500
 * 		#define BUTTONS_VERY_LONG_PRESS_MS                           3500
 * 4. настроить константу GPIO_BUTTON_NOT_PRESSED.
 * 5. указать в файле .c к каким ножкам подключены кнопки:
 * 		static McuPin buttons[BUTTONS_NUM] = {{GPIOC, GPIO_PIN_13}};
 * 6. добавить в проект библиотеку include «button.h»
 * 7. в Callback от таймера вызвать BUTTON_TimerProcess().
 * 8. работа с библиотекой сводится к вызову нескольких функций:
 * 		while (1)
 * 		{
 * 		    BUTTON_Process();
 * 		    if (BUTTON_GetAction(BUTTON_UP) == BUTTON_SHORT_PRESS)...
 * 		    BUTTON_ResetActions();
 * 9. вызвать  BUTTON_Init();
 * 10.запустить таймер с прерыванием: HAL_TIM_Base_Start_IT(&htim6);
 *
*/

/* Includes ------------------------------------------------------------------*/

#include "keypad.h"
#include "main.h"



/* Declarations and definitions ----------------------------------------------*/

// Configuration

static McuPin buttons[BUTTONS_NUM] = {{OK_GPIO_Port, OK_Pin},
									{UP_GPIO_Port, UP_Pin},
									{DOWN_GPIO_Port, DOWN_Pin},
									{RIGHT_GPIO_Port, RIGHT_Pin},
									{LEFT_GPIO_Port, LEFT_Pin}};

// End of configuration

static uint16_t debounceCounter[BUTTONS_NUM];
static uint8_t waitButtonRelease[BUTTONS_NUM];
static uint16_t buttonPressCounter[BUTTONS_NUM];

static ButtonAction buttonActions[BUTTONS_NUM];
static ButtonState buttonState[BUTTONS_NUM];

/* Functions -----------------------------------------------------------------*/

/******************************************************************************/
void BUTTON_LowLevelManager()
{
  uint8_t currentStates[BUTTONS_NUM];

  for (uint8_t i = 0; i < BUTTONS_NUM; i++)
  {
    currentStates[i] = HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin);

    switch (buttonState[i])
    {
      case BUTTON_STARTING:
        if (currentStates[i] == GPIO_BUTTON_NOT_PRESSED)
        {
          buttonState[i] = BUTTON_NOT_PRESSED;
        }
        break;

      case BUTTON_NOT_PRESSED:
        if (currentStates[i] == GPIO_BUTTON_PRESSED)
        {
          buttonState[i] = BUTTON_WAIT_DEBOUNCE;
          debounceCounter[i] = 0;
        }
        break;

      case BUTTON_WAIT_DEBOUNCE:
        if (debounceCounter[i] == DEBOUNCE_TIME_MS)
        {
          if (currentStates[i] == GPIO_BUTTON_PRESSED)
          {
            buttonState[i] = BUTTON_PRESSED;
          }
          else
          {
            buttonState[i] = BUTTON_NOT_PRESSED;
          }
        }
        break;

      case BUTTON_PRESSED:
        if (currentStates[i] == GPIO_BUTTON_NOT_PRESSED)
        {
          buttonState[i] = BUTTON_WAIT_DEBOUNCE;
          debounceCounter[i] = 0;
        }
        break;

      default:
        break;
    }
  }
}



/******************************************************************************/
void BUTTON_HighLevelManager()
{
  for (uint8_t i = 0; i < BUTTONS_NUM; i++)
  {
    if (buttonActions[i] == BUTTON_NONE)
    {
      if (waitButtonRelease[i] == 0)
      {
        if (buttonState[i] == BUTTON_PRESSED)
        {
          waitButtonRelease[i] = 1;
        }
      }
      else
      {
        if (buttonState[i] == BUTTON_NOT_PRESSED)
        {
          waitButtonRelease[i] = 0;

          if (buttonPressCounter[i] >= BUTTONS_VERY_LONG_PRESS_MS)
          {
            buttonActions[i] = BUTTON_VERY_LONG_PRESS;
          }
          else
          {
            if (buttonPressCounter[i] >= BUTTONS_LONG_PRESS_MS)
            {
              buttonActions[i] = BUTTON_LONG_PRESS;
            }
            else
            {
              buttonActions[i] = BUTTON_SHORT_PRESS;
            }
          }
        }
      }
    }
  }
}



/******************************************************************************/
void BUTTON_Process()
{
  BUTTON_LowLevelManager();
  BUTTON_HighLevelManager();

}



/******************************************************************************/
void BUTTON_TimerProcess()
{
  for (uint8_t i = 0; i < BUTTONS_NUM; i++)
  {
    if (debounceCounter[i] < DEBOUNCE_TIME_MS)
    {
      debounceCounter[i]++;
    }

    if (waitButtonRelease[i] == 1)
    {
      buttonPressCounter[i]++;
    }
    else
    {
      buttonPressCounter[i] = 0;
    }
  }
}



/******************************************************************************/
ButtonAction BUTTON_GetAction(uint8_t index)
{
  return buttonActions[index];
}



/******************************************************************************/
void BUTTON_ResetActions()
{
  for (uint8_t i = 0; i < BUTTONS_NUM; i++)
  {
    buttonActions[i] = BUTTON_NONE;
  }
}



/******************************************************************************/
void BUTTON_Init()
{
  for (uint8_t i = 0; i < BUTTONS_NUM; i++)
  {
    debounceCounter[i] = 0;
    waitButtonRelease[i] = 0;
    buttonPressCounter[i] = 0;
    buttonActions[i] = BUTTON_NONE;
    buttonState[i] = BUTTON_STARTING;
  }
}



/******************************************************************************/


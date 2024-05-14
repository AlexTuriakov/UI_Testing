/*
 * pid_regulator.h
 *
 *  Created on: Apr 9, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_PID_REGULATOR_H_
#define INC_PID_REGULATOR_H_

//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <math.h>
#include "stm32f0xx_hal.h"

/** @brief Структура для хранения параметров PID
 * float Kp Коэффициент пропорциональности
 * float Ki Коэффициент интегральной составляющей
 * float Kd Коэффициент дифференциальной составляющей
 * float prevError Предыдущее значение ошибки
 * float integral Интеграл ошибкиf
 * loat maxLimit
 * float minLimit;
 */
typedef struct _sPIDController_t{
    float Kp;
    float Ki;
    float Kd;
    float dt;
    float setpoint;
    float prevError;
    float integral;
    float maxLimit;
    float minLimit;
} sPIDController_t;

/** @brief Структура для зберігання параметрів необхідних для
 * отримання значення для Pulse таймера, який
 * контролює dutly cycle
 * 		float minDutyCycle;
 * 		float maxDutyCycle;
 *     float minPidOutput;
 *     float maxPidOutput;
 *     float periodPwm;
 *     float deadTime;
 *     unsigned int resolutionCounter;
 *
 */
typedef struct _sPWMSettings_t{
    float minDutyCycle;
    float maxDutyCycle;
    float minPidOutput;
    float maxPidOutput;
    float scale;
    float periodPwm;
    /*@brief: deadTime
     * @deprecated
     */
    float deadTime;
} sPWMSettings_t;

/**
 *  @brief Функция инициализации параметров PID
 *
*/
HAL_StatusTypeDef PID_init(
		volatile sPIDController_t* pidSettings,
		float Kp,
		float Ki,
		float Kd,
		float dt,
		float min,
		float max);

/**
 * @brief Функція чищення
 *
 */
void PID_deinit(sPIDController_t* pidSettings);

/** @brief Функция расчета управляющего сигнала
 *
 */
float PID_update(volatile sPIDController_t *pid, float setpoint, float feedback);

/**
 * @brief
 *
 * */
HAL_StatusTypeDef PID_initPWM(
		volatile sPIDController_t*,
		volatile sPWMSettings_t*,
		float,
		float,
		float
		);
void PID_deinitPWM(sPWMSettings_t* pwmSettings);
int PID_calcOutputToPWM(volatile sPWMSettings_t*, float);
void PID_setProportionalFactor(volatile sPIDController_t *,	float);
void PID_setIntegralFactor(volatile sPIDController_t *,	float);
void PID_setDerivativeFactor(volatile sPIDController_t *, float);
void PID_setTime(volatile sPIDController_t *, float);
void TA_setLimits(volatile sPIDController_t *, float, float);
void PID_resetAccumulatedDeviation(volatile sPIDController_t *);
float PID_updateDelta(volatile sPIDController_t *, float, float);

#endif /* INC_PID_REGULATOR_H_ */

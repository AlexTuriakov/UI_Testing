/*
 * pid_regulator.c
 *
 *  Created on: Apr 9, 2024
 *      Author: Tyuryakov_OA
 */


#include "pid_regulator.h"
#include <stdlib.h>
//#include <math.h>

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
		float max) {

//	 pidSettings = (sPIDController_t*)malloc(sizeof(sPIDController_t));
	 if(!pidSettings){
		 return HAL_ERROR;
	 }

	pidSettings->Kp = Kp;
	pidSettings->Ki = Ki;
	pidSettings->Kd = Kd;
	pidSettings->dt = dt;
	pidSettings->prevError = 0;
	pidSettings->integral = 0;
	pidSettings->maxLimit = max;
	pidSettings->minLimit = min;

	return HAL_OK;
}

void PID_deinit(sPIDController_t* pidSettings){
//	free(pidSettings);
}

inline void PID_resetAccumulatedDeviation(volatile sPIDController_t *ppid){
	ppid->prevError = ppid->integral = 0;
}

/** @brief Функция расчета управляющего сигнала
 *
 */
float PID_update(volatile sPIDController_t *pid, float setpoint, float feedback) {

	if(pid->setpoint != setpoint){
		pid->setpoint = setpoint;
		PID_resetAccumulatedDeviation(pid);
	}

	inline float constrain(float input){
		return (input < pid->minLimit)? pid->minLimit:
				((input > pid->maxLimit)? pid->maxLimit: input);
	}

    float error = setpoint - feedback;
    float proportional = pid->Kp * error;
    pid->integral = constrain(pid->integral + error * pid->dt);
    float integral = pid->Ki * pid->integral;
    float derivative = pid->Kd * (error - pid->prevError) / pid->dt;
    pid->prevError = error;
    return constrain(feedback + proportional + integral + derivative);
}

/**
 * @brief
 *
 * */
HAL_StatusTypeDef PID_initPWM(
		volatile sPIDController_t* pidSettings,
		volatile sPWMSettings_t* pwmSettings,
		float minDutyInPercent,
		float maxDutyInPercent,
		float periodPwm
		){

//	pwmSettings = (sPWMSettings_t*)malloc(sizeof(sPWMSettings_t));

	if(!pwmSettings){
		return HAL_ERROR;
	}

	if(maxDutyInPercent > 100){
		maxDutyInPercent = 100;
	}


	if(minDutyInPercent > maxDutyInPercent){
		minDutyInPercent = maxDutyInPercent;
	}

	pwmSettings->minDutyCycle = minDutyInPercent;
	pwmSettings->maxDutyCycle =  maxDutyInPercent;

	if(pidSettings){
		pwmSettings->minPidOutput = pidSettings->minLimit;
		pwmSettings->maxPidOutput = pidSettings->maxLimit;
	}
	else{
		pwmSettings->minPidOutput = 0;
		pwmSettings->maxPidOutput = 0;
	}

	pwmSettings->periodPwm = periodPwm + 1;

	pwmSettings->scale = (float)(pwmSettings->maxDutyCycle - pwmSettings->minDutyCycle) /
			(pwmSettings->maxPidOutput - pwmSettings->minPidOutput);

	return HAL_OK;
}

/**
 * @brief Визволити  захвачені ресурси
 * */
void PID_deinitPWM(sPWMSettings_t* pwmSettings){
//	free(pwmSettings);
}

/*
 * @brief
 * */

unsigned int PID_calcOutputToPWM(volatile sPWMSettings_t* pwmSettings, float pidOut){
	if(!pwmSettings){
		return 0;
	}

	float dutyCycle = ((pidOut - pwmSettings->minPidOutput) *
			pwmSettings->scale + (float)pwmSettings->minDutyCycle) /
					100.0;

	return (unsigned int) abs((int)(pwmSettings->periodPwm * (dutyCycle)));
}

void PID_setProportionalFactor(volatile sPIDController_t *pid, float kp){
	if(!pid){
		return;
	}
	pid->Kp = kp;
}

/*
 * @brief
 * */
void PID_setIntegralFactor(volatile sPIDController_t *pid, float ki){
	if(!pid){
		return;
	}
	pid->Ki = ki;
}

/*
 * @brief
 * */
void PID_setDerivativeFactor(volatile sPIDController_t *pid, float kd){
	if(!pid){
		return;
	}
	pid->Kd = kd;
}

/*
 * @brief
 * */
void PID_setTime(volatile sPIDController_t *pid, float dt){
	if(!pid){
		return;
	}
	pid->dt = dt;
}

/*
 * @brief
 * */
void TA_setLimits(volatile sPIDController_t *pid, float min, float max){
	if(!pid){
		return;
	}
	pid->minLimit = min;
	pid->maxLimit = max;
}

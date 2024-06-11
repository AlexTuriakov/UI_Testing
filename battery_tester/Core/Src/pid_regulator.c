/*
 * pid_regulator.c
 *
 *  Created on: Apr 9, 2024
 *      Author: Tyuryakov_OA
 */


#include "pid_regulator.h"
#include <stdlib.h>
//#include <math.h>


inline void PID_resetAccumulatedDeviation(volatile sPIDController_t *ppid){
	ppid->prevError = ppid->integral = 0;
}

/** @brief Функция расчета управляющего сигнала
 * Назову його піп-регулятор
 * Нерекурентная реалізація ідеального безперервного
 * аналогового під-регулятора з великою похибкою
 * диференціюючої складової з відсутності фільтруючої
 * частини, як в реальних аналогових регуляторах з
 * диференціюючим rc-ланцюгом
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
    //return constrain(feedback + proportional + integral + derivative); // WTF
    return constrain(proportional + integral + derivative);
}

/* @brief: WTF
 * @deprecated
 */
float PID_updateDelta(volatile sPIDController_t *pid, float setpoint, float feedback) {

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
    return constrain(proportional + integral + derivative);
}

/*
 * @brief
 * */
int PID_calcOutputToPWM(volatile sPWMSettings_t* pwmSettings, float pidOut){
	if(!pwmSettings){
		return 0;
	}

	float dutyCycle = ((pidOut - pwmSettings->minPidOutput) *
			pwmSettings->scale + (float)pwmSettings->minDutyCycle) /
					100.0;

	return /*abs*/((int)(pwmSettings->periodPwm * (dutyCycle)));
}

/**
 *  @brief PID_setProportionalFactor
 *  @deprecated
 *
*/
void PID_setProportionalFactor(volatile sPIDController_t *pid, float kp){
	if(!pid){
		return;
	}
	pid->Kp = kp;
}

/**
 *  @brief PID_setIntegralFactor
 *  @deprecated
 *
*/
void PID_setIntegralFactor(volatile sPIDController_t *pid, float ki){
	if(!pid){
		return;
	}
	pid->Ki = ki;
}

/**
 *  @brief PID_setDerivativeFactor
 *  @deprecated
 *
*/
void PID_setDerivativeFactor(volatile sPIDController_t *pid, float kd){
	if(!pid){
		return;
	}
	pid->Kd = kd;
}

/**
 *  @brief PID_setTime
 *  @deprecated
 *
*/
void PID_setTime(volatile sPIDController_t *pid, float dt){
	if(!pid){
		return;
	}
	pid->dt = dt;
}

/**
 *  @brief TA_setLimits
 *  @deprecated
 *
*/
void TA_setLimits(volatile sPIDController_t *pid, float min, float max){
	if(!pid){
		return;
	}
	pid->minLimit = min;
	pid->maxLimit = max;
}

/**
 *  @brief Функция инициализации параметров PID
 *  @deprecated
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

/**
 *  @brief PID_deinit
 *  @deprecated
 *
*/
void PID_deinit(sPIDController_t* pidSettings){
//	free(pidSettings);
}

/**
 *  @brief PID_initPWM
 *  @deprecated
 *
*/
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
 *  @brief PID_deinitPWM
 *  @deprecated
 *
*/
void PID_deinitPWM(sPWMSettings_t* pwmSettings){
//	free(pwmSettings);
}

/*
 * @brief:
 * https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller#Discrete_implementation
 */
float PIDF_update(sPIDFController_t *pid, float setpoint, float measured_value) {
	pid->pre_pre_error = pid->pre_error;
	pid->pre_error = pid->error;
	pid->error = setpoint - measured_value;
	// PI
	float output = pid->pre_output +
			pid->a0 * pid->error +
			pid->a1 * pid->pre_error;
	// Filtered D
	pid->d1 = pid->d0;
	pid->d0 = pid->a0d * pid->error +
			pid->a1d * pid->pre_error +
			pid->a2d * pid->pre_pre_error;
	pid->fd1 = pid->fd0;
	pid->fd0 = (pid->alpha / (pid->alpha + 1)) * (pid->d0 + pid->d1) -
			((pid->alpha - 1) / (pid->alpha + 1)) * pid->fd1;
	output += pid->fd0;
	pid->pre_output = output;
	return output;
}

void PIDF_Init(
		sPIDFController_t *pid,
		float kp, float ki,
		float kd, float N,
		float dt) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->N = N; // 5
    pid->dt = dt;
    pid->a0 = kp + ki * dt;
    pid->a1 = -kp;
    pid->pre_pre_error = 0;
    pid->pre_error = 0;
    pid->error = 0;
    pid->pre_output = 0;
    pid->a0d = kd / dt;
    pid->a1d = -2 * kd / dt;
    pid->a2d = kd / dt;
    pid->tau = kd / (kp * N);
    pid->alpha = dt / (2 * pid->tau);
    pid->d0 = 0;
    pid->d1 = 0;
    pid->fd0 = 0;
    pid->fd1 = 0;
}

/*
 * @brief: From wikipedia
 * https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller#Discrete_implementation
 */
float PIDr_update(sPIDrController_t* pid, float setpoint, float measured_value){
	pid->pre_previous_error = pid->previous_error;
	pid->previous_error = pid->error;
	pid->error = setpoint - measured_value;
	float output = pid->previous_output +
			pid->a0 * pid->error +
			pid->a1 * pid->previous_error +
			pid->a2 * pid->pre_previous_error;
	pid->previous_output = output;
	return output;
}

void PIDr_init(sPIDrController_t *pid,
		float kp, float ki,
		float kd, float dt){
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->a0 = kp + ki * dt + kd / dt;
	pid->a1 = -kp - 2 * kd / dt;
	pid->a2 = kd / dt;
	pid->previous_output = 0;
	pid->pre_previous_error = 0;
	pid->previous_error = 0;
	pid->error = 0;
}

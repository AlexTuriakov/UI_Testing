/*
 * work_status.h
 *
 *  Created on: May 17, 2024
 *      Author: Tyuryakov_OA
 */

#ifndef INC_WORK_STATUS_H_
#define INC_WORK_STATUS_H_

/**************type declaration****************/
typedef void (*BatteryTester_WorkStatus_updateCallback_t)(void);

/****************Function declaration****************/
void BatteryTester_WorkStatus_updateStatus(void* param);
void BatteryTester_WorkStatus_returnToPreviousState(void* param);
void BatteryTester_WorkStatus_upStatus(void* param);
void BatteryTester_WorkStatus_downStatus(void* param);
void BatteryTester_WorkStatus_setUpdateCallback(
		BatteryTester_WorkStatus_updateCallback_t);

#endif /* INC_WORK_STATUS_H_ */

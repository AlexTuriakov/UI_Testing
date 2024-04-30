/*
 * auxiliary_function.c
 *
 *  Created on: Apr 16, 2024
 *      Author: Tyuryakov_OA
 */
#include "auxiliary_function.h"

inline void __BatteryTester_AuxiliaryFunction_copy(
		void* pInput, void* pOutput, unsigned int size){
	for(int i = 0; i < size; i++){
		*(((float*)pOutput) + i) = *(((float*)pInput) + i);
	}
}

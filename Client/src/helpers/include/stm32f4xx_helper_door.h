/*
 * stm32f4xx_helper_door.h
 *
 *  Created on: 04/giu/2015
 *      Author: giovannidifiore
 */

#ifndef HELPERS_INCLUDE_STM32F4XX_HELPER_DOOR_H_
#define HELPERS_INCLUDE_STM32F4XX_HELPER_DOOR_H_

#include <stdint.h>

#define DOOR_AXE_DELTA 	2000

typedef enum {
	DOOR_AUTOLIGHT_ON = 1,
	DOOR_AUTOLIGHT_OFF = 0
} DOORAutoLight_TypeDef;

typedef enum {
	DOOR_OPEN = 1,
	DOOR_CLOSE = 0
} DOORStatus_TypeDef;

void DOOR_Init();
void DOOR_UpdateSteadyAxe(const float axe);
uint8_t DOOR_checkOpen(const float currentAxe);

#endif /* HELPERS_INCLUDE_STM32F4XX_HELPER_DOOR_H_ */

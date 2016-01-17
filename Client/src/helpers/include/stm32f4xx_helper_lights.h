/*
 * stm32f4xx_helper_lights.h
 *
 *  Created on: 05/giu/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPER_LIGHTS_H_
#define __STM32F4XX_HELPER_LIGHTS_H_

#include "stm32f4xx_hal.h"

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} GPIOLight_TypeDef;

void LIGHT_Init(GPIOLight_TypeDef);
void LIGHT_TurnOn(GPIOLight_TypeDef);
void LIGHT_TurnOff(GPIOLight_TypeDef);

#endif /* __STM32F4XX_HELPER_LIGHT_H_ */

/*
 * stm32f4xx_helper_buzzer.h
 *
 *  Created on: 05/giu/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPER_BUZZER_H_
#define __STM32F4XX_HELPER_BUZZER_H_

/* Includes */
#include "stm32f4xx_hal.h"

/** Exported types *******************************************/

#define BUZZER_PIN					GPIO_PIN_10
#define BUZZER_PORT					GPIOD
#define BUZZER_CLK_ENABLE()			__HAL_RCC_GPIOD_CLK_ENABLE()
#define BUZZER_CLK_DISABLE()		__HAL_RCC_GPIOD_CLK_DISABLE()

/* Exported functions ------------------------------------------*/
void TIMx_BUZZER_Init();
void buzz(uint32_t frequency, uint32_t duration);
void buzz_sleep(uint32_t duration);


#endif /* __STM32F4XX_HELPER_BUZZER_H_ */

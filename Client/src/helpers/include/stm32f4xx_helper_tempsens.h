/*
 * stm32f4xx_helper_tempsens.h
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPER_TEMPSENS_H_
#define __STM32F4XX_HELPER_TEMPSENS_H_

#include "stm32f4xx_hal.h"

/* Definition for ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_0
#define ADCx_CHANNEL_GPIO_PORT          GPIOB

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_TEMPSENSOR

/* Definition for ADCx's NVIC */
#define ADCx_IRQn                   	ADC_IRQn
#define ADCx_IRQHandler             	ADC_IRQHandler


void ADCx_TEMPSENS_Init(void);
void ADCx_TEMPSENS_Read_IT(void);
float ADCx_TEMPSENS_GetValue(void);
float TEMPSENS_CelsiusFromMillivolts(uint32_t);

#endif /* __STM32F4XX_HELPER_TEMPSENS_H_ */

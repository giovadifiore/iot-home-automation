/*
 * stm32f4xx_helper_timers.h
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPER_TIMERS_H_
#define __STM32F4XX_HELPER_TIMERS_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_helper_errors.h"

/* Definition for TIMx's */
#define TIMx_SYS						TIM3
#define TIMx_SYS_IRQn					TIM3_IRQn
#define TIMx_SYS_IRQHandler				TIM3_IRQHandler

void TIMx_SYS_Init(int);
void TIMx_SYS_Start(void);


/* Definition for PWM */
#define TIMx_PWM                           TIM4
#define TIMx_PWM_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()
#define TIMx_PWM_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()

#define PWM_GPIO_PORT                      GPIOB
#define PWM_GPIO_PIN_CHANNEL2              GPIO_PIN_7
#define GPIO_AF_TIMx_PWM                   GPIO_AF2_TIM4
#define TIMx_PWM_IRQn                      TIM4_IRQn
#define TIMx_PWM_IRQHandler                TIM4_IRQHandler

// PWM
typedef struct {
	float level;
} TIMx_PWM_Handle;


/* Buzzer  */
#define TIMx_BUZZER					TIM2
#define TIMx_BUZZER_CLK_ENABLE()	__HAL_RCC_TIM2_CLK_ENABLE()
#define TIMx_BUZZER_CLK_DISABLE()	__HAL_RCC_TIM2_CLK_DISABLE()
#define TIMx_BUZZER_IRQn			TIM2_IRQn
#define TIMx_BUZZER_PERIOD			84

void TIMx_PWM_Init(TIMx_PWM_Handle *tph);
void TIMx_PWM_IncrementPulse(TIMx_PWM_Handle *tph);
void TIMx_PWM_DecrementPulse(TIMx_PWM_Handle *tph);
void TIMx_PWM_ResetPulse(TIMx_PWM_Handle *tph);


#endif /* __STM32F4XX_HELPER_TIMERS_H_ */

/*
 * stm32f4xx_buzzer.c
 *
 *	Buzzer Module
 *
 *  Created on: 21/apr/2015
 *      Author: Federico Iannucci
 */

#include "stm32f4xx_helpers.h"

/** Global variables *******************************************/
uint64_t globalBuzzerTick;
TIM_HandleTypeDef BuzzerTimHandle;

/**
 * @brief Initialization
 */
void TIMx_BUZZER_Init() {
	globalBuzzerTick = 0;
	/* Configure timer */
	BuzzerTimHandle.Instance = TIMx_BUZZER;

	BuzzerTimHandle.Init.Prescaler = 0; // (84 Mhz)
	BuzzerTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	BuzzerTimHandle.Init.Period = TIMx_BUZZER_PERIOD - 1;
	BuzzerTimHandle.Init.ClockDivision = 0;

	if (HAL_TIM_Base_Init(&BuzzerTimHandle) != HAL_OK) {
		//Error
	}

	GPIO_InitTypeDef GPIO_Init;

	// Enable peripheral clock
	BUZZER_CLK_ENABLE();

	GPIO_Init.Pin = BUZZER_PIN;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(BUZZER_PORT, &GPIO_Init);

	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

/**
 * @ brief Buzz the buzzer.
 * @param frequency : The frequency to buzz.
 * @param duration 	: The duration of the buzz in ms.
 */
void buzz(uint32_t frequency, uint32_t duration)
{
	uint32_t delayValue = (500000 / frequency) / 2; // calculate the delay value between transitions splitted by 2
	uint64_t numCycles = frequency * duration / 1000; // calculate the number of cycles for proper timing
	//// multiply frequency, which is really cycles per second, by the number of seconds to
	//// get the total number of cycles to produce
	for (uint64_t i = 0; i < numCycles; i++) { // for the calculated length of time...
		HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET); // write the buzzer pin high to push out the diaphram
		buzz_sleep(delayValue);
		HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET); // write the buzzer pin low to pull back the diaphram
		buzz_sleep(delayValue);
	}
}

/**
 * @brief Stop the execution for duration usec/2.
 * @param duration : The pause duration express in usec/2.
 */
void buzz_sleep(uint32_t duration)
{
	uint64_t globalBuzzerTickstart = 0;
	globalBuzzerTickstart = globalBuzzerTick;

	/* Start the timer on Channel1 */
	if (HAL_TIM_Base_Start_IT(&BuzzerTimHandle) != HAL_OK) {
		/* Starting Error */
	}

	/* Wait */
	while ((globalBuzzerTick - globalBuzzerTickstart) < duration) {
	}

    /* Stop the timer on Channel1 */
	if (HAL_TIM_Base_Stop_IT(&BuzzerTimHandle) != HAL_OK) {
		/* Starting Error */
	}
}

// activate the interrupt on the TIM2
void TIM2_IRQHandler() {
	HAL_TIM_IRQHandler(&BuzzerTimHandle);
}

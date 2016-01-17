/*
 * stm32f4xx_helper_timers.c
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#include "stm32f4xx_helper_timers.h"

TIM_HandleTypeDef		TimSysHandle;
TIM_HandleTypeDef		TimPWMHandle;
TIM_OC_InitTypeDef		TimPWMOcHandle;

/************************************************
 *
 * TIMx_SYS Section
 *
 */

/**
 * @brief Init the timer specified into the helper file
 * @param period: the period in ms
 *
 */
void TIMx_SYS_Init(int period)
{
	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
	uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;

	/* Set TIMx instance */
	TimSysHandle.Instance = TIMx_SYS;

	TimSysHandle.Init.Period = 10*period - 1;
	TimSysHandle.Init.Prescaler = uwPrescalerValue;
	TimSysHandle.Init.ClockDivision = 0;
	TimSysHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

	// inner init
	if(HAL_TIM_Base_Init(&TimSysHandle) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIMx_BUZZER)
	{
		// INIT THE TIMER FOR THE BUZZER

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* TIMx Peripheral clock enable */
		TIMx_BUZZER_CLK_ENABLE();

		/*##-2- Configure the NVIC for TIMx ########################################*/
		/* Set Interrupt Group Priority */
		HAL_NVIC_SetPriority(TIMx_BUZZER_IRQn, 0x04, 0);

		/* Enable the TIMx global Interrupt */
		HAL_NVIC_EnableIRQ(TIMx_BUZZER_IRQn);
		return;
	}

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	/*##-2- Configure the NVIC for TIMx ########################################*/
	/* Set Interrupt Group Priority */
	HAL_NVIC_SetPriority(TIMx_SYS_IRQn, 5, 1);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIMx_SYS_IRQn);
}

void TIMx_SYS_Start(void)
{
	// init the 1s timer
	if(HAL_TIM_Base_Start_IT(&TimSysHandle) != HAL_OK)
	{
		/* Starting Error */
		Error_Handler();
	}
}



/************************************************
 *
 * PWM Section
 *
 */

void TIMx_PWM_Init(TIMx_PWM_Handle *tph)
{
	/* Compute the prescaler value to have TIM3 counter clock equal to 1 Mhz */
	uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 1000000) - 1;

	tph->level = 0.05f;

	/* Set TIMx instance */
	TimPWMHandle.Instance = TIMx_PWM;
	TimPWMHandle.Init.Period = 20 - 1;
	TimPWMHandle.Init.Prescaler = uwPrescalerValue;
	TimPWMHandle.Init.ClockDivision = 0;
	TimPWMHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

	// inner init
	if(HAL_TIM_PWM_Init(&TimPWMHandle) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	TimPWMOcHandle.OCMode = TIM_OCMODE_PWM1;
	TimPWMOcHandle.Pulse = (int)(tph->level*20.0f - 1);
	TimPWMOcHandle.OCFastMode = TIM_OCFAST_ENABLE;
	TimPWMOcHandle.OCPolarity = TIM_OCPOLARITY_HIGH;

	HAL_TIM_PWM_ConfigChannel(&TimPWMHandle, &TimPWMOcHandle, TIM_CHANNEL_2);

	HAL_TIM_PWM_Start(&TimPWMHandle, TIM_CHANNEL_2);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_PWM_CLK_ENABLE();

  /* Enable GPIO channels Clock */
  TIMx_PWM_CHANNEL_GPIO_PORT();

  /* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
  GPIO_InitStruct.Pin = PWM_GPIO_PIN_CHANNEL2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF_TIMx_PWM;
  HAL_GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStruct);
}

void TIMx_PWM_IncrementPulse(TIMx_PWM_Handle *tph)
{
	tph->level += 0.05f;

	if (tph->level == 1.05f)
	{
		tph->level = 1.00f;
	}

	TIMx_PWM_ResetPulse(tph);
}

void TIMx_PWM_DecrementPulse(TIMx_PWM_Handle *tph)
{
	tph->level -= 0.05f;

	if (tph->level == 0)
	{
		tph->level = 0.05;
	}

	TIMx_PWM_ResetPulse(tph);
}

void TIMx_PWM_ResetPulse(TIMx_PWM_Handle *tph)
{
	TimPWMOcHandle.Pulse = (int)(tph->level*20.0f - 1);

	HAL_TIM_PWM_Stop(&TimPWMHandle, TIM_CHANNEL_2);

	HAL_TIM_PWM_DeInit(&TimPWMHandle);

	if(HAL_TIM_PWM_Init(&TimPWMHandle) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	HAL_TIM_PWM_ConfigChannel(&TimPWMHandle, &TimPWMOcHandle, TIM_CHANNEL_2);

	HAL_TIM_PWM_Start(&TimPWMHandle, TIM_CHANNEL_2);
}

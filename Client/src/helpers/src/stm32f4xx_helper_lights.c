/*
 * stm32f4xx_helper_lights.c
 *
 *  Created on: 05/giu/2015
 *      Author: giovannidifiore
 */

#include "stm32f4xx_helper_lights.h"

void LIGHT_Init(GPIOLight_TypeDef light)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_LED Clock */
  if (light.port == GPIOC)
  {
	  __HAL_RCC_GPIOC_CLK_ENABLE();
  }

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = light.pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(light.port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(light.port, light.pin, GPIO_PIN_RESET);
}

void LIGHT_TurnOn(GPIOLight_TypeDef light)
{
	HAL_GPIO_WritePin(light.port, light.pin, GPIO_PIN_SET);
}

void LIGHT_TurnOff(GPIOLight_TypeDef light)
{
	HAL_GPIO_WritePin(light.port, light.pin, GPIO_PIN_RESET);
}

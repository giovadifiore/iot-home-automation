/*
 * stm32f4xx_helper_errors.c
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#include "stm32f4xx_helper_errors.h"

void Error_Handler(void)
{
  /* Turn LED5 on */
  BSP_LED_On(LED5);
  while(1)
  {
  }
}

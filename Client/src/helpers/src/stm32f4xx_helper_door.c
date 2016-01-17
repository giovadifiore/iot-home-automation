/*
 * stm32f4xx_helper_door.c
 *
 *  Created on: 04/giu/2015
 *      Author: giovannidifiore
 */

#include "stm32f4xx_helper_door.h"
#include "stm32f4xx_helpers.h"

static float steadyAxe = 0;

void DOOR_Init()
{
	// Init the LIS3DSH
	SPIx_LIS3DSH_Init();

	// Update the steadyAxe
	LIS3DSH_AxesOutputTypeDef accelerometerData;
	LIS3DSH_readAxes(&accelerometerData);
	DOOR_UpdateSteadyAxe(accelerometerData.y);
}

void DOOR_UpdateSteadyAxe(const float axe)
{
	steadyAxe = axe;
}

uint8_t DOOR_checkOpen(const float currentAxe)
{
	if (steadyAxe==0)
	{
		return 0;
	}
	if (currentAxe < (steadyAxe - DOOR_AXE_DELTA))
	{
		return 1;
	}
	return 0;
}

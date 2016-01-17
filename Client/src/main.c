//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "parson/parson.h"
#include "usbd_cdc.h"
#include "stm32f4xx_helpers.h"
#include "commands.h"
#include "signals.h"

/* Some useful defines and macros ----------------------------------------------------------------------- */
#define READY_TO_COMMANDS_LED	LED4
#define HOST_PAIRING_LED		LED6
#define READY_FOR_COMMANDS()	USB_DeviceIsConnected() && PROTOCOL_HostIsPaired()
#define ROOM_LIGHT_PORT			GPIOC
#define ROOM_LIGHT_PIN			GPIO_PIN_1

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/* Global status variables ------------------------------------------------------------------------------- */
float 						socTemp = 0;
LIS3DSH_AxesOutputTypeDef 	accelerometerData;
DOORStatus_TypeDef 			doorStatus = DOOR_CLOSE; // this initialize the door closed as default
DOORAutoLight_TypeDef 		roomAutoLight = DOOR_AUTOLIGHT_OFF; // this initialize the autolight feature off

/* Internal variables ------------------------------------------------------------------------------------ */
GPIOLight_TypeDef roomLight;
TIMx_PWM_Handle PWMSignalHandle;
JsonFrame_TypeDef outgoingFrame, ingoingFrame;
uint8_t timCycle = 0;
extern uint64_t globalBuzzerTick;

/* User functions (only in main) ------------------------------------------------------------------------- */
void TIM_TwoPeriodsElapsedCallback(TIM_HandleTypeDef* TimHandle);
void TIM_FourPeriodsElapsedCallback(TIM_HandleTypeDef* TimHandle);
void VisualEffect_TurnAllLedOnOff(uint32_t);
void doPairWithHost(JsonFrame_TypeDef*);

int main(int argc, char* argv[])
{
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

	// Init USB
	USB_CDC_Init();

	// Init LED and USER Button
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	// Init TEMPSENS
	ADCx_TEMPSENS_Init();

	// Init the LIS3DSH
	SPIx_LIS3DSH_Init();

	// Perform an initial read and init the DOOR watcher
	LIS3DSH_readAxes(&accelerometerData);
	DOOR_UpdateSteadyAxe(accelerometerData.y);

	// Init and start timer with a period of 250ms
	CAL_TIM_Init(250);
	TIMx_SYS_Start();

	// Init the signal on the output pin specified
	TIMx_PWM_Init(&PWMSignalHandle);

	// Init the outgoing & ingoing frame
	JSON_InitFrame(&outgoingFrame);
	JSON_InitFrame(&ingoingFrame);

	// Init the roomLight
	roomLight.port = ROOM_LIGHT_PORT;
	roomLight.pin = ROOM_LIGHT_PIN;
	LIGHT_Init(roomLight);

	// Init the buzzer
	TIMx_BUZZER_Init();

	// wait for the USB Connection
	while(!USB_DeviceIsConnected()) {};

	HAL_Delay(100);

	// get an interrupt on the user button
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	char* receiving_buffer = (char*) malloc(PROTOCOL_FRAME_MAXLENGTH*sizeof(char));

	while(1)
	{
		// wait for the USB Connection
		while(!USB_DeviceIsConnected());

		// now we have to wait for the pair request
		while(!PROTOCOL_HostIsPaired())
		{
			// Receive the first frame
			USB_readLine(receiving_buffer);

			FrameCommand_TypeDef command = JSON_ReadCommand(receiving_buffer);

			if (command==FRAME_CMD_PAIR_REQUEST)
			{
				// perform the pair with the host
				doPairWithHost(&outgoingFrame);
			}
		}

		// Wait for commands...
		USB_readLine(receiving_buffer);

		// A disconnection could happen when reading commands...
		if (READY_FOR_COMMANDS())
		{
			// open the frame buffer
			JSON_OpenFrameBuffer(&ingoingFrame, receiving_buffer);

			switch(JSON_GetFrameCommand(&ingoingFrame))
			{
				case FRAME_CMD_GET_TEMPSENS_VALUE:
					doCommandGetTempsensValue(&outgoingFrame, socTemp);
				break;
				case FRAME_CMD_GET_ACCELERO_AXES:
					doCommandGetAcceleroAxesValue(&outgoingFrame, accelerometerData);
				break;
				case FRAME_CMD_GET_DOOR_STATUS:
					doCommandGetDoorStatus(&outgoingFrame, doorStatus);
				break;
				case FRAME_CMD_TURN_LIGHT_ON:
					doCommandTurnLightOn(&outgoingFrame, roomLight);
				break;
				case FRAME_CMD_DOOR_AUTOLIGHT_ON:
					roomAutoLight = DOOR_AUTOLIGHT_ON;
					doCommandSendDoorAutolightStatus(&outgoingFrame, roomAutoLight);
				break;
				case FRAME_CMD_DOOR_AUTOLIGHT_OFF:
					roomAutoLight = DOOR_AUTOLIGHT_OFF;
					doCommandSendDoorAutolightStatus(&outgoingFrame, roomAutoLight);
				break;
				case FRAME_CMD_GET_DOOR_AUTOLIGHT_STATUS:
					doCommandSendDoorAutolightStatus(&outgoingFrame, roomAutoLight);
				break;
				case FRAME_CMD_GET_LED_STATUS:
					doCommandGetLedStatus(&ingoingFrame, &outgoingFrame);
				break;
				case FRAME_CMD_SET_LED_STATUS:
					doCommandUpdateLedStatus(&ingoingFrame, &outgoingFrame);
				break;
				case FRAME_CMD_START_ALARM:
					doCommandStartAlarm(&outgoingFrame);
				break;
				/*
				case FRAME_CMD_INCREMENT_LED_BRIGHTNESS:
					//TIMx_PWM_IncrementPulse(&PWMSignalHandle);
				break;
				case FRAME_CMD_DECREMENT_LED_BRIGHTNESS:
					//TIMx_PWM_DecrementPulse(&PWMSignalHandle);
				break;
				*/
				default:
					JSON_BuildGenericCmdNotFoundFrame(&outgoingFrame);
					USB_writeLine(JSON_GetData(&outgoingFrame));
				break;
			}

			// close the ingoing buffer
			JSON_CloseFrameBuffer(&ingoingFrame);
		}
		else
		{
			// a disconnection has happen
			FrameCommand_TypeDef command = JSON_ReadCommand(receiving_buffer);

			if (command==FRAME_CMD_PAIR_REQUEST)
			{
				// perform the pair with the host
				doPairWithHost(&outgoingFrame);
			}
		}

		__NOP(); // anything else

	} // ends the main-loop

	JSON_DeInitFrame(&outgoingFrame);
}

void doPairWithHost(JsonFrame_TypeDef* outgoingFrame)
{
	// Send the connected signal
	JSON_BuildPairedFrame(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));

	// Set Paired
	PROTOCOL_SetHostPaired();

	// Turn off the Pairing LED
	BSP_LED_Off(HOST_PAIRING_LED);

	// When the pairing is done, all led will be turned on for 1s
	VisualEffect_TurnAllLedOnOff(3000);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* TimHandle)
{
	// Attention: this task will be executed ONLY when the buzzer is active
	if (TimHandle->Instance == TIMx_BUZZER)
	{
		globalBuzzerTick++;
		return;
	}

	// Following tasks will be executed every 250ms
	if (USB_DeviceIsConnected() && !PROTOCOL_HostIsPaired())
	{
		BSP_LED_Toggle(HOST_PAIRING_LED);
	}

	// 2) Read accelerometer data
	if (READY_FOR_COMMANDS())
		LIS3DSH_readAxes(&accelerometerData);

	// last) manage the two periods callback
	if (timCycle==1 || timCycle==3)
	{
		TIM_TwoPeriodsElapsedCallback(TimHandle);
	}
	if (timCycle==3)
	{
		TIM_FourPeriodsElapsedCallback(TimHandle);
	}

	timCycle = (timCycle+1)%4;
}

void TIM_TwoPeriodsElapsedCallback(TIM_HandleTypeDef* TimHandle)
{
	// 1) Blink the RED led while waiting for the USB connection
	if (!USB_DeviceIsConnected())
	{
		BSP_LED_Toggle(USB_STATUS_LED);
	}

	// 2) Check if the door is open
	if (READY_FOR_COMMANDS())
	{
		DOORStatus_TypeDef tmpDoorStatus = doorStatus;
		doorStatus = DOOR_checkOpen(accelerometerData.y);
		// if a change is detected
		if (tmpDoorStatus != doorStatus)
		{
			// the door status is changed
			// do we have auto light turn on enabled?
			if (roomAutoLight==DOOR_AUTOLIGHT_ON && doorStatus==DOOR_OPEN)
			{
				LIGHT_TurnOn(roomLight);
			} else if (roomAutoLight==DOOR_AUTOLIGHT_ON && doorStatus==DOOR_CLOSE)
			{
				LIGHT_TurnOff(roomLight);
			}
			// sends a signal with the door status change event
			doSignalDoorStatusChanged(&outgoingFrame, doorStatus);
		}
	}
}

void TIM_FourPeriodsElapsedCallback(TIM_HandleTypeDef* TimHandle)
{
	// 1) Update the global socTemp variable
	if (READY_FOR_COMMANDS())
		ADCx_TEMPSENS_Read_IT();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	// update the global socTemp
	socTemp = ADCx_TEMPSENS_GetValue();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// send the async signal to the host
	doSignalUserBtnPressed(&outgoingFrame);

	// clear repeated events.
	HAL_NVIC_ClearPendingIRQ(KEY_BUTTON_EXTI_IRQn);
}

void VisualEffect_TurnAllLedOnOff(uint32_t period)
{
	BSP_LED_On(LED3);
	BSP_LED_On(LED4);
	BSP_LED_On(LED5);
	BSP_LED_On(LED6);

	HAL_Delay(period);

	BSP_LED_Off(LED3);
	BSP_LED_Off(LED4);
	BSP_LED_Off(LED5);
	BSP_LED_Off(LED6);
}

void USB_DeviceConnectedUserHook(PCD_HandleTypeDef *hpcd)
{
	// turn the USB_STATUS_LED on (stucked)
	BSP_LED_Off(USB_STATUS_LED);
}

void USB_DeviceDisconnectedUserHook(PCD_HandleTypeDef *hpcd)
{
	// Turn off the command led
	//BSP_LED_Off(READY_TO_COMMANDS_LED);

	// Turn off all leds
	BSP_LED_Off(LED3);
	BSP_LED_Off(LED4);
	BSP_LED_Off(LED5);
	BSP_LED_Off(LED6);

	// Unpair a previous paired host
	PROTOCOL_SetHostNotPaired();

	// Turn off the pairing led
	//BSP_LED_Off(HOST_PAIRING_LED);
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

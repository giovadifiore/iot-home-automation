/*
 * commands.c
 *
 *  Created on: 04/giu/2015
 *      Author: giovannidifiore
 */

#include "commands.h"

void doCommandGetTempsensValue(JsonFrame_TypeDef* outgoingFrame, float socTemp)
{
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_TEMPSENS_VALUE);
	JSON_SetMessageValueNumber(outgoingFrame, socTemp);
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandGetAcceleroAxesValue(JsonFrame_TypeDef* outgoingFrame, LIS3DSH_AxesOutputTypeDef axesValue)
{
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_ACCELERO_AXES);

	char* names[] = {
		"value_x",
		"value_y",
		"value_z"
	};

	double values[] = {
		axesValue.x,
		axesValue.y,
		axesValue.z
	};

	JSON_SetMessageMapOfNumbers(outgoingFrame, (const char**)names, values, 3);
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandGetDoorStatus(JsonFrame_TypeDef* outgoingFrame, DOORStatus_TypeDef doorStatus)
{
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_DOOR_STATUS);
	if (doorStatus==DOOR_CLOSE)
	{
		JSON_SetMessageValueString(outgoingFrame, "close");
	}
	else if (doorStatus==DOOR_OPEN)
	{
		JSON_SetMessageValueString(outgoingFrame, "open");
	}
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandTurnLightOn(JsonFrame_TypeDef* outgoingFrame, GPIOLight_TypeDef light)
{
	// turn the light on
	LIGHT_TurnOn(light);

	// build the reply frame
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_LIGHT_STATUS);
	JSON_SetMessageValueString(outgoingFrame, "on");
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandSendDoorAutolightStatus(JsonFrame_TypeDef* outgoingFrame, DOORAutoLight_TypeDef autoLight)
{
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_DOOR_AUTOLIGHT_STATUS);
	if (autoLight==DOOR_AUTOLIGHT_ON)
	{
		JSON_SetMessageValueString(outgoingFrame, "on");
	}
	else if (autoLight==DOOR_AUTOLIGHT_OFF)
	{
		JSON_SetMessageValueString(outgoingFrame, "off");
	}
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandGetLedStatus(const JsonFrame_TypeDef* ingoingFrame, JsonFrame_TypeDef* outgoingFrame)
{
	// we have to understand which led the status is requested for
	char requestedLed[PROTOCOL_JSON_FIELD_MAXLENGTH];
	JSON_GetMessageString(ingoingFrame, "led", requestedLed);

	// hold the ledstate in this GPIO_PinState variable
	GPIO_PinState ledState = GPIO_PIN_RESET;

	// now requestedLed contains a string with the requested led
	if (strcmp(requestedLed, "ambra") == 0)
	{
		// the led is ambra
		ledState = HAL_GPIO_ReadPin(LED3_GPIO_PORT, LED3_PIN);
	} else if (strcmp(requestedLed, "red") == 0)
	{
		// the led is red
		ledState = HAL_GPIO_ReadPin(LED5_GPIO_PORT, LED5_PIN);
	} else if (strcmp(requestedLed, "blue") == 0)
	{
		// the led is blue
		ledState = HAL_GPIO_ReadPin(LED6_GPIO_PORT, LED6_PIN);
	} else if (strcmp(requestedLed, "green") == 0)
	{
		// the led is green
		ledState = HAL_GPIO_ReadPin(LED4_GPIO_PORT, LED4_PIN);
	}

	// prepare the outgoing frame
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_LED_STATUS);
	JSON_SetMessageString(outgoingFrame, "led", requestedLed);

	if (ledState==GPIO_PIN_SET)
	{
		JSON_SetMessageValueString(outgoingFrame, "on");
	}
	else if (ledState==GPIO_PIN_RESET)
	{
		JSON_SetMessageValueString(outgoingFrame, "off");
	}

	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandUpdateLedStatus(const JsonFrame_TypeDef* ingoingFrame, JsonFrame_TypeDef* outgoingFrame)
{
	// we have to understand which led the status update is requested for
	char requestedLed[PROTOCOL_JSON_FIELD_MAXLENGTH];
	char updateTo[PROTOCOL_JSON_FIELD_MAXLENGTH];

	// red the led
	JSON_GetMessageString(ingoingFrame, "led", requestedLed);

	// red the value to change to
	JSON_GetMessageString(ingoingFrame, "value", updateTo);

	// hold the led in this GPIO_PinState variable
	Led_TypeDef led = 0xFF; // default value

	// now requestedLed contains a string with the requested led
	if (strcmp(requestedLed, "ambra") == 0)
	{
		// the led is ambra
		led = LED3;
		//ledState = HAL_GPIO_ReadPin(LED4_GPIO_PORT, LED4_PIN);
	} else if (strcmp(requestedLed, "red") == 0)
	{
		// the led is red
		led = LED5;
		//ledState = HAL_GPIO_ReadPin(LED5_GPIO_PORT, LED5_PIN);
	} else if (strcmp(requestedLed, "blue") == 0)
	{
		// the led is blue
		led = LED6;
		//ledState = HAL_GPIO_ReadPin(LED6_GPIO_PORT, LED6_PIN);
	} else if (strcmp(requestedLed, "green") == 0)
	{
		// the led is green
		led = LED4;
		//ledState = HAL_GPIO_ReadPin(LED3_GPIO_PORT, LED3_PIN);
	}

	// prepare the outgoing frame and perform the action
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_LED_STATUS);
	JSON_SetMessageString(outgoingFrame, "led", requestedLed);

	if (strcmp(updateTo, "on")==0)
	{
		BSP_LED_On(led);
		JSON_SetMessageValueString(outgoingFrame, "on");
	}
	else if (strcmp(updateTo, "off")==0)
	{
		BSP_LED_Off(led);
		JSON_SetMessageValueString(outgoingFrame, "off");
	}

	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

void doCommandStartAlarm(JsonFrame_TypeDef* outgoingFrame)
{
	JSON_CreateCmdReplyFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_CMD_REPLY_ALARM_STARTED);
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);

	// Play alarm (this is blocking... no other commands can be received until the alarm ends)
	play_song(0);
}

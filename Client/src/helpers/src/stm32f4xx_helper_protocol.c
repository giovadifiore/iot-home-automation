/*
 * stm32f4xx_helper_protocol.c
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#include "stm32f4xx_helper_protocol.h"

HostCommunicationStatus_TypeDef hCommStatus = HOST_COMMUNICATION_NOT_PAIRED;

const char* frameTargetStrings[] = {
	"BOARD",
	"HOST",
	"USER",
	NULL
};

const char* frameCommandStrings[] = {
	"CMD_PAIR_REQUEST",
	"CMD_GET_TEMPSENS_VALUE",
	"CMD_INCREMENT_LED_BRIGHTNESS",
	"CMD_DECREMENT_LED_BRIGHTNESS",
	"CMD_GET_ACCELERO_AXES",
	"CMD_GET_DOOR_STATUS",
	"CMD_TURN_LIGHT_ON",
	"CMD_DOOR_AUTOLIGHT_ON",
	"CMD_DOOR_AUTOLIGHT_OFF",
	"CMD_GET_DOOR_AUTOLIGHT_STATUS",
	"CMD_GET_LED_STATUS",
	"CMD_SET_LED_STATUS",
	"CMD_START_ALARM",
	NULL
};

const char* frameCommandReplyStrings[] = {
	"PAIR_COMPLETE",
	"CMD_NOT_FOUND",
	"TEMPSENS_VALUE",
	"ACCELERO_AXES",
	"DOOR_STATUS",
	"LIGHT_STATUS",
	"DOOR_AUTOLIGHT_STATUS",
	"LED_STATUS",
	"ALARM_STARTED",
	NULL
};

const char* frameSignalStrings[] = {
	"SIG_DOOR_STATUS_CHANGED",
	"SIG_USR_BTN_PRESSED",
	NULL
};

void PROTOCOL_InitFrame(Frame_TypeDef* frame)
{
	frame->data = (char*)malloc(PROTOCOL_FRAME_MAXLENGTH*sizeof(char));
}

char* PROTOCOL_GetData(const Frame_TypeDef* frame)
{
	return frame->data;
}

void PROTOCOL_DeInitFrame(Frame_TypeDef* frame)
{
	free(frame->data);
}

FrameCommand_TypeDef PROTOCOL_ReadCommand(const char* ptrBuffer)
{
	int i = 0, found = 0;
	while (!found && frameCommandStrings[i] != NULL)
	{
		if (strcmp(ptrBuffer, frameCommandStrings[i])==0)
		{
			found = 1;
		} else i++;
	}
	if (found)
	{
		return i;
	} else return -1;
}

FrameTarget_TypeDef PROTOCOL_ReadTarget(const char* ptrBuffer)
{
	int i = 0, found = 0;
	while (!found && frameCommandStrings[i] != NULL)
	{
		if (strcmp(ptrBuffer, frameTargetStrings[i])==0)
		{
			found = 1;
		} else i++;
	}
	if (found)
	{
		return i;
	} else return -1;
}

uint8_t PROTOCOL_HostIsPaired()
{
	return hCommStatus == HOST_COMMUNICATION_PAIRED;
}

void PROTOCOL_SetHostPaired()
{
	hCommStatus = HOST_COMMUNICATION_PAIRED;
}

void PROTOCOL_SetHostNotPaired()
{
	hCommStatus = HOST_COMMUNICATION_NOT_PAIRED;
}

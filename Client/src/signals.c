/*
 * signals.c
 *
 *  Created on: 05/giu/2015
 *      Author: giovannidifiore
 */

#include "signals.h"

void doSignalDoorStatusChanged(JsonFrame_TypeDef* outgoingFrame, const DOORStatus_TypeDef doorStatus)
{
	JSON_CreateSignalFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_SIGNAL_DOOR_STATUS_CHANGED);
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

void doSignalUserBtnPressed(JsonFrame_TypeDef* outgoingFrame)
{
	JSON_CreateSignalFrameBuffer(outgoingFrame, FRAME_TARGET_USER, FRAME_SIGNAL_USR_BTN_PRESSED);
	JSON_SerializeFrameBuffer(outgoingFrame);
	USB_writeLine(JSON_GetData(outgoingFrame));
	JSON_CloseFrameBuffer(outgoingFrame);
}

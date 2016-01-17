/*
 * stm32f4xx_helper_protocol_json.c
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#include "stm32f4xx_helper_protocol_json.h"
#include "parson/parson.h"

#include "iothings_def.h"

char* JSON_GetData(const JsonFrame_TypeDef* jsonFrame)
{
	return PROTOCOL_GetData(jsonFrame->parent);
}

void JSON_InitFrame(JsonFrame_TypeDef* jsonFrame)
{
	jsonFrame->parent = (Frame_TypeDef*) malloc(sizeof(Frame_TypeDef));
	PROTOCOL_InitFrame(jsonFrame->parent);
}

void JSON_DeInitFrame(JsonFrame_TypeDef* jsonFrame)
{
	free(jsonFrame->parent);
}

void JSON_OpenFrameBuffer(JsonFrame_TypeDef* jsonFrame, const char* jsonBuffer)
{
	jsonFrame->jsonPacket = json_parse_string(jsonBuffer);
}

FrameCommand_TypeDef JSON_GetFrameCommand(const JsonFrame_TypeDef* jsonFrame)
{
	// return if there isn't an open packet
	if (jsonFrame->jsonPacket==NULL)
	{
		return 0;
	}

	char cmdBuffer[PROTOCOL_FRAME_CMD_MAXLENGTH];

	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);

	const char* cmdGot = json_object_get_string(rootObject, PROTOCOL_CMD_NAME);

	strcpy(cmdBuffer, cmdGot);

	return PROTOCOL_ReadCommand(cmdBuffer);
}

void JSON_GetMessageString(const JsonFrame_TypeDef* jsonFrame, const char* name, char* strBuffer)
{
	if (jsonFrame->jsonPacket == NULL)
	{
		return;
	}

	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);
	JSON_Value* messageValue = json_object_get_value(rootObject, "message");
	JSON_Object* messageObject = json_value_get_object(messageValue);

	// read the requested string
	const char* strGot = json_object_get_string(messageObject, name);

	// copy the string inside the buffer
	strcpy(strBuffer, strGot);
}

double JSON_GetMessageNumber(const JsonFrame_TypeDef* jsonFrame, const char* name)
{
	if (jsonFrame->jsonPacket == NULL)
	{
		return -1;
	}

	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);
	JSON_Value* messageValue = json_object_get_value(rootObject, "message");
	JSON_Object* messageObject = json_value_get_object(messageValue);

	return json_object_get_number(messageObject, name);
}

FrameTarget_TypeDef JSON_GetFrameTarget(const JsonFrame_TypeDef* jsonFrame)
{
	// return if there isn't an open packet
	if (jsonFrame->jsonPacket==NULL)
	{
		return 0;
	}

	char cmdBuffer[PROTOCOL_FRAME_TARGET_MAXLENGTH];

	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);

	const char* targetGot = json_object_get_string(rootObject, PROTOCOL_TARGET_NAME);

	strcpy(cmdBuffer, targetGot);

	return PROTOCOL_ReadTarget(cmdBuffer);
}

void JSON_CloseFrameBuffer(JsonFrame_TypeDef* jsonFrame)
{
	if (jsonFrame->jsonPacket==NULL)
	{
		return;
	}
	json_value_free(jsonFrame->jsonPacket);
}

FrameCommand_TypeDef JSON_ReadCommand(const char* jsonBuffer)
{
	char cmdBuffer[PROTOCOL_FRAME_CMD_MAXLENGTH];

	JSON_Value* jsonPacket = json_parse_string(jsonBuffer);
	JSON_Object* rootObject = json_value_get_object(jsonPacket);

	const char* cmdGot = json_object_get_string(rootObject, PROTOCOL_CMD_NAME);

	strcpy(cmdBuffer, cmdGot);

	json_value_free(jsonPacket);

	return PROTOCOL_ReadCommand(cmdBuffer);
}

void JSON_BuildPairedFrame(JsonFrame_TypeDef* jsonFrame)
{
	JSON_Value* jsonPacket = json_value_init_object();
	JSON_Object* rootObject = json_value_get_object(jsonPacket);

	json_object_set_string(rootObject, PROTOCOL_TARGET_NAME, PROTOCOL_FRAME_TARGET_STR(FRAME_TARGET_HOST));
	json_object_set_string(rootObject, PROTOCOL_CMD_REPLY_NAME, PROTOCOL_FRAME_CMD_REPLY_STR(FRAME_CMD_REPLY_PAIR_COMPLETE));

	JSON_Value* messageValue = json_value_init_object();
	JSON_Object* messageObject = json_value_get_object(messageValue);

	json_object_set_string(messageObject, "device_name", DEVICE_NAME);
	json_object_set_string(messageObject, "room_type", ROOM_TYPE);
	json_object_set_string(messageObject, "room_name", ROOM_NAME);
	json_object_set_string(messageObject, "room_icon", ROOM_ICON);
	json_object_set_value(rootObject, "message", messageValue);

	char* serialized = json_serialize_to_string(jsonPacket);

	strcpy(jsonFrame->parent->data, serialized);

	json_free_serialized_string(serialized);
	json_value_free(jsonPacket);
}

void JSON_CreateCmdReplyFrameBuffer(JsonFrame_TypeDef* jsonFrame, const FrameTarget_TypeDef target, const FrameCommandReply_TypeDef cmd)
{
	jsonFrame->jsonPacket = json_value_init_object();
	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);

	json_object_set_string(rootObject, PROTOCOL_TARGET_NAME, PROTOCOL_FRAME_TARGET_STR(target));
	json_object_set_string(rootObject, PROTOCOL_CMD_REPLY_NAME, PROTOCOL_FRAME_CMD_REPLY_STR(cmd));

	JSON_Value* messageValue = json_value_init_object();

	json_object_set_value(rootObject, "message", messageValue);
}

void JSON_CreateSignalFrameBuffer(JsonFrame_TypeDef* jsonFrame, const FrameTarget_TypeDef target, const FrameSignal_TypeDef sig)
{
	jsonFrame->jsonPacket = json_value_init_object();
	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);

	json_object_set_string(rootObject, PROTOCOL_TARGET_NAME, PROTOCOL_FRAME_TARGET_STR(target));
	json_object_set_string(rootObject, PROTOCOL_SIGNAL_NAME, PROTOCOL_FRAME_SIGNAL_STR(sig));

	JSON_Value* messageValue = json_value_init_object();

	json_object_set_value(rootObject, "message", messageValue);
}

void JSON_SerializeFrameBuffer(JsonFrame_TypeDef* jsonFrame)
{
	char* serialized = json_serialize_to_string(jsonFrame->jsonPacket);

	strcpy(jsonFrame->parent->data, serialized);
}

void JSON_SetMessageValueNumber(JsonFrame_TypeDef* jsonFrame, const double number)
{
	if (jsonFrame->jsonPacket == NULL)
	{
		return;
	}
	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);
	JSON_Value* messageValue = json_object_get_value(rootObject, "message");
	JSON_Object* messageObject = json_value_get_object(messageValue);

	json_object_set_number(messageObject, "value", number);
}

void JSON_SetMessageValueString(JsonFrame_TypeDef* jsonFrame, const char* string)
{
	JSON_SetMessageString(jsonFrame, "value", string);
}

void JSON_SetMessageString(JsonFrame_TypeDef* jsonFrame, const char* name, const char* string)
{
	if (jsonFrame->jsonPacket == NULL)
	{
		return;
	}
	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);
	JSON_Value* messageValue = json_object_get_value(rootObject, "message");
	JSON_Object* messageObject = json_value_get_object(messageValue);

	json_object_set_string(messageObject, name, string);
}

void JSON_SetMessageMapOfNumbers(JsonFrame_TypeDef* jsonFrame, const char** names, const double* numbers, const double length)
{
	if (jsonFrame->jsonPacket == NULL)
	{
		return;
	}

	JSON_Object* rootObject = json_value_get_object(jsonFrame->jsonPacket);
	JSON_Value* messageValue = json_object_get_value(rootObject, "message");
	JSON_Object* messageObject = json_value_get_object(messageValue);

	uint16_t i = 0;

	for (i=0; i<length; i++)
	{
		json_object_set_number(messageObject, names[i], numbers[i]);
	}
}

void JSON_BuildGenericCmdNotFoundFrame(JsonFrame_TypeDef* jsonFrame)
{
	JSON_Value* jsonPayload = json_value_init_object();
	JSON_Object* rootObject = json_value_get_object(jsonPayload);

	json_object_set_string(rootObject, PROTOCOL_TARGET_NAME, PROTOCOL_FRAME_TARGET_STR(FRAME_TARGET_HOST));
	json_object_set_string(rootObject, PROTOCOL_CMD_REPLY_NAME, PROTOCOL_FRAME_CMD_REPLY_STR(FRAME_CMD_REPLY_NOT_FOUND));

	char* serialized = json_serialize_to_string(jsonPayload);

	strcpy(jsonFrame->parent->data, serialized);

	json_free_serialized_string(serialized);
	json_value_free(jsonPayload);
}

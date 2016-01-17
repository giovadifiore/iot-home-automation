/*
 * stm32f4xx_helper_protocol_json.h
 *
 *  Created on: 29/mag/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPER_PROTOCOL_JSON_H_
#define __STM32F4XX_HELPER_PROTOCOL_JSON_H_

#include "stm32f4xx_helper_protocol.h"
#include "parson/parson.h"

#define PROTOCOL_JSON_FIELD_MAXLENGTH	256

// Wrapper to the generic Frame_TypeDef frame
typedef struct {

	/* parent generic frame (containing char* data) ------ */
	Frame_TypeDef* parent;

	/* private json frame data ----------------------------*/
	JSON_Value* jsonPacket;

} JsonFrame_TypeDef;


void JSON_InitFrame(JsonFrame_TypeDef*);
void JSON_OpenFrameBuffer(JsonFrame_TypeDef*, const char*);
void JSON_DeInitFrame(JsonFrame_TypeDef*);

void JSON_CreateCmdReplyFrameBuffer(JsonFrame_TypeDef*, const FrameTarget_TypeDef target, const FrameCommandReply_TypeDef);
void JSON_CreateSignalFrameBuffer(JsonFrame_TypeDef*, const FrameTarget_TypeDef target, const FrameSignal_TypeDef);
void JSON_SetMessageValueNumber(JsonFrame_TypeDef*, const double number);
void JSON_SetMessageValueString(JsonFrame_TypeDef*, const char* string);
void JSON_SetMessageString(JsonFrame_TypeDef*, const char* name, const char* string);
void JSON_SetMessageMapOfNumbers(JsonFrame_TypeDef*, const char**, const double*, const double);
void JSON_SerializeFrameBuffer(JsonFrame_TypeDef*);

void JSON_CloseFrameBuffer(JsonFrame_TypeDef*);

FrameCommand_TypeDef JSON_GetFrameCommand(const JsonFrame_TypeDef*);
FrameTarget_TypeDef JSON_GetFrameTarget(const JsonFrame_TypeDef*);
void JSON_GetMessageString(const JsonFrame_TypeDef*, const char*, char*);
double JSON_GetMessageNumber(const JsonFrame_TypeDef*, const char*);

// This gives the serialized data
char* JSON_GetData(const JsonFrame_TypeDef*);

// Utility functions
FrameCommand_TypeDef JSON_ReadCommand(const char*);
void JSON_BuildPairedFrame(JsonFrame_TypeDef*);
void JSON_BuildGenericCmdNotFoundFrame(JsonFrame_TypeDef*);

#endif /* __STM32F4XX_HELPER_PROTOCOL_JSON_H_ */

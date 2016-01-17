/*
 * commands.h
 *
 *  Created on: 04/giu/2015
 *      Author: giovannidifiore
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "stm32f4xx_helpers.h"

void doCommandGetTempsensValue(JsonFrame_TypeDef* outgoingFrame, float socTemp);
void doCommandGetAcceleroAxesValue(JsonFrame_TypeDef* outgoingFrame, LIS3DSH_AxesOutputTypeDef axesValue);
void doCommandGetDoorStatus(JsonFrame_TypeDef* outgoingFrame, DOORStatus_TypeDef doorStatus);
void doCommandTurnLightOn(JsonFrame_TypeDef* outgoingFrame, GPIOLight_TypeDef light);
void doCommandSendDoorAutolightStatus(JsonFrame_TypeDef*, DOORAutoLight_TypeDef);
void doCommandGetLedStatus(const JsonFrame_TypeDef*, JsonFrame_TypeDef*);
void doCommandUpdateLedStatus(const JsonFrame_TypeDef*, JsonFrame_TypeDef*);
void doCommandStartAlarm(JsonFrame_TypeDef* outgoingFrame);

#endif /* COMMANDS_H_ */

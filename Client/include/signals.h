#ifndef SIGNALS_H_
#define SIGNALS_H_

#include "stm32f4xx_helpers.h"

void doSignalDoorStatusChanged(JsonFrame_TypeDef* outgoingFrame, const DOORStatus_TypeDef doorStatus);
void doSignalUserBtnPressed(JsonFrame_TypeDef* outgoingFrame);

#endif

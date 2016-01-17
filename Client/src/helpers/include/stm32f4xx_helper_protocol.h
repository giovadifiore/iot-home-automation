/*
 * stm32f4xx_helper_protocol.h
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPER_PROTOCOL_H_
#define __STM32F4XX_HELPER_PROTOCOL_H_

#include <stdint.h>
#include <stdlib.h>

#define PROTOCOL_TARGET_NAME	"target"
#define PROTOCOL_CMD_NAME		"cmd"
#define PROTOCOL_CMD_REPLY_NAME	"cmd_reply"
#define PROTOCOL_SIGNAL_NAME	"signal"
#define PROTOCOL_VALUE_EMPTY	""

/* Frame */
#define PROTOCOL_FRAME_MAXLENGTH		0xFF // 255
#define PROTOCOL_FRAME_CMD_MAXLENGTH	0x40 // 64
#define PROTOCOL_FRAME_TARGET_MAXLENGTH	0x40 // 64

typedef struct {
	char* data;
} Frame_TypeDef;

/* Define arrays for protocol strings */
#define __INNER_SIZE	16
extern const char* frameTargetStrings[__INNER_SIZE];
extern const char* frameCommandStrings[__INNER_SIZE];
extern const char* frameCommandReplyStrings[__INNER_SIZE];
extern const char* frameSignalStrings[__INNER_SIZE];

/* Frame Target */
#define PROTOCOL_FRAME_TARGET_STR(tgt)		frameTargetStrings[tgt]

typedef enum {
	FRAME_TARGET_BOARD = 0,
	FRAME_TARGET_HOST = 1,
	FRAME_TARGET_USER = 2
} FrameTarget_TypeDef;


/* Frame Command */
#define PROTOCOL_FRAME_CMD_STR(cmd)			frameCommandStrings[cmd]
typedef enum
{
	FRAME_CMD_PAIR_REQUEST = 0,
	FRAME_CMD_GET_TEMPSENS_VALUE = 1,
	FRAME_CMD_INCREMENT_LED_BRIGHTNESS = 2,
	FRAME_CMD_DECREMENT_LED_BRIGHTNESS = 3,
	FRAME_CMD_GET_ACCELERO_AXES = 4,
	FRAME_CMD_GET_DOOR_STATUS = 5,
	FRAME_CMD_TURN_LIGHT_ON = 6,
	FRAME_CMD_DOOR_AUTOLIGHT_ON = 7,
	FRAME_CMD_DOOR_AUTOLIGHT_OFF = 8,
	FRAME_CMD_GET_DOOR_AUTOLIGHT_STATUS = 9,
	FRAME_CMD_GET_LED_STATUS = 10,
	FRAME_CMD_SET_LED_STATUS = 11,
	FRAME_CMD_START_ALARM = 12
} FrameCommand_TypeDef;


/* Frame Command Reply */
#define PROTOCOL_FRAME_CMD_REPLY_STR(cmd)	frameCommandReplyStrings[cmd]
typedef enum
{
	FRAME_CMD_REPLY_PAIR_COMPLETE = 0,
	FRAME_CMD_REPLY_NOT_FOUND = 1,
	FRAME_CMD_REPLY_TEMPSENS_VALUE = 2,
	FRAME_CMD_REPLY_ACCELERO_AXES = 3,
	FRAME_CMD_REPLY_DOOR_STATUS = 4,
	FRAME_CMD_REPLY_LIGHT_STATUS = 5,
	FRAME_CMD_REPLY_DOOR_AUTOLIGHT_STATUS = 6,
	FRAME_CMD_REPLY_LED_STATUS = 7,
	FRAME_CMD_REPLY_ALARM_STARTED = 8
} FrameCommandReply_TypeDef;


/* Frame Signal */
#define PROTOCOL_FRAME_SIGNAL_STR(sig)		frameSignalStrings[sig]

typedef enum {
	FRAME_SIGNAL_DOOR_STATUS_CHANGED = 0,
	FRAME_SIGNAL_USR_BTN_PRESSED = 1
} FrameSignal_TypeDef;



/**
 * Host Communication status
 *
 */
typedef enum {
	HOST_COMMUNICATION_NOT_PAIRED = 0,
	HOST_COMMUNICATION_PAIRED = 1
} HostCommunicationStatus_TypeDef;



// Prototypes
void PROTOCOL_InitFrame(Frame_TypeDef*);
char* PROTOCOL_GetData(const Frame_TypeDef*);
void PROTOCOL_DeInitFrame(Frame_TypeDef*);
FrameCommand_TypeDef PROTOCOL_ReadCommand(const char*);
FrameTarget_TypeDef PROTOCOL_ReadTarget(const char*);

uint8_t PROTOCOL_HostIsPaired();
void PROTOCOL_SetHostPaired();
void PROTOCOL_SetHostNotPaired();

#endif /* __STM32F4XX_HELPER_PROTOCOL_H_ */

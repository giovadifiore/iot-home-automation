/*
 * helper_usbcdc.h
 *
 *  Created on: 13/mag/2015
 *      Author: giovannidifiore
 */

#ifndef HELPER_USBCDC_H_
#define HELPER_USBCDC_H_

#include <stdio.h>
#include <stdlib.h>
#include "usbd_cdc.h"
#include "stm32f4xx_helpers.h"

#define READLINE_MAX_LENGTH		256
//#define READLINE_ECHO_CHARS		1

typedef enum {
	USB_DEVICE_NOT_CONNECTED = 0,
	USB_DEVICE_CONNECTED = 1
} USBD_ConnectionStatus_TypeDef;

#define USB_STATUS_LED	LED3

// Management functions
uint8_t 	USB_DeviceIsConnected();
void		USB_DeviceConnectedUserHook(PCD_HandleTypeDef *hpcd);
void 		USB_DeviceDisconnectedUserHook(PCD_HandleTypeDef *hpcd);


// Init
void 		USB_CDC_Init(void);
void		USB_CDC_ConnectedHook(PCD_HandleTypeDef *hpcd);
void 		USB_CDC_DisconnectedHook(PCD_HandleTypeDef *hpcd);

// Serial reading functions
uint16_t 	USB_readLine(void *);
uint16_t 	USB_writeLine(void*);
uint16_t 	USB_writeFloat(float);
void 		USB_writeNewLine(void);

#endif /* HELPER_USBCDC_H_ */

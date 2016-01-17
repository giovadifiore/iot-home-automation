/*
 * helper_usbcdc.c
 *
 *  Created on: 13/mag/2015
 *      Author: giovannidifiore
 */

#include <stm32f4xx_helper_usb.h>

// get the handle
USBD_HandleTypeDef		USBD_Device;
uint8_t usbConnectionStatus = USB_DEVICE_NOT_CONNECTED;

uint16_t USB_writeFloat(float value)
{
	float local = value;
	char str[5];
	sprintf((char *)str, "%2.2f\r\n", local);
	return VCP_write(str, strlen(str));
}

uint16_t USB_readLine(void * ptrBuffer)
{
	int i = 0;
	char receivedChar;
	do {
		// wait to receive char
		while(VCP_read(&receivedChar, 1)==0);

		if (receivedChar!='\r')
		{
			#ifdef READLINE_ECHO_CHARS
			// echo?
			VCP_write(&receivedChar, 1);
			#endif

			// add this char to ptrBuffer
			*((char*)ptrBuffer) = receivedChar;
			ptrBuffer++;
		}
		else
		{
			// the \r was sent, so append the terminator char \0
			#ifdef READLINE_ECHO_CHARS
			// echo?
			VCP_write("\r\n", 2);
			#endif

			// end string
			*((char*)ptrBuffer) = '\0';
		}

		i++;
	} while(receivedChar != '\r' && i < READLINE_MAX_LENGTH);

	return i-1;
}

uint16_t USB_writeLine(void* ptrBuffer)
{
	int wchars = VCP_write(ptrBuffer, strlen((char*)ptrBuffer));
	USB_writeNewLine();
	return wchars;
}

void USB_writeNewLine()
{
	VCP_write("\r\n", 2);
}

void USB_CDC_Init()
{
	USBD_Init(&USBD_Device, &VCP_Desc, 0);

	USBD_RegisterClass(&USBD_Device, &USBD_CDC);
	USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_Template_fops);

	USBD_Start(&USBD_Device);
}

uint8_t USB_DeviceIsConnected()
{
	return usbConnectionStatus==USB_DEVICE_CONNECTED;
}

void USB_CDC_ConnectedHook(PCD_HandleTypeDef *hpcd)
{
	usbConnectionStatus = USB_DEVICE_CONNECTED;

	USB_DeviceConnectedUserHook(hpcd);
}

void USB_CDC_DisconnectedHook(PCD_HandleTypeDef *hpcd)
{
	usbConnectionStatus = USB_DEVICE_NOT_CONNECTED;

	USB_DeviceDisconnectedUserHook(hpcd);
}

__weak void	USB_DeviceConnectedUserHook(PCD_HandleTypeDef *hpcd)
{

}

__weak void USB_DeviceDisconnectedUserHook(PCD_HandleTypeDef *hpcd)
{

}

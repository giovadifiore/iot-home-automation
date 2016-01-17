/*
 * stm32f4xx_helpers.h
 *
 *  Created on: 22/mag/2015
 *      Author: giovannidifiore
 */

#ifndef __STM32F4XX_HELPERS_H_
#define __STM32F4XX_HELPERS_H_

#define HELPER_DISCOVERY
#define HELPER_ERRORS
#define HELPER_USB
#define HELPER_TEMPSENS
#define HELPER_TIMERS
#define HELPER_PROTOCOL
#define HELPER_PROTOCOL_JSON
#define HELPER_LIS3DSH
#define HELPER_DOOR
#define HELPER_LIGHTS
#define HELPER_BUZZER
#define HELPER_BUZZER_RINGTONES

#ifdef HELPER_DISCOVERY
	#include "stm32f4_discovery.h"
#endif

#ifdef HELPER_ERRORS
	#include "stm32f4xx_helper_errors.h"
#endif

#ifdef HELPER_USB
	#include "stm32f4xx_helper_usb.h"
#endif

#ifdef HELPER_TEMPSENS
	#include "stm32f4xx_helper_tempsens.h"
#endif

#ifdef HELPER_LIS3DSH
	#include "stm32f4xx_helper_lis3dsh.h"
#endif

#ifdef HELPER_DOOR
	#include "stm32f4xx_helper_door.h"
#endif

#ifdef HELPER_BUZZER
	#include "stm32f4xx_helper_buzzer.h"
#endif

#ifdef HELPER_BUZZER_RINGTONES
	#include "ringtones/stm32f4xx_helper_ringtones.h"
#endif

#ifdef HELPER_LIGHTS
	#include "stm32f4xx_helper_lights.h"
#endif

#ifdef HELPER_TIMERS
	#include "stm32f4xx_helper_timers.h"
#endif

#ifdef HELPER_PROTOCOL
	#include "stm32f4xx_helper_protocol.h"
#endif

#ifdef HELPER_PROTOCOL_JSON
	#include "stm32f4xx_helper_protocol_json.h"
#endif

#endif /* __STM32F4XX_HELPERS_H_ */

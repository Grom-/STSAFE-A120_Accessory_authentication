 /******************************************************************************
 * \file	stselib_conf.h
 * \brief   STSecureElement library configuration file
 * \author  STMicroelectronics - CS application team
 *
 ******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STSAFE_CONF_H
#define STSAFE_CONF_H

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************
 *                PLATFORM SETTINGS
 ************************************************************/
#include "stm32u5xx.h"
#include <string.h>
#include <stdio.h>

#define PLAT_UI8 uint8_t
#define PLAT_UI16 uint16_t
#define PLAT_UI32 uint32_t
#define PLAT_UI64 uint64_t
#define PLAT_I8 int8_t
#define PLAT_I16 int16_t
#define PLAT_I32 int32_t
#define PLAT_PACKED_STRUCT __PACKED

/************************************************************
 *                TARGET STSAFE SETTINGS
 ************************************************************
 * This section should be modified according to the
 * personalization options enabled in target STSAFE devices
 ************************************************************/
#define STSAFE_USE_PAIRING_KEY
#define STSAFE_USE_STSAFE_COMPANION
#define STSE_USE_RSP_POLLING
#define STSE_MAX_POLLING_RETRY 			100
#define STSE_FIRST_POLLING_INTERVAL		10
#define STSE_POLLING_RETRY_INTERVAL		10
//#define STSAFE_FRAME_DEBUG_LOG


/************************************************************
 *                HOST / CA KEYS SETTINGS
 ************************************************************
 * This section should be modified according to the
 * personalization options enabled in target STSAFE devices
 ************************************************************/
#define STSAFE_USE_ENGINEERING_HOST_KEY_PAIR
#define STSAFE_USE_RANDOM_HOST_KEY_PAIR
#define STSAFE_USE_ENGINEERING_CA_PUB_KEY


/************************************************************
 *                HOST/STSAFE I2C SETTINGS
 ************************************************************/
#define STSAFE_I2C_STATIC
//#define STSAFE_I2C_DYNAMIC


#if defined(STSAFE_I2C_STATIC) && defined(STSAFE_I2C_DYNAMIC)
#error Define only one HOST/STSAFE I2C setting !!!
#endif


#ifdef __cplusplus
}
#endif

#endif /* STSAFE_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

 /******************************************************************************
 * \file	stselib_services_platform.c
 * \brief   STSecureElement Services platform (source)
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

#include <stdlib.h>
#include "Drivers/crc16/crc16.h"
#include "Drivers/rng/rng.h"
#include "Drivers/delay_ms/delay_ms.h"
#include "Drivers/delay_us/delay_us.h"
#include "core/stse_platform.h"

/**
 * \brief STSAFE Middleware initialization platform abstraction function
 * This function is called by the Middleware on stse_init API execution
 * to initialize low-level Plaform specific drivers.
 */
stse_ReturnCode_t stse_services_platform_init (void)
{
	/* Initialize platform Drivers used by PAL */
	delay_ms_init();
	delay_us_init();
	crc16_Init();
	rng_start();

	/* -Initialize power line control (PC0  - open-drain) */
	GPIOC->MODER 	&=  ~(GPIO_MODER_MODE0_Msk);
	GPIOC->MODER 	|=  (1 << GPIO_MODER_MODE0_Pos);
	GPIOC->ODR 	&=  ~(1 << GPIO_ODR_OD0_Pos);

	/* -Initialize power line control (PC1  - open-drain) */
	GPIOC->MODER 	&=  ~(GPIO_MODER_MODE1_Msk);
	GPIOC->MODER 	|=  (1 << GPIO_MODER_MODE1_Pos);
	GPIOC->ODR 	&=  ~(1 << GPIO_ODR_OD1_Pos);

	/* -Initialize power line control (PB0  - open-drain) */
	GPIOB->MODER 	&=  ~(GPIO_MODER_MODE0_Msk);
	GPIOB->MODER 	|=  (1 << GPIO_MODER_MODE0_Pos);
	GPIOB->ODR      &=  ~(1 << GPIO_ODR_OD0_Pos);

	return STSE_OK;
}

/**
 *
 */
PLAT_UI16 stse_platform_Crc16_Calculate (PLAT_UI8 *pbuffer, PLAT_UI16 length)
{
	return crc16_Calculate(pbuffer, length);
}


PLAT_UI16 stse_platform_Crc16_Accumulate (PLAT_UI8 *pbuffer, PLAT_UI16 length)
{
	return crc16_Accumulate(pbuffer, length);
}



/**
 *
 */
void stse_platform_Delay_ms (PLAT_UI32 delay_val)
{
	delay_ms(delay_val);
}

/**
 *
 */
void stse_platform_timeout_ms_start(PLAT_UI16 timeout_val)
{
	timeout_ms_start(timeout_val);
}

/**
 *
 */
PLAT_UI8 stse_platform_timeout_ms_get_status(void)
{
	return timeout_ms_get_status();
}

stse_ReturnCode_t stse_platform_power_on (PLAT_UI8 bus , PLAT_UI8 devAddr)
{
	(void)bus;
	(void)devAddr;

	/* - Power on all the STSAFE SLOTS */
	GPIOB->ODR &= ~(1 << GPIO_ODR_OD0_Pos);
	GPIOC->ODR &= ~(1 << GPIO_ODR_OD0_Pos);
	GPIOC->ODR &= ~(1 << GPIO_ODR_OD1_Pos);

	return ( STSE_OK );

}

stse_ReturnCode_t stse_platform_power_off (PLAT_UI8 bus , PLAT_UI8 devAddr)
{
	(void)bus;
	(void)devAddr;

	/* - Power-off all the STSAFE SLOTS */
	GPIOB->ODR |= (1 << GPIO_ODR_OD0_Pos);
	GPIOC->ODR |= (1 << GPIO_ODR_OD0_Pos);
	GPIOC->ODR |= (1 << GPIO_ODR_OD1_Pos);

	return(STSE_OK);

}

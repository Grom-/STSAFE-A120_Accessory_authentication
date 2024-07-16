 /******************************************************************************
 * \file	stse_platform_i2c_dynamic.c
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


#include "core/stse_platform.h"

#ifdef STSAFE_I2C_DYNAMIC

#include <stdlib.h>
#include "drivers/i2c/I2C.h"

static PLAT_UI8* pI2c_buffer;
static PLAT_UI16 i2c_frame_size;
static volatile PLAT_UI16 i2c_frame_offset;

stse_ReturnCode_t stse_platform_i2c_init (PLAT_UI8 busID)
{
	if (busID == 0)
	{
		return (stse_ReturnCode_t)i2c_init(I2C2);
	}

	return (stse_ReturnCode_t)i2c_init(I2C1);
}

stse_ReturnCode_t stse_platform_i2c_wake (PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed)
{
	(void) speed;
	if (busID == 0)
	{
		i2c_wake(I2C2, devAddr);
	}
	else
	{
		i2c_wake(I2C1, devAddr);
	}

	return (STSE_OK);
}

stse_ReturnCode_t stse_platform_i2c_send_start (
		PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed,
		PLAT_UI16 FrameLength)
{
	(void)busID;
	(void)devAddr;
	(void)speed;

	/* - Allocate Communication buffer */
	pI2c_buffer = malloc(FrameLength);

	/* - Check buffer overflow */
	if (pI2c_buffer == NULL)
	{
		return STSE_PLATFORM_BUFFER_ERR;
	}

	i2c_frame_size = FrameLength;
	i2c_frame_offset = 0;

	return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_continue (
		PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed,
		PLAT_UI8* pData,
		PLAT_UI16 data_size)
{
	(void)busID;
	(void)devAddr;
	(void)speed;

	if(data_size != 0)
	{
		if(pData == NULL)
		{
			memset((pI2c_buffer + i2c_frame_offset),0x00,data_size);
		}
		else
		{
			memcpy((pI2c_buffer + i2c_frame_offset),pData,data_size);
		}
		i2c_frame_offset += data_size;
	}

	return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_stop (
		PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed,
		PLAT_UI8* pData,
		PLAT_UI16 data_size)
{
	stse_ReturnCode_t ret;

	I2C_TypeDef * pI2C;
	if (busID == 0)
	{
		pI2C = I2C2;
	}
	else
	{
		pI2C = I2C1;
	}
	ret =  stse_platform_i2c_send_continue (
			busID,
			devAddr,
			speed,
			pData,
			data_size);

	/* - Send I2C frame buffer */
	if (ret == STSE_OK)
	{
		ret = (stse_ReturnCode_t)i2c_write(pI2C, devAddr,speed,pI2c_buffer,i2c_frame_size);
	}

	/* - Free memory allocated to i2c buffer*/
	free(pI2c_buffer);

	if (ret != STSE_OK)
	{
		ret = STSE_PLATFORM_BUS_ACK_ERROR;
	}

	return ret;
}

stse_ReturnCode_t stse_platform_i2c_receive_start (
		PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed,
		PLAT_UI16 *pFrame_Length)
{
	PLAT_UI8 stat_len[3];
	PLAT_I8 ret = 0;

	I2C_TypeDef * pI2C;
	if (busID == 0)
	{
		pI2C = I2C2;
	}
	else
	{
		pI2C = I2C1;
	}


	/* - Get STSAFE Status + Response Length */
	ret = i2c_read(pI2C, devAddr,speed,stat_len,sizeof(stat_len));
	if(ret != 0)
	{
		return STSE_PLATFORM_BUS_ACK_ERROR;
	}

	/* - Store response Length */
	i2c_frame_size = ((stat_len[1] << 8) + stat_len[2]) + STSE_RSP_FRAME_HEADER_SIZE + STSE_FRAME_CRC_SIZE;
	*pFrame_Length = i2c_frame_size;

	/* - Allocate Communication buffer */
	pI2c_buffer = malloc(*pFrame_Length);

	/* - Check buffer overflow */
	if (pI2c_buffer == NULL)
	{
		return STSE_PLATFORM_BUFFER_ERR;
	}

	/* - Read full Frame */
	ret = i2c_read(pI2C, devAddr,speed,pI2c_buffer,*pFrame_Length);
	if(ret != 0)
	{
		return STSE_PLATFORM_BUS_ERR;
	}

	/* - reset read offset */
	i2c_frame_offset = 0;

	return (stse_ReturnCode_t)(stat_len[0] & 0x3F);
}

stse_ReturnCode_t stse_platform_i2c_receive_continue (
		PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed,
		PLAT_UI8* pData,
		PLAT_UI16 data_size)
{
	(void)busID;
	(void)devAddr;
	(void)speed;

	if (pData != NULL)
	{
		/* Ignore I2C length */
		if(i2c_frame_offset == 1 )
		{
			i2c_frame_offset += 2;
		}
		/* Copy buffer content */
		memcpy(pData,(pI2c_buffer + i2c_frame_offset),data_size);
	}
	i2c_frame_offset += data_size;
	return (STSE_OK);
}

stse_ReturnCode_t stse_platform_i2c_receive_stop (
		PLAT_UI8 busID,
		PLAT_UI8 devAddr,
		PLAT_UI16 speed,
		PLAT_UI8* pData,
		PLAT_UI16 data_size)
{
	(void) busID;
	(void) devAddr;
	(void) speed;

	/*- Copy last element*/
	if (pData != NULL)
	{
		memcpy(pData,(pI2c_buffer + i2c_frame_offset),data_size);
	}

	/*- Free i2c buffer*/
	free(pI2c_buffer);
	return (STSE_OK);
}

#endif

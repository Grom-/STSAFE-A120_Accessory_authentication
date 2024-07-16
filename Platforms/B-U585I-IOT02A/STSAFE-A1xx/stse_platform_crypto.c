 /******************************************************************************
 * \file	stselib_crypto_platform.h
 * \brief   STSecureElement cryptographic platform file
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

#include "Drivers/rng/rng.h"
#include "Drivers/crc16/crc16.h"
#include "Drivers/delay_ms/delay_ms.h"
#include "Drivers/i2c/I2C.h"
#include "stselib.h"

/* Include Platform Crypto */
#include "Middleware/STM32_Cryptographic/include/cmox_crypto.h"

/************************************************************
 *                STSAFE CRYPTO Global variables
 **************************************************************/

#define KEK_WRAP_IV_SIZE 8
const PLAT_UI8 KEK_WRAP_IV[KEK_WRAP_IV_SIZE] = {0xA6,0xA6,0xA6,0xA6,0xA6,0xA6,0xA6,0xA6};

cmox_ecc_handle_t Ecc_Ctx;
PLAT_UI8 cmox_math_buffer[2400];

cmox_mac_handle_t* pMAC_Handler;
cmox_cmac_handle_t CMAC_Handler;

/* Private_key */
PLAT_UI8 static_c25519_priv_key[32] = {0x3D,0xAC,0x2A,0xFF,0x7A,0x55,0x9F,0xAA,
									   0xAC,0x1B,0xB6,0x46,0xE9,0xD5,0xE0,0x50,
									   0x28,0x72,0xFE,0x9F,0xD5,0xE8,0x3B,0x7E,
									   0x68,0x28,0x7A,0xB3,0xF4,0x7E,0x59,0x8F};

/* Public_key */
PLAT_UI8 static_c25519_pub_key[32] = {0x59,0x86,0xA8,0xA3,0x51,0xBB,0x07,0xCA,
									  0x40,0x01,0x76,0xF7,0x66,0x8A,0x4F,0xBF,
									  0xA1,0xA5,0xE1,0x9A,0xCB,0x57,0x55,0xF6,
									  0x57,0xF9,0x43,0xE9,0xBC,0x39,0x54,0x0B};

/************************************************************
 *                STATIC FUNCTIONS
 **************************************************************/

//static cmox_hash_algo_t stse_platform_get_cmox_hash_algo(stse_hash_algorithm_t hash_algo)
//{
//	switch(hash_algo)
//	{
//		case STSAFE_SHA_256:
//			return CMOX_SHA256_ALGO;
//		case STSAFE_SHA_384:
//			return CMOX_SHA384_ALGO;
//		case STSAFE_SHA_512:
//			return CMOX_SHA512_ALGO;
//		case STSAFE_SHA3_256:
//			return CMOX_SHA3_256_ALGO;
//		case STSAFE_SHA3_384:
//			return CMOX_SHA3_384_ALGO;
//		case STSAFE_SHA3_512:
//			return CMOX_SHA3_512_ALGO;
//		default:
//			return STSE_OK;
//	}
//}

static cmox_ecc_impl_t stse_platform_get_cmox_ecc_impl(stse_ecc_key_type_t key_type)
{
	switch(key_type)
	{
		case STSE_ECC_KT_NIST_P_256:
			return CMOX_ECC_SECP256R1_LOWMEM;
		case STSE_ECC_KT_NIST_P_384:
			return CMOX_ECC_SECP384R1_LOWMEM;
		case STSE_ECC_KT_NIST_P_521:
			return CMOX_ECC_SECP521R1_LOWMEM;
		case STSE_ECC_KT_BP_P_256:
			return CMOX_ECC_BPP256R1_LOWMEM;
		case STSE_ECC_KT_BP_P_384:
			return CMOX_ECC_BPP384R1_LOWMEM;
		case STSE_ECC_KT_BP_P_512:
			return CMOX_ECC_BPP512R1_LOWMEM;
		case STSE_ECC_KT_CURVE25519:
			return CMOX_ECC_CURVE25519;
		case STSE_ECC_KT_ED25519:
			return CMOX_ECC_ED25519_OPT_LOWMEM;
		default:
			return NULL;
	}
}

static size_t stse_platform_get_cmox_ecc_priv_key_len(stse_ecc_key_type_t key_type)
{
	switch(key_type)
	{
		case STSE_ECC_KT_NIST_P_256:
			return CMOX_ECC_SECP256R1_PRIVKEY_LEN;
		case STSE_ECC_KT_NIST_P_384:
			return CMOX_ECC_SECP384R1_PRIVKEY_LEN;
		case STSE_ECC_KT_NIST_P_521:
			return CMOX_ECC_SECP521R1_PRIVKEY_LEN;
		case STSE_ECC_KT_BP_P_256:
			return CMOX_ECC_BPP256R1_PRIVKEY_LEN;
		case STSE_ECC_KT_BP_P_384:
			return CMOX_ECC_BPP384R1_PRIVKEY_LEN;
		case STSE_ECC_KT_BP_P_512:
			return CMOX_ECC_BPP512R1_PRIVKEY_LEN;
		case STSE_ECC_KT_CURVE25519:
			return CMOX_ECC_CURVE25519_PRIVKEY_LEN;
		case STSE_ECC_KT_ED25519:
			return CMOX_ECC_ED25519_PRIVKEY_LEN;
		default:
			return STSE_OK;
	}
}

static size_t stse_platform_get_cmox_ecc_pub_key_len(stse_ecc_key_type_t key_type)
{
	switch(key_type)
	{
		case STSE_ECC_KT_NIST_P_256:
			return CMOX_ECC_SECP256R1_PUBKEY_LEN;
		case STSE_ECC_KT_NIST_P_384:
			return CMOX_ECC_SECP384R1_PUBKEY_LEN;
		case STSE_ECC_KT_NIST_P_521:
			return CMOX_ECC_SECP521R1_PUBKEY_LEN;
		case STSE_ECC_KT_BP_P_256:
			return CMOX_ECC_BPP256R1_PUBKEY_LEN;
		case STSE_ECC_KT_BP_P_384:
			return CMOX_ECC_BPP384R1_PUBKEY_LEN;
		case STSE_ECC_KT_BP_P_512:
			return CMOX_ECC_BPP512R1_PUBKEY_LEN;
		case STSE_ECC_KT_CURVE25519:
			return CMOX_ECC_CURVE25519_PUBKEY_LEN;
		case STSE_ECC_KT_ED25519:
			return CMOX_ECC_ED25519_PUBKEY_LEN;
		default:
			return STSE_OK;
	}
}

static size_t stse_platform_get_cmox_ecc_sig_len(stse_ecc_key_type_t key_type)
{
	switch(key_type)
	{
		case STSE_ECC_KT_NIST_P_256:
			return CMOX_ECC_SECP256R1_SIG_LEN;
		case STSE_ECC_KT_NIST_P_384:
			return CMOX_ECC_SECP384R1_SIG_LEN;
		case STSE_ECC_KT_NIST_P_521:
			return CMOX_ECC_SECP521R1_SIG_LEN;
		case STSE_ECC_KT_BP_P_256:
			return CMOX_ECC_BPP256R1_SIG_LEN;
		case STSE_ECC_KT_BP_P_384:
			return CMOX_ECC_BPP384R1_SIG_LEN;
		case STSE_ECC_KT_BP_P_512:
			return CMOX_ECC_BPP512R1_SIG_LEN;
		case STSE_ECC_KT_CURVE25519:
			return STSE_OK; /* No signature with curve25519 */
		case STSE_ECC_KT_ED25519:
			return CMOX_ECC_ED25519_SIG_LEN;
		default:
			return STSE_OK;
	}
}

static cmox_hash_algo_t stse_platform_get_cmox_hash_algo(stse_hash_algorithm_t hash_algo)
{
	switch(hash_algo)
	{
		case STSE_SHA_1:
			return CMOX_SHA1_ALGO;
		case STSE_SHA_224:
			return CMOX_SHA224_ALGO;
		case STSE_SHA_256:
			return CMOX_SHA256_ALGO;
		case STSE_SHA_384:
			return CMOX_SHA384_ALGO;
		case STSE_SHA_512:
			return CMOX_SHA512_ALGO;
		case STSE_SHA3_256:
			return CMOX_SHA3_256_ALGO;
		case STSE_SHA3_384:
			return CMOX_SHA3_384_ALGO;
		case STSE_SHA3_512:
			return CMOX_SHA3_512_ALGO;
		default:
			return (cmox_hash_algo_t)STSE_SHA_INVALID;
	}
}

/************************************************************
 *                STSAFE CRYPTO HAL
 **************************************************************/


stse_ReturnCode_t stse_crypto_platform_init (void)
{
	stse_ReturnCode_t ret = STSE_OK;

	/* - Initialize cryptographic library */
	if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
	{
		ret = STSE_PLATFORM_CRYPTO_INIT_ERROR;
	}

	/* - Initialize Random Number Generator */
	rng_start();

	return ret;

}

PLAT_UI32 stse_platform_Random (void)
{
	return rng_generate_random_number();
}

stse_ReturnCode_t stse_platform_hash_compute(stse_hash_algorithm_t hash_algo,
									  PLAT_UI8 *pPayload, PLAT_UI32 payload_length,
									  PLAT_UI8 *pHash, PLAT_UI32 *hash_length)
{
	cmox_ecc_retval_t retval;

	retval = cmox_hash_compute(
			stse_platform_get_cmox_hash_algo(hash_algo),
			pPayload,
			payload_length,
			pHash,
			*hash_length,
			(size_t*)hash_length);

	/*- Verify Hash compute return */
	if (retval != CMOX_HASH_SUCCESS)
	{
		return STSE_PLATFORM_HASH_ERROR;
	} else {
		return STSE_OK;
	}
}

cmox_ecc_retval_t stsafe_cmox_curve25519_generate_key_pair(
		cmox_ecc_handle_t     *P_pEccCtx,
        const cmox_ecc_impl_t P_CurveParams,
        const uint8_t         *P_pRandom,
        size_t                P_RandomLen,
        uint8_t               *P_pPrivKey,
        size_t                *P_pPrivKeyLen,
        uint8_t               *P_pPubKey,
        size_t                *P_pPubKeyLen)
{
	(void)P_pPrivKeyLen;
	cmox_ecc_retval_t retval;

	/* base point */
	const PLAT_UI8 base[32] = {9u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
							   0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};

	/* check for NULL parameters */
	if(P_pPrivKey	 == NULL
	|| P_pPubKey 	 == NULL
	|| P_pRandom 	 == NULL
	|| P_CurveParams != CMOX_ECC_CURVE25519
	|| P_RandomLen 	 <  32u)
	{
		return CMOX_ECC_ERR_BAD_PARAMETERS;
	}

	memcpy(P_pPrivKey, P_pRandom, 32u);

	/* Clamp private key */
	P_pPrivKey[0]  &= 248u;
	P_pPrivKey[31] &= 127u;
	P_pPrivKey[31] |= 64u;

	/* Perform scalar multiplication to obtain the public key */
	retval = cmox_ecdh(
			P_pEccCtx, P_CurveParams,
			P_pPrivKey, 32u,
			base, 32u,
			P_pPubKey, P_pPubKeyLen);

	return retval;
}

stse_ReturnCode_t stse_platform_ecc_generate_key_pair(
		stse_ecc_key_type_t key_type,
		PLAT_UI8 *pPrivKey,
		PLAT_UI8 *pPubKey)
{
	cmox_ecc_retval_t retval;

	/*- Set ECC context */
	cmox_ecc_construct(&Ecc_Ctx,			/* ECC context */
			CMOX_MATH_FUNCS_SMALL,			/* Small math functions */
			cmox_math_buffer,				/* Crypto math buffer */
			sizeof(cmox_math_buffer)		/* buffer size */
	);

	/* Minimum random length equal the private key length */
	size_t randomLength = stse_platform_get_cmox_ecc_priv_key_len(key_type);
	/* Align the random length to modulo 4 */
	randomLength += 4 - (randomLength&0x3);
	/* Add 32bytes to random length if the key is Curve25519 because it will use the Ed25519 key gen */
	randomLength += ((key_type == STSE_ECC_KT_CURVE25519) ? 32 : 0);

	/* Retry loop in case the RNG isn't strong enough */
	do
	{
		/* - Generate a random number */
		PLAT_UI8 randomNumber[randomLength];
		for(uint8_t i=0; i<randomLength; i+=4)
		{
			*((PLAT_UI32*) &randomNumber[i]) = stse_platform_Random();
		}

		/*- Generate EdDSA key pair */
		if(key_type == STSE_ECC_KT_ED25519)
		{
			retval = cmox_eddsa_keyGen(&Ecc_Ctx,	/* ECC context */
					stse_platform_get_cmox_ecc_impl(key_type),	/* Curve param */
					randomNumber, 					/* Random number */
					randomLength,					/* Random number length */
					pPrivKey, 						/* Private key */
					NULL,							/* Private key length*/
					pPubKey, 						/* Public key */
					NULL);							/* Public key length */
		}
		else if(key_type == STSE_ECC_KT_CURVE25519)
		{
			memcpy(pPrivKey, static_c25519_priv_key, 32);
			memcpy(pPubKey, static_c25519_pub_key, 32);

			retval = CMOX_ECC_SUCCESS;
		}
		else
		{
			retval = cmox_ecdsa_keyGen(&Ecc_Ctx,	/* ECC context */
					stse_platform_get_cmox_ecc_impl(key_type),	/* Curve param */
					randomNumber, 					/* Random number */
					randomLength,					/* Random number length */
					pPrivKey,						/* Private key */
					NULL,							/* Private key length*/
					pPubKey, 						/* Public key */
					NULL);							/* Public key length */
		}
	} while(retval == CMOX_ECC_ERR_WRONG_RANDOM);

	/* - Clear ECC context */
	cmox_ecc_cleanup(&Ecc_Ctx);

	if (retval != CMOX_ECC_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_ecc_verify(
		stse_ecc_key_type_t key_type,
		const PLAT_UI8 *pPubKey,
		PLAT_UI8 *pDigest,
		PLAT_UI16 digestLen,
		PLAT_UI8 *pSignature)
{
	cmox_ecc_retval_t retval;
	PLAT_UI32 faultCheck;

	/*- Set ECC context */
	cmox_ecc_construct(&Ecc_Ctx,			/* ECC context */
			CMOX_MATH_FUNCS_SMALL,			/* Small math functions */
			cmox_math_buffer,				/* Crypto math buffer */
			sizeof(cmox_math_buffer)		/* buffer size */
	);

	if(key_type == STSE_ECC_KT_ED25519)
	{
		/* - Perform EDDSA verify */
		retval = cmox_eddsa_verify( &Ecc_Ctx,   					/* ECC context */
				stse_platform_get_cmox_ecc_impl(key_type),  		/* Curve param */
				pPubKey, 											/* Public key */
				stse_platform_get_cmox_ecc_pub_key_len(key_type), 	/* Public key length */
				pDigest, 											/* Message */
				digestLen,              							/* Message length */
				pSignature, 										/* Pointer to signature */
				stse_platform_get_cmox_ecc_sig_len(key_type),		/* Signature size */
				&faultCheck											/* Fault check variable */
		);
	}
	else
	{
		/* - Perform ECDSA verify */
		retval = cmox_ecdsa_verify( &Ecc_Ctx,   					/* ECC context */
				stse_platform_get_cmox_ecc_impl(key_type),    		/* Curve : SECP256R1 */
				pPubKey, 											/* Public key */
				stse_platform_get_cmox_ecc_pub_key_len(key_type), 	/* Public key length */
				pDigest, 											/* Message */
				digestLen,              							/* Message length */
				pSignature, 										/* Pointer to signature */
				stse_platform_get_cmox_ecc_sig_len(key_type),		/* Signature size */
				&faultCheck											/* Fault check variable */
		);
	}

	/* - Clear ECC context */
	cmox_ecc_cleanup(&Ecc_Ctx);

	if (retval != CMOX_ECC_AUTH_SUCCESS)
	{
		return STSE_PLATFORM_ECC_VERIFY_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_ecc_sign(
		stse_ecc_key_type_t key_type,
		PLAT_UI8 *pPrivKey,
		PLAT_UI8 *pDigest,
		PLAT_UI16 digestLen,
		PLAT_UI8 *pSignature)
{
	cmox_ecc_retval_t retval;

	/*- Set ECC context */
	cmox_ecc_construct(&Ecc_Ctx,			/* ECC context */
			CMOX_MATH_FUNCS_SMALL,			/* Small math functions */
			cmox_math_buffer,				/* Crypto math buffer */
			sizeof(cmox_math_buffer)		/* buffer size */
	);

	if(key_type == STSE_ECC_KT_ED25519)
	{
		/* - Perform EDDSA sign */
		retval = cmox_eddsa_sign(&Ecc_Ctx,			/* ECC context */
				stse_platform_get_cmox_ecc_impl(key_type),    	/* Curve param */
				pPrivKey,							/* Private key */
				stse_platform_get_cmox_ecc_priv_key_len(key_type),/* Private key length*/
				pDigest,							/* Message */
				digestLen,							/* Message length */
				pSignature,							/* Signature */
				NULL								/* Signature length */
		);
	}
	else
	{
		do
		{
			/* - Generate a random number */
			size_t randomLength = stse_platform_get_cmox_ecc_priv_key_len(key_type) + (4-(stse_platform_get_cmox_ecc_priv_key_len(key_type)&0x3));
			PLAT_UI8 randomNumber[randomLength];
			for(uint8_t i=0; i<randomLength; i+=4)
			{
				*((PLAT_UI32*) &randomNumber[i]) = stse_platform_Random();
			}

			/* - Perform ECDSA sign */
			retval = cmox_ecdsa_sign(&Ecc_Ctx,			/* ECC context */
					stse_platform_get_cmox_ecc_impl(key_type),    	/* Curve param */
					randomNumber,
					stse_platform_get_cmox_ecc_priv_key_len(key_type),
					pPrivKey,							/* Private key */
					stse_platform_get_cmox_ecc_priv_key_len(key_type),/* Private key length*/
					pDigest,							/* Message */
					digestLen,							/* Message length */
					pSignature,							/* Signature */
					NULL								/* Signature length */
			);
		} while(retval == CMOX_ECC_ERR_WRONG_RANDOM);
	}

	/* - Clear ECC context */
	cmox_ecc_cleanup(&Ecc_Ctx);

	if (retval != CMOX_ECC_SUCCESS)
	{
		return STSE_PLATFORM_ECC_SIGN_ERROR;
	} else {
		return STSE_OK;
	}
}


stse_ReturnCode_t stse_platform_ecc_ecdh(
		stse_ecc_key_type_t key_type,
		const PLAT_UI8 *pPubKey,
		const PLAT_UI8 *pPrivKey,
		PLAT_UI8       *pSharedSecret)
{
	cmox_ecc_retval_t retval;

	/*- Set ECC context */
	cmox_ecc_construct(&Ecc_Ctx,			/* ECC context */
			CMOX_MATH_FUNCS_SMALL,			/* Small math functions */
			cmox_math_buffer,				/* Crypto math buffer */
			sizeof(cmox_math_buffer)		/* buffer size */
	);

	retval = cmox_ecdh(&Ecc_Ctx,            		/* ECC context */
			stse_platform_get_cmox_ecc_impl(key_type),      		/* Curve param */
			pPrivKey,								/* Private key (local) */
			stse_platform_get_cmox_ecc_priv_key_len(key_type),	/* Private key length*/
			pPubKey,								/* Public key (remote) */
			stse_platform_get_cmox_ecc_pub_key_len(key_type), 	/* Public key length */
			pSharedSecret,							/* Shared secret */
			NULL									/* Shared secret length */
	);

	/* - Clear ECC context */
	cmox_ecc_cleanup(&Ecc_Ctx);

	if (retval != CMOX_ECC_SUCCESS)
	{
		return STSE_PLATFORM_ECC_ECDH_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_cmac_init (const PLAT_UI8 	*pKey,
		PLAT_UI16 		key_length,
		PLAT_UI16 		exp_tag_size)
{
	cmox_mac_retval_t retval;

	/* - Call CMAC constructor */
	pMAC_Handler = cmox_cmac_construct(&CMAC_Handler,CMOX_CMAC_AESSMALL);

	/* - Init MAC */
	retval = cmox_mac_init(pMAC_Handler);
	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
	}
	/* - Set Tag length */
	retval = cmox_mac_setTagLen(pMAC_Handler, exp_tag_size);
	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
	}
	/* - Set Key  */
	retval = cmox_mac_setKey(pMAC_Handler, pKey, key_length);
	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_cmac_append(PLAT_UI8* pInput,
		PLAT_UI16 lenght)
{
	cmox_mac_retval_t retval;

	retval = cmox_mac_append(pMAC_Handler,pInput,lenght);

	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_cmac_compute_finish(PLAT_UI8* pTag, PLAT_UI8* pTagLen)
{
	cmox_mac_retval_t retval;

	retval = cmox_mac_generateTag(pMAC_Handler, pTag, (size_t *)pTagLen);
	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
	}

	retval = cmox_mac_cleanup(pMAC_Handler);
	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
	}

	return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cmac_verify_finish(PLAT_UI8* pTag)
{
	cmox_mac_retval_t retval;
	uint32_t cmox_mac_fault_check = 0;

	retval = cmox_mac_verifyTag(
			pMAC_Handler,
			pTag,
            &cmox_mac_fault_check);


	cmox_mac_cleanup(pMAC_Handler);

	if ((retval != CMOX_MAC_AUTH_SUCCESS)||(cmox_mac_fault_check != CMOX_MAC_AUTH_SUCCESS))
	{
		return STSE_PLATFORM_AES_CMAC_VERIFY_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_cmac_compute(const PLAT_UI8 *pPayload,
		PLAT_UI16 		payload_length,
		const PLAT_UI8 	*pKey,
		PLAT_UI16 		key_length,
		PLAT_UI16 		exp_tag_size,
		PLAT_UI8 		*pTag,
		PLAT_UI16 		*pTag_length)
{
	cmox_mac_retval_t retval;

	retval = cmox_mac_compute(CMOX_CMAC_AESSMALL_ALGO,	/* Use AES CMAC algorithm */
			pPayload,								/* Message */
			payload_length,  						/* Message length*/
			pKey,									/* AES key */
			key_length,          					/* AES key length */
			NULL, 									/* Custom Data */
			0,                   					/* Custom Data length */
			pTag,              		 				/* Tag */
			exp_tag_size,      		 				/* Expected Tag size */
			(size_t*)pTag_length					/* Generated Tag size */
	);



	if (retval != CMOX_MAC_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_VERIFY_ERROR;
	} else {
		return STSE_OK;
	}
}
stse_ReturnCode_t stse_platform_aes_cmac_verify(const PLAT_UI8 *pPayload,
		PLAT_UI16 		payload_length,
		const PLAT_UI8 	*pKey,
		PLAT_UI16 		key_length,
		const PLAT_UI8 	*pTag,
		PLAT_UI16 		tag_length)
{
	cmox_mac_retval_t retval;

	/* - Perform CMAC verification */
	retval = cmox_mac_verify(CMOX_CMAC_AESSMALL_ALGO, /* Use AES CMAC algorithm */
			pPayload,			/* Message length */
			payload_length,		/* Message length */
			pKey,				/* AES key */
			key_length,   		/* AES key length */
			NULL, 				/* Custom data */
			0,					/* Custom data length*/
			pTag,               /* Tag */
			tag_length			/* Tag size */
	);

	if (retval != CMOX_MAC_AUTH_SUCCESS)
	{
		return STSE_PLATFORM_AES_CMAC_VERIFY_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_ccm_enc(const PLAT_UI8 *pPlaintext,
		PLAT_UI16 		plaintext_length,
		const PLAT_UI8 	*pKey,
		PLAT_UI16  		key_length,
		const PLAT_UI8 	*pNonce,
		PLAT_UI16  		nonce_length,
		const PLAT_UI8 	*pAssocData,
		PLAT_UI16  		assocData_length,
		PLAT_UI8       	*pEncryptedtext,
		PLAT_UI16 		*pEncryptedtext_length,
		PLAT_UI8       	*pTag,
		PLAT_UI16  		tag_length)
{
	cmox_cipher_retval_t retval;
	PLAT_UI8 pCiphertext[ *pEncryptedtext_length + tag_length ];
	size_t ciphertext_length;

	retval = cmox_aead_encrypt(CMOX_AESSMALL_CCM_ENC_ALGO,	/* Use AES CCM algorithm */
			pPlaintext, 								/* Plain text */
			plaintext_length,							/* Plain text length */
			tag_length,									/* Tag length*/
			pKey,										/* AES key */
			key_length,									/* AES key length*/
			pNonce,										/* Nonce */
			nonce_length,								/* Nonce length */
			pAssocData,									/* Associated Data */
			assocData_length,							/* Associated Data length */
			pCiphertext,								/* Ciphered text */
			&ciphertext_length							/* Ciphered length */
	);

	if (retval != CMOX_CIPHER_SUCCESS)
	{
		return STSE_PLATFORM_AES_CCM_ENCRYPT_ERROR;
	}

	memcpy(pEncryptedtext, pCiphertext, *pEncryptedtext_length);
	memcpy(pTag, &pCiphertext[*pEncryptedtext_length], tag_length);

	return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_ccm_dec(const PLAT_UI8 *pEncryptedtext, PLAT_UI16 encryptedtext_length,
		const PLAT_UI8 *pTag,
		PLAT_UI16 tag_length,
		const PLAT_UI8 *pKey,
		PLAT_UI16 key_length,
		const PLAT_UI8 *pNonce,
		PLAT_UI16 nonce_length,
		const PLAT_UI8 *pAssocData,
		PLAT_UI16 assocData_length,
		const PLAT_UI8 *pPlaintext,
		PLAT_UI16 plaintext_length)
{
	cmox_cipher_retval_t retval;

	size_t ciphertext_length = (size_t)encryptedtext_length + (size_t)tag_length;
	PLAT_UI8 pCiphertext[ ciphertext_length ];

	memcpy(pCiphertext, pEncryptedtext, (size_t)encryptedtext_length);

	memcpy(&pCiphertext[encryptedtext_length], pTag, tag_length);

	retval = cmox_aead_decrypt(CMOX_AESSMALL_CCM_DEC_ALGO,	/* Use AES CCM algorithm */
			pCiphertext,								/* Ciphered Text */
			ciphertext_length,    						/* Ciphered Text length */
			tag_length,                   				/* Tag length */
			pKey,										/* AES Key*/
			key_length,                        			/* AES Key length*/
			pNonce,										/* Nonce */
			nonce_length,                    			/* Nonce length */
			pAssocData,									/* Associated Data */
			assocData_length,            				/* Associated Data length */
			(PLAT_UI8*)pPlaintext,						/* Plain Text */
			(size_t*)&plaintext_length					/* Plain Text length*/
	);

	if (retval != CMOX_CIPHER_AUTH_SUCCESS)
	{
		return STSE_PLATFORM_AES_CCM_DECRYPT_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_cbc_enc(const PLAT_UI8 *pPlaintext,
		PLAT_UI16  plaintext_length,
		PLAT_UI8 *pInitial_value,
		const PLAT_UI8 *pKey,
		PLAT_UI16  key_length,
		PLAT_UI8  *pEncryptedtext,
		PLAT_UI16 *pEncryptedtext_length)
{
	cmox_cipher_retval_t retval;

	/*- Perform AES ECB Encryption */
	retval = cmox_cipher_encrypt(CMOX_AESSMALL_CBC_ENC_ALGO, /* Use AES CBC algorithm */
			pPlaintext,									/* Plain Text */
			plaintext_length,          					/* Plain Text length*/
			pKey,										/* AES Key */
			key_length,                     	 		/* AES Key length*/
			pInitial_value, 							/* Initial Value */
			16,     									/* Initial Value length */
			pEncryptedtext, 							/* Ciphered Text */
			(size_t*)pEncryptedtext_length				/* Ciphered Text length*/
	);

	/*- Verify AES ECB Encryption status */
	if (retval != CMOX_CIPHER_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_cbc_dec(const PLAT_UI8 *pEncryptedtext,
		PLAT_UI16  encryptedtext_length,
		PLAT_UI8 *pInitial_value,
		const PLAT_UI8 *pKey,
		PLAT_UI16  key_length,
		PLAT_UI8  *pPlaintext,
		PLAT_UI16 *pPlaintext_length)
{
	cmox_cipher_retval_t retval;

	/*- Perform AES ECB decryption */
	retval = cmox_cipher_decrypt(CMOX_AESSMALL_CBC_DEC_ALGO,	/* Use AES CBC algorithm */
			pEncryptedtext, 							/* Ciphered Text */
			encryptedtext_length,  						/* Ciphered Text length */
			pKey, 										/* AES key length */
			key_length,                      			/* AES key */
			pInitial_value, 							/* Initial Value */
			16,        									/* Initial Value length*/
			pPlaintext, 								/* Plain Text */
			(size_t*)pPlaintext_length					/* Plain Text length*/
	);

	/*- Verify AES ECB decrypt return */
	if (retval != CMOX_CIPHER_SUCCESS)
	{
		return STSE_PLATFORM_AES_CBC_DECRYPT_ERROR;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_ecb_enc(const PLAT_UI8 *pPlaintext,
		PLAT_UI16  plaintext_length,
		const PLAT_UI8 *pKey,
		PLAT_UI16  key_length,
		PLAT_UI8  *pEncryptedtext,
		PLAT_UI16 *pEncryptedtext_length)
{
	cmox_cipher_retval_t retval;
	PLAT_UI8 IV[16] = {0};

	/*- Perform AES ECB Encryption */
	retval = cmox_cipher_encrypt(CMOX_AESSMALL_ECB_ENC_ALGO, /* Use AES ECB algorithm */
			pPlaintext,									/* Plain Text */
			plaintext_length,          					/* Plain Text length*/
			pKey,										/* AES Key */
			key_length,                     	 		/* AES Key length*/
			IV, 										/* Initial Value */
			16,     									/* Initial Value length */
			pEncryptedtext, 							/* Ciphered Text */
			(size_t*)pEncryptedtext_length				/* Ciphered Text length*/
	);

	/*- Verify AES ECB Encryption status */
	if (retval != CMOX_CIPHER_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_aes_ecb_dec(const PLAT_UI8 *pEncryptedtext,
		PLAT_UI16  encryptedtext_length,
		const PLAT_UI8 *pKey,
		PLAT_UI16  key_length,
		PLAT_UI8  *pPlaintext,
		PLAT_UI16 *pPlaintext_length)
{
	cmox_cipher_retval_t retval;
	PLAT_UI8 IV[16] = {0};

	/*- Perform AES ECB decryption */
	retval = cmox_cipher_decrypt(CMOX_AESSMALL_ECB_DEC_ALGO,	/* Use AES ECB algorithm */
			pEncryptedtext, 							/* Ciphered Text */
			encryptedtext_length,  						/* Ciphered Text length */
			pKey, 										/* AES key length */
			key_length,                      			/* AES key */
			IV, 										/* Initial Value */
			16,        									/* Initial Value length*/
			pPlaintext, 								/* Plain Text */
			(size_t*)pPlaintext_length					/* Plain Text length*/
	);

	/*- Verify AES ECB decrypt return */
	if (retval != CMOX_CIPHER_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_nist_kw_encrypt(PLAT_UI8 *pPayload, PLAT_UI32 payload_length,
												PLAT_UI8 *pKey,		PLAT_UI8 key_length,
												PLAT_UI8 *pOutput, 	PLAT_UI32 *pOutput_length)
{
	cmox_cipher_retval_t retval;

	retval = cmox_cipher_encrypt(
		CMOX_AESSMALL_KEYWRAP_ENC_ALGO,
		pPayload, payload_length,
		pKey, 	  key_length,
		KEK_WRAP_IV, KEK_WRAP_IV_SIZE,
		pOutput,  (size_t*)pOutput_length);

	if (retval != CMOX_CIPHER_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	}

	return STSE_OK;
}

stse_ReturnCode_t stse_platform_hmac_sha256_compute(PLAT_UI8 *pSalt, PLAT_UI16 salt_length,
											 PLAT_UI8 *pInput_keying_material, PLAT_UI16 input_keying_material_length,
											 PLAT_UI8 *pInfo, PLAT_UI16 info_length,
											 PLAT_UI8 *pOutput_keying_material, PLAT_UI16 output_keying_material_length)
{
	PLAT_UI8 retval;
	PLAT_UI8 pPseudorandom_key[STSAFEA_SHA_256_HASH_SIZE];

	/* Extract pseudo-random key from input keying material */
	retval = stse_platform_hmac_sha256_extract(pSalt,
												 salt_length,
												 pInput_keying_material,
												 input_keying_material_length,
												 pPseudorandom_key,
												 STSAFEA_SHA_256_HASH_SIZE);

	if(retval != 0)
	{
		memset(pPseudorandom_key, 0, STSAFEA_SHA_256_HASH_SIZE);
		return (stse_ReturnCode_t)retval;
	}

	/* Expand output key from pseudo-random key */
	retval = stse_platform_hmac_sha256_expand(pPseudorandom_key,
													STSAFEA_SHA_256_HASH_SIZE,
													pInfo,
													info_length,
													pOutput_keying_material,
													output_keying_material_length);

	/* Pseudo-random key no more needed, cleanup */
	memset(pPseudorandom_key, 0, STSAFEA_SHA_256_HASH_SIZE);

	return (stse_ReturnCode_t)retval;
}

stse_ReturnCode_t stse_platform_hmac_sha256_extract(PLAT_UI8 *pSalt, PLAT_UI16 salt_length,
											 PLAT_UI8 *pInput_keying_material, PLAT_UI16 input_keying_material_length,
											 PLAT_UI8 *pPseudorandom_key, PLAT_UI16 pseudorandom_key_expected_length)
{
	cmox_mac_retval_t retval;

	PLAT_UI16 pseudorandom_key_length = pseudorandom_key_expected_length;

	retval = cmox_mac_compute(CMOX_HMAC_SHA256_ALGO,
						   pInput_keying_material,
						   input_keying_material_length,
						   pSalt,
						   salt_length,
						   NULL,
						   0,
						   pPseudorandom_key,
						   pseudorandom_key_expected_length,
						   (size_t*)&pseudorandom_key_length);

	/*- Verify MAC compute return */
	if (retval != CMOX_MAC_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	} else {
		return STSE_OK;
	}
}

stse_ReturnCode_t stse_platform_hmac_sha256_expand(PLAT_UI8  *pPseudorandom_key, PLAT_UI16 pseudorandom_key_length,
											PLAT_UI8  *pInfo, PLAT_UI16 info_length,
											PLAT_UI8  *pOutput_keying_material, PLAT_UI16 output_keying_material_length)
{
	cmox_mac_retval_t retval = CMOX_MAC_SUCCESS;

	PLAT_UI8   	tmp[CMOX_SHA256_SIZE];
	PLAT_UI16   tmp_length = 0;
	PLAT_UI16	out_index = 0;
	PLAT_UI8   	n = 0x1;

	cmox_mac_handle_t *pMac_handle;
	cmox_hmac_handle_t hmac_handle;

	/*	RFC 5869 : output keying material must be
	 * 		- L <= 255*HashLen
	 * 		- N = ceil(L/HashLen) */
	if ( pOutput_keying_material == NULL
	  || ((output_keying_material_length/CMOX_SHA256_SIZE) + ((output_keying_material_length % CMOX_SHA256_SIZE) != 0)) > 255)
	{
		return (stse_ReturnCode_t)1;
	}

	pMac_handle = cmox_hmac_construct(&hmac_handle, CMOX_HMAC_SHA256);
	retval = cmox_mac_init(pMac_handle);

	if (retval != CMOX_MAC_SUCCESS)
	{
		return (stse_ReturnCode_t)1;
	}

	while (out_index < output_keying_material_length)
	{
		PLAT_UI16 left = output_keying_material_length - out_index;

		retval = cmox_mac_setKey(pMac_handle, pPseudorandom_key, pseudorandom_key_length);
		if (retval != CMOX_MAC_SUCCESS)
			break;
		retval = cmox_mac_append(pMac_handle, tmp, tmp_length);
		if (retval != CMOX_MAC_SUCCESS)
			break;
		retval = cmox_mac_append(pMac_handle, pInfo, info_length);
		if (retval != CMOX_MAC_SUCCESS)
			break;
		retval = cmox_mac_append(pMac_handle, &n, 1);
		if (retval != CMOX_MAC_SUCCESS)
			break;
		retval = cmox_mac_generateTag(pMac_handle, tmp, NULL);
		if (retval != CMOX_MAC_SUCCESS)
			break;

		left = left < CMOX_SHA256_SIZE ? left : CMOX_SHA256_SIZE;
		memcpy(pOutput_keying_material+out_index, tmp, left);

		tmp_length = CMOX_SHA256_SIZE;
		out_index += CMOX_SHA256_SIZE;
		n++;
	}

	cmox_mac_cleanup(pMac_handle);

	/*- Verify MAC compute return */
	if (retval != CMOX_MAC_SUCCESS)
	{
		memset(tmp, 0, CMOX_SHA256_SIZE);
		memset(pOutput_keying_material, 0, output_keying_material_length);
		return (stse_ReturnCode_t)1;
	} else {
		return STSE_OK;
	}
}


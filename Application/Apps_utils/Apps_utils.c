#include "Application/Apps_utils/Apps_utils.h"

void HardFault_Handler( void )
{
	printf("\n\r ## SYSTEM : HARDFAULT ## ");
	while(1);
}

/* STDIO redirect */
#if defined ( __GNUC__ ) && !defined (__ARMCC_VERSION)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar()
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	uart_putc(ch);
	return ch;
}
GETCHAR_PROTOTYPE
{
	return uart_getc();
}

char * get_key_type_str(stse_ecc_key_type_t key_type)
{
	switch(key_type)
	{
		case STSE_ECC_KT_NIST_P_256:
			return("NIST_P_256");
		case STSE_ECC_KT_NIST_P_384:
			return("NIST_P_384");
		case STSE_ECC_KT_NIST_P_521:
			return("NIST_P_521");
		case STSE_ECC_KT_BP_P_256:
			return(" BP_P_256 ");
		case STSE_ECC_KT_BP_P_384:
			return(" BP_P_384 ");
		case STSE_ECC_KT_BP_P_512:
			return(" BP_P_512 ");
		case STSE_ECC_KT_CURVE25519:
			return("CURVE25519");
		case STSE_ECC_KT_ED25519:
			return(" ED25519  ");
		default:
			return(PRINT_YELLOW"bad curve ID"PRINT_RESET);
	}
}

stse_ReturnCode_t get_curve_id_key_type(stsafea_ecc_curve_id_t curve_id, stse_ecc_key_type_t * pKey_type)
{
	stse_ecc_key_type_t curve_id_index;
	int diff;
	*pKey_type = STSE_ECC_KT_INVALID;

	/* Compare slot curve ID against each known curve ID to set the key type */
	for(curve_id_index=STSE_ECC_KT_NIST_P_256; curve_id_index<STSE_ECC_NUMBER_OF_CURVES; curve_id_index++)
	{
		diff = memcmp((PLAT_UI8*)&stse_ecc_info_table[curve_id_index].curve_id,
						(PLAT_UI8*)&curve_id,
						stse_ecc_info_table[curve_id_index].curve_id_total_length);
		if(diff == 0)
		{
			*pKey_type = curve_id_index;
			break;
		}
	}
	/* If the comparison loop reach the end and pKey_type is always as initialized return error */
	if((curve_id_index+1) == STSE_ECC_NUMBER_OF_CURVES
	&& *pKey_type==STSE_ECC_KT_INVALID)
	{
		return STSE_UNEXPECTED_ERROR;
	}

	return STSE_OK;
}

void apps_print_symmetric_key_table_info ( stse_Handler_t *pSTSE )
{
	stse_ReturnCode_t ret;
	PLAT_UI8 i;
	PLAT_UI8 slot_count;
	
	ret = stse_get_symmetric_key_slots_count(pSTSE, &slot_count);
	if( ret != STSE_OK )
	{
		printf("\n\n\r - stse_get_symmetric_key_slots_count : ERROR 0x%04x", ret);
		while(1);
	}

	stsafea_symmetric_key_slot_information_t symmetric_key_table[slot_count];

	ret = stse_get_symmetric_key_table_info(pSTSE, slot_count, symmetric_key_table);
	if( ret != STSE_OK )
	{
		printf("\n\n\r - stse_get_symmetric_key_slot_info : ERROR 0x%04x", ret);
		while(1);
	}

	printf("\n\n\r");
	printf("\n\r -------------------------------------+--------------------------------------------------------------------------+------------------------------------------------------------------");
	printf("\n\r                                      |                                  KEY USAGE                               |                            PARAMETERS                            ");
	printf("\n\r ------+------+----------+------------+--------+---------+-----------+---------+---------+-----------+-----------+-------------+--------------+------------+-------------+----------");
	printf("\n\r  SLOT | LOCK | KEY TYPE | MODE OF OP | DERIVE | MAC GEN | MAC VERIF | ENCRYPT | DECRYPT | ENC_CHUNK | DEC CHUNK | MIN MAC LEN | AUTH TAG LEN | CNT OFFSET |   CNT VAL   | HKDF RET ");
	printf("\n\r ------+------+----------+------------+--------+---------+-----------+---------+---------+-----------+-----------+-------------+--------------+------------+-------------+----------");

	for(i=0; i < slot_count; i++)
	{
		printf("\r\n   %02d  |", i);
		printf("   %s  |",(symmetric_key_table[i].lock_indicator == 0)? ".": "x" );
		if(symmetric_key_table[i].key_presence == 1)
		{
			printf(" %s  |", (symmetric_key_table[i].key_type == 0)?"AES 128":"AES 256" );
			/* Mode of operation */
			printf("    %s    |", (symmetric_key_table[i].mode_of_operation == 0x00)	? "CCM*" :
								  (symmetric_key_table[i].mode_of_operation == 0x01) 	? "CMAC" :
								  (symmetric_key_table[i].mode_of_operation == 0x02) 	? "ECB " :
								  (symmetric_key_table[i].mode_of_operation == 0x03) 	? "GCM " :
								  (symmetric_key_table[i].mode_of_operation == 0x04) 	? "HKDF" : "HMAC");
			/* Key usage */
			printf("    %s   |", (symmetric_key_table[i].key_usage.derive == 0)? ".":"x");
			printf("    %s    |", (symmetric_key_table[i].key_usage.mac_generation == 0)? ".":"x");
			printf("     %s     |", (symmetric_key_table[i].key_usage.mac_verification == 0)? ".":"x");
			printf("    %s    |", (symmetric_key_table[i].key_usage.encryption == 0)? ".":"x");
			printf("    %s    |", (symmetric_key_table[i].key_usage.decryption == 0)? ".":"x");
			printf("     %s     |", (symmetric_key_table[i].key_usage.chunks_encryption == 0)? ".":"x");
			printf("     %s     |", (symmetric_key_table[i].key_usage.chunks_decryption == 0)? ".":"x");
			/* Parameters for MAC */
			if(symmetric_key_table[i].mode_of_operation == STSAFEA_KEY_OPERATION_MODE_CMAC)
			{
				printf("    0x%02X     |", symmetric_key_table[i].parameters.cmac.minimum_MAC_length);
			} else if(symmetric_key_table[i].mode_of_operation == STSAFEA_KEY_OPERATION_MODE_HMAC)
			{
				printf("    0x%02X     |", symmetric_key_table[i].parameters.hmac.minimum_MAC_length);
			} else {
				printf("      .      |");
			}
			/* Parameters for AEAD */
			if(symmetric_key_table[i].mode_of_operation == STSAFEA_KEY_OPERATION_MODE_CCM)
			{
				printf("    0x%02X      |", symmetric_key_table[i].parameters.ccm.auth_tag_length);
				if(symmetric_key_table[i].parameters.ccm.counter_presence == 1)
				{
					printf("    %03d     |", symmetric_key_table[i].parameters.ccm.counter_offset_in_nonce);
					printf(" 0x%02X%02X%02X%02X |",
							symmetric_key_table[i].parameters.ccm.counter_value[3],
							symmetric_key_table[i].parameters.ccm.counter_value[2],
							symmetric_key_table[i].parameters.ccm.counter_value[1],
							symmetric_key_table[i].parameters.ccm.counter_value[0]);
				} else {
					printf("     .      |      .      |");
				}
			}
			else if(symmetric_key_table[i].mode_of_operation == STSAFEA_KEY_OPERATION_MODE_GCM)
			{
				printf("    0x%02X      |     .      |      .      |", symmetric_key_table[i].parameters.gcm.auth_tag_length);
			} else {
				printf("      .       |     .      |      .      |");
			}
			/* Parameters for Derivation */
			printf("    %s     ", (symmetric_key_table[i].mode_of_operation == STSAFEA_KEY_OPERATION_MODE_HKDF
							   &&  symmetric_key_table[i].parameters.hkdf.derivation_output_in_response_allowed == 1)? "x":".");
		} else {
			printf("    .     |     .      |    .   |    .    |     .     |    .    |    .    |     .     |     .     |      .      |      .       |     .      |      .      |    .");
		}
	}
}

void apps_print_symmetric_key_table_provisioning_control_fields ( stse_Handler_t *pSTSE )
{
	stse_ReturnCode_t ret;
	PLAT_UI8 i;
	PLAT_UI8 slot_count;
	stsafea_symmetric_key_slot_provisioning_ctrl_fields_t ctrl_fields;

	ret = stse_get_symmetric_key_slots_count(pSTSE, &slot_count);
	if( ret != STSE_OK )
	{
		printf("\n\n\r - stse_get_symmetric_key_slots_count : ERROR 0x%04x", ret);
		while(1);
	}

	printf("\n\n\r");
	printf("\n\r ------+----------------------------------------------------------------");
	printf("\n\r       |                    PROVISIONNING CTRL FIELDS                   ");
	printf("\n\r ------+--------------+---------+-----------+---------+---------+-------");
	printf("\n\r  SLOT | CHANGE RIGHT | DERIVED | PLAINTEXT | PUT KEY | WRAPPED | ECDHE ");
	printf("\n\r ------+--------------+---------+-----------+---------+---------+-------");

	for(i=0; i < slot_count; i++)
	{
		printf("\r\n   %02d  |", i);

		ret = stse_get_symmetric_key_slot_provisioning_ctrl_fields(pSTSE, i, &ctrl_fields);
		if( ret != STSE_OK )
		{
			printf("\n\n\r - stse_query_symmetric_key_slot_provisioning_ctrl_fields : ERROR 0x%04x", ret);
			while(1);
		}

		printf("       %s      |",(ctrl_fields.change_right == 0)? ".": "x" );
		printf("    %s    |",(ctrl_fields.derived == 0)? ".": "x" );
		printf("     %s     |",(ctrl_fields.plaintext == 0)? ".": "x" );
		printf("    %s    |",(ctrl_fields.put_key == 0)? ".": "x" );
		if(ctrl_fields.wrapped_authentication_key != 0xFF)
		{
			printf("  0x%02X   |", ctrl_fields.wrapped_authentication_key);
		} else {
			printf("    %s    |",(ctrl_fields.wrapped_anonymous == 0)? ".": "x" );
		}

		if(ctrl_fields.ECDHE_authentication_key != 0xFF)
		{
			printf(" 0x%02X ", ctrl_fields.ECDHE_authentication_key);
		} else {
			printf("   %s   ",(ctrl_fields.ECDHE_anonymous == 0)? ".": "x" );
		}
	}
}

void apps_print_asymmetric_key_table_info(stse_Handler_t *pSTSE)
{
	stse_ReturnCode_t ret;
	PLAT_UI8 i;
	PLAT_UI8 slot_count;
	PLAT_UI8 change_right;
	PLAT_UI16 global_usage_limit;
	stse_ecc_key_type_t key_type;

	ret = stse_get_ecc_key_slots_count(pSTSE, &slot_count);
	if( ret != STSE_OK )
	{
		printf("\n\n\r - stse_get_ecc_key_slots_count : ERROR 0x%04x", ret);
		while(1);
	}

	stsafea_private_key_slot_information_t pPrivate_key_table_info[slot_count];
	ret = stse_get_ecc_key_table_info(pSTSE, slot_count, &change_right, &global_usage_limit, pPrivate_key_table_info);
	if( ret != STSE_OK )
	{
		printf("\n\n\r - stse_get_ecc_key_table_info : ERROR 0x%04x", ret);
		while(1);
	}

	printf("\n\n\r");
	printf("\n\r -----------------------------------------------------------------------------------------");
	printf("\n\r               Change right : %s                  Global usage limit : %d", change_right == 1 ? "YES":"NO ", global_usage_limit);
	printf("\n\r ------+----------------------------------------------------------------------------------");
	printf("\n\r       |                                 ASYMMETRIC KEY INFO                              ");
	printf("\n\r ------+----------+------------+---------------+------------+---------+-------------------");
	printf("\n\r  SLOT | PRESENCE |    TYPE    | EdDSA Variant | Gen Key AC | Sig Gen | Key establishment ");
	printf("\n\r ------+----------+------------+---------------+------------+---------+-------------------");

	for(i=0; i < slot_count; i++)
	{
		get_curve_id_key_type(pPrivate_key_table_info[i].curve_id, &key_type);
		printf("\r\n  0x%02X |", pPrivate_key_table_info[i].slot_number);
		printf("    %s     |",(pPrivate_key_table_info[i].presence_flag == 0)? ".": "x" );
		printf(" %s |", (pPrivate_key_table_info[i].presence_flag == 0)? "     .    ":get_key_type_str(key_type));
		printf("   %s   |",(key_type != STSE_ECC_KT_CURVE25519 && key_type != STSE_ECC_KT_ED25519) ? "  None   " :
						   (pPrivate_key_table_info[i].mode_of_operation.EdDSA_variant == 0) 	   ? "  Pure   " :
						   (pPrivate_key_table_info[i].mode_of_operation.EdDSA_variant == 1) 	   ? "PreHashed" :
								  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	     "  ERROR  ");
		printf("  %s |",(pPrivate_key_table_info[i].mode_of_operation.generate_key_AC == 0) ? "  Free   " :
					   (pPrivate_key_table_info[i].mode_of_operation.generate_key_AC == 1) ? "  Host   " :
					 		  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	     "Forbidden");
		printf("    %s    |",(pPrivate_key_table_info[i].mode_of_operation.sig_gen_over_external_data 		 == 0 &&
						    pPrivate_key_table_info[i].mode_of_operation.sig_gen_over_internal_external_data == 0) ? ".": "x" );
		printf("         %s",(pPrivate_key_table_info[i].mode_of_operation.key_establishment == 0)? ".": "x" );
	}
}

void apps_print_host_key_provisioning_control_fields( stse_Handler_t *pSTSE )
{
	stsafea_host_key_provisioning_ctrl_fields_t provisioning_ctrl_fields;
	stse_ReturnCode_t ret;

	ret = stsafea_query_host_key_provisioning_ctrl_fields(pSTSE, &provisioning_ctrl_fields);

	if(ret != STSE_OK)
	{
		printf("\n\n\r - "PRINT_RED"stsafea_query_host_key_provisioning_ctrl_fields ERROR : 0x%04X"PRINT_RESET,ret);
		while(1);
	}

	printf("\n\r --------------+--------------+-----------+-------------------+----------------------------");
	printf("\n\r  Change right | Re-provision | Plaintext | Wrapped anonymous | Wrapped authentication key ");
	printf("\n\r --------------+--------------+-----------+-------------------+----------------------------");
	printf("\n\r      %s      |", (provisioning_ctrl_fields.change_right == 0)?"NO ":"YES" );
	printf("     %s      |", (provisioning_ctrl_fields.reprovision == 0)?"NO ":"YES" );
	printf("    %s    |", (provisioning_ctrl_fields.plaintext == 0)?"NO ":"YES" );
	printf("        %s        |", (provisioning_ctrl_fields.wrapped_anonymous == 0)?"NO ":"YES" );
	printf("            0x%02X            ", provisioning_ctrl_fields.wrapped_authentication_key);
	printf("\n\r --------------+--------------+-----------+-------------------+----------------------------");
}

void apps_print_generic_public_key_slot_configuration_flags( stse_Handler_t *pSTSE, PLAT_UI8 slot_number )
{
	stse_ReturnCode_t ret;
	PLAT_UI8 generic_public_key_presence;
	stse_ecc_key_type_t generic_public_key_type;
	stsafea_generic_public_key_configuration_flags_t generic_public_key_config;

	ret = stsafea_query_generic_public_key_slot_info(
			pSTSE,
			slot_number,
			&generic_public_key_presence,
			&generic_public_key_config,
			&generic_public_key_type);
	if(ret != STSE_OK)
	{
		printf("\n\n\r - "PRINT_RED"stsafea_query_generic_public_key_slot_info ERROR : 0x%04X"PRINT_RESET,ret);
		while(1);
	}

	printf("\n\r ------+--------------+--------------------------+----------------------------+--------------");
	printf("\n\r  Slot | Change right | Estabblish symmetric key | Start volatile KEK session | Key presence ");
	printf("\n\r ------+--------------+--------------------------+----------------------------+--------------");
	printf("\n\r  0x%02X |", slot_number );
	printf("      %s     |", (generic_public_key_config.change_right == 0)?"NO ":"YES" );
	printf("        %s         |", (generic_public_key_config.establish_symmetric_key == 0)?"FORBIDDEN":"ALLOWED  " );
	printf("          %s         |", (generic_public_key_config.start_volatile_kek_session == 0)?"FORBIDDEN":"ALLOWED  " );
	printf("      %s     ", (generic_public_key_presence == 0)?"NO ":"YES");
	printf("\n\r ------+--------------+--------------------------+----------------------------+--------------");
}

void apps_print_hex_buffer (uint8_t* buffer ,uint16_t buffer_size )
{
	uint16_t i;
	for(i=0;i<buffer_size;i++)
	{
		if(i%16 == 0 )
		{
			printf(" \n\r ");
		}
		printf(" 0x%02X",buffer[i]);
	}
}

void apps_print_data_partition_record_table ( stse_Handler_t *pSTSE )
{
	uint8_t i;
	stse_ReturnCode_t ret;
	uint16_t data_partition_record_table_length;
	uint8_t total_zone_count;

	/*- Get total partition count */
	ret = stse_data_storage_get_total_partition_count(
			pSTSE,
			&total_zone_count
	);
	if( ret != STSE_OK )
	{
		printf("\n\n\r ### stsafe_get_total_partition_count : ERROR 0x%04X", ret);
		while(1);
	}

	/*- Allocate data partition table  */
	stsafea_data_partition_record_t  data_partition_record_table[total_zone_count];

	/*- Get data partition table  */
	data_partition_record_table_length = sizeof(data_partition_record_table);
	ret = stse_data_storage_get_partitioning_table(
			pSTSE,
			total_zone_count,
			data_partition_record_table,
			data_partition_record_table_length
	);
	if(ret != STSE_OK )
	{
		printf("\n\r ### stse_get_data_partitions_configuration : ERROR 0x%04X", ret);
		while(1);
	} else {
		printf("\n\n\r - stse_get_data_partitions_configuration");
	}

	printf("\n\r  ID | COUNTER | DATA SEGMENT SIZE | READ AC CR |  READ AC | UPDATE AC CR | UPDATE AC |  COUNTER VAL \r\n");
	for(i=0;i<total_zone_count;i++)
	{
		/*- print id (col 1)*/
		printf(" %03d | ", data_partition_record_table[i].index);

		/*- print counter presence (col 2)*/
		printf("   %c    |",(data_partition_record_table[i].zone_type) == 0 ? '.' : 'x');

		/*- print data segment size (col 3) */
		printf("       %04u        | ",data_partition_record_table[i].data_segment_length);

		/*- read ac change right (col 4) */
		printf(" %s | ",(data_partition_record_table[i].read_ac_cr) == 1 ? " ALLOWED " : " DENIED  ");

		/*- read ac right (col 5) */
		switch (data_partition_record_table[i].read_ac)
		{
			case STSAFEA_AC_ALWAYS :
				printf(" ALWAYS  |");
				break;

			case STSAFEA_AC_HOST :
				printf("   HOST  |");
				break;

			case STSAFEA_AC_AUTH :
				printf("   AUTH  |");
				break;

			default  :
				printf("  NEVER  |");
				break;

		}

		/*- update ac change right (col 4) */
		printf(" %s | ",(data_partition_record_table[i].update_ac_cr) == 1 ? "   ALLOWED  " : "   DENIED   ");

		/*- update ac right (col 5) */
		switch (data_partition_record_table[i].update_ac)
		{
			case STSAFEA_AC_ALWAYS :
			printf("  ALWAYS  |");
				break;

			case STSAFEA_AC_HOST :
				printf("   HOST   |");
				break;

			case STSAFEA_AC_AUTH :
				printf("  AUTH    |");
				break;

			default  :
				printf("  NEVER   |");
				break;

		}

		printf(" %06" PRId32 "\r\n", data_partition_record_table[i].counter_value);

	}
}

void apps_print_command_ac_record_table ( stse_cmd_authorization_record_t*  command_ac_record_table,
		uint8_t total_command_count)
{
	uint8_t i;

	printf("\n\r  HEADER | EXT-HEADER |    AC   | CMDEnc | RSPEnc  ");
	for(i=0;i<total_command_count;i++)
	{
		printf("\n\r");
		/*- print HEADER (col 1)*/
		printf("   0x%02X  | ",command_ac_record_table[i].header );

		/*- print EXT-HEADER (col 2)*/
		if (command_ac_record_table[i].extended_header != 0)
		{
			printf("    0x%02X   | ", command_ac_record_table[i].extended_header);
		} else {
			printf("     -     | ");
		}

		/*- Access conditions (col 3) */
		switch (command_ac_record_table[i].command_AC)
		{
			case STSE_CMD_AC_NEVER :
				printf(" NEVER  |");
				break;

			case STSE_CMD_AC_FREE :
				printf("  FREE  |");
				break;

			case STSE_CMD_AC_ADMIN :
				printf(" ADMIN  |");
				break;

			case STSE_CMD_AC_HOST :
				printf("  HOST  |");
				break;

			case STSE_CMD_AC_ADMIN_OR_PWD :
				printf("ADM/PWD |");
				break;

			case STSE_CMD_AC_ADMIN_OR_HOST :
				printf(" ADM/HST |");
				break;

			default  :
				printf("  --?--  |");
				break;
		}

		/*- cmd encryption (col 4) */
		printf(" %s | ",(command_ac_record_table[i].host_encryption_flags.cmd) == 1 ? "  YES " : "  NO  ");

		/*- rsp encryption (col 5) */
		printf(" %s | ",(command_ac_record_table[i].host_encryption_flags.rsp) == 1 ? "  YES " : "  NO  ");

	}
}

void apps_print_life_cycle_state (stsafea_life_cycle_state_t life_cycle_state)
{
	switch(life_cycle_state)
	{
		case STSAFEA_LCS_BORN : printf("BORN") ; break;
		case STSAFEA_LCS_PATCHING : printf("PATCHING") ; break;
		case STSAFEA_LCS_OPERATIONAL : printf("OPERATIONAL") ; break;
		case STSAFEA_LCS_TERMINATED : printf("TERMINATED") ; break;
		case STSAFEA_LCS_BORN_AND_LOCKED : printf("BORN_AND_LOCKED") ; break;
		case STSAFEA_LCS_OPERATIONAL_AND_LOCKED : printf("OPERATIONAL_AND_LOCKED") ; break;
		default : printf("UNKNOWN") ; break;
	}
};

void apps_terminal_init (uint32_t baudrate)
{
	(void)baudrate;
	/* - Initialize UART for example output log (baud 115200)  */
	uart_init(115200);
	/* Disable I/O buffering for STDOUT stream*/
	setvbuf(stdout, NULL, _IONBF, 0);
	/* - Clear terminal */
	printf(PRINT_RESET PRINT_CLEAR_SCREEN);
}

uint8_t apps_terminal_read_string (char * string, uint8_t * length)
{
	char c = 1;
	uint16_t i = 0;
	while(c != '\r')
	{
		c = uart_getc();
		if(string != NULL)
		{
			string[i] = c;
		}
		i++;
		if(((length != NULL) && (i > *length))
		|| ((length == NULL) && (i > 0)))
		{
			return 1;
		}
	}
	if(length != NULL)
	{
		*length = i;
	}
	return 0;
}

uint8_t apps_terminal_read_unsigned_integer (uint16_t * integer)
{
	uint8_t length = 6;
	char string[length];
	if(apps_terminal_read_string(string, &length))
	{
		return 1;
	}

	*integer = (uint8_t)atoi(string);

	return 0;
}

uint32_t apps_generate_random_number (void)
{
	return rng_generate_random_number();
}

void apps_randomize_buffer(uint8_t * pBuffer , uint16_t buffer_length)
{
	for(uint16_t i=0 ; i<buffer_length;i++) {
		*(pBuffer+i) =  (rng_generate_random_number() & 0xFF);
	}
}

uint8_t apps_compare_buffers(uint8_t *pBuffer1 , uint8_t *pBuffer2 , uint16_t buffers_length)
{

	for(uint16_t i=0;i<buffers_length;i++)
	{
		if(*pBuffer1 != *pBuffer2)
		{
			return 1;
		}
	}

	return 0;
}

void apps_delay_ms(uint16_t ms)
{
	delay_ms(ms);
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Drivers/uart/uart.h"
#include "Drivers/rng/rng.h"
#include "Drivers/delay_ms/delay_ms.h"
#include "stselib.h"

#ifndef APPS_UTILS
#define APPS_UTILS

#define PRINT_CLEAR_SCREEN		"\x1B[1;1H\x1B[2J"
#define PRINT_BLINK				"\x1B[5m"
#define PRINT_UNDERLINE 		"\x1B[4m"
#define PRINT_BELL				"\a"
#define PRINT_BOLD				"\x1B[1m"
#define PRINT_ITALIC			"\x1B[3m"
#define PRINT_RESET				"\x1B[0m"
#ifdef DISABLE_PRINT_COLOR
#define PRINT_BLACK			""      /* Black */
#define PRINT_RED			""      /* Red */
#define PRINT_GREEN			""      /* Green */
#define PRINT_YELLOW		""      /* Yellow */
#define PRINT_BLUE			""      /* Blue */
#define PRINT_MAGENTA		""      /* Magenta */
#define PRINT_CYAN			""      /* Cyan */
#define PRINT_WHITE			""      /* White */
#else
#define PRINT_BLACK			"\x1B[30m"      /* Black */
#define PRINT_RED			"\x1B[31m"      /* Red */
#define PRINT_GREEN			"\x1B[32m"      /* Green */
#define PRINT_YELLOW		"\x1B[33m"      /* Yellow */
#define PRINT_BLUE			"\x1B[34m"      /* Blue */
#define PRINT_MAGENTA		"\x1B[35m"      /* Magenta */
#define PRINT_CYAN			"\x1B[36m"      /* Cyan */
#define PRINT_WHITE			"\x1B[37m"      /* White */

#endif

void apps_terminal_init (uint32_t baudrate);

uint8_t apps_terminal_read_string(char *string, uint8_t *length);

uint8_t apps_terminal_read_unsigned_integer(uint16_t *integer);

void apps_print_data_partition_record_table(stse_Handler_t *pSTSE);

void apps_print_symmetric_key_table_info(stse_Handler_t *pSTSE);

void apps_print_symmetric_key_table_provisioning_control_fields ( stse_Handler_t *pSTSE );

void apps_print_asymmetric_key_table_info(stse_Handler_t *pSTSE);

void apps_print_host_key_provisioning_control_fields( stse_Handler_t *pSTSE );

void apps_print_generic_public_key_slot_configuration_flags( stse_Handler_t *pSTSE, PLAT_UI8 slot_number );

void apps_print_hex_buffer (uint8_t* buffer ,uint16_t buffer_size);

void apps_print_command_ac_record_table(stse_cmd_authorization_record_t*  command_ac_record_table,
		uint8_t total_command_count);

void apps_print_life_cycle_state (stsafea_life_cycle_state_t life_cycle_state);

uint32_t apps_generate_random_number (void);

void apps_randomize_buffer(uint8_t *pBuffer , uint16_t buffer_length);

uint8_t apps_compare_buffers(uint8_t *pBuffer1 , uint8_t *pBuffer2 , uint16_t buffers_length);

void apps_delay_ms(uint16_t ms);

#endif /*APPS_UTILS*/

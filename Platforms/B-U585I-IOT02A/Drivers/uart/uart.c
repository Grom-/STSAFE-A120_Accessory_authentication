 /******************************************************************************
 * \file	uart.c
 * \brief   UART driver for STM32U5
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

#include <Drivers/uart/uart.h>

#ifdef STM32G0
void uart_init (uint32_t baudrate)
{
	/* - Set prescaler & baudrate (baud = usart_ker_ck_pres / BRR ) */
	USART1->BRR = SystemCoreClock/baudrate ;
	/* - Configure UART */
	USART1->CR1 = ((1<<USART_CR1_TE_Pos) | // Enable TX
			       (1<<USART_CR1_RE_Pos)  // Enable RX
	);
	/* - Enable UART2 */
	USART1->CR1 |= USART_CR1_UE;
}


void uart_putc(uint8_t c)
{
	/* - Wait for TX empty */
	while(!(USART1->ISR & USART_ISR_TXE_TXFNF));
	/* - fill transmit buffer */
	USART1->TDR = c;
}

uint8_t uart_getc(void)
{
	/* - Wait for RX not empty */
	while(!(USART1->ISR & USART_ISR_RXNE_RXFNE));
	/* - fill transmit buffer */
	return USART1->RDR;
}
#endif

void uart_init (uint32_t baudrate)
{
	/* - Set prescaler & baudrate (baud = usart_ker_ck_pres / BRR ) */
	USART1->GTPR = (0x1UL << USART_GTPR_PSC_Pos);
	USART1->BRR = SystemCoreClock/baudrate ;
	/* Enables receive transmit mode  */
	USART1->CR1 |= ((1<<USART_CR1_TE_Pos) | // Enable TX
	              (1<<USART_CR1_RE_Pos)  // Enable RX
	);
	USART1->CR3 |= USART_CR3_OVRDIS;
	/* - Enable UART2 */
	USART1->CR1 |= USART_CR1_UE;
}


void uart_putc(uint8_t c)
{
	/* - Wait for TX empty */
	while(!(USART1->ISR & USART_ISR_TXE));
	/* - fill transmit buffer */
	USART1->TDR = c;
	while(!(USART1->ISR & USART_ISR_TXE));
}

uint8_t uart_getc(void)
{
	/* - Wait for RX not empty */
	while(!(USART1->ISR & USART_ISR_RXNE));
	/* - fill transmit buffer */
	return USART1->RDR;
}

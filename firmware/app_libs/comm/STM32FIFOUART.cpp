
#include <STM32FIFOUART.h>

#define fifo_available(buffer, size) (buffer != NULL && size > 0)

// ----------------------------------------------------------------------------
void STM32_FIFO_UART::irq()
{
	uint32_t isrflags   = READ_REG(m_huart->Instance->ISR);
	uint32_t cr1its     = READ_REG(m_huart->Instance->CR1);
	uint32_t cr3its     = READ_REG(m_huart->Instance->CR3);

	uint32_t errorflags;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE));
	if (errorflags == 0U)
	{
		/* UART in mode Receiver ---------------------------------------------------*/
		if (((isrflags & USART_ISR_RXNE) != 0U)
				&& ((cr1its & USART_CR1_RXNEIE) != 0U))
		{
			uint8_t byte = (uint8_t)(READ_REG(m_huart->Instance->RDR) & 0xFFU);

			if(fifo_available(m_fifo.rx_buffer, m_fifo.rx_buffer_size) && !fifo_full(m_fifo.rx_incnt, m_fifo.rx_outcnt, m_fifo.rx_buffer_size))
			{
				fifo_in(byte, m_fifo.rx_buffer, m_fifo.rx_incnt, m_fifo.rx_buffer_size);
			}

			return;
		}
	}

	/* If some errors occur */
	if ((errorflags != 0U)
			&& (((cr3its & USART_CR3_EIE) != 0U)
					|| ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != 0U)))
	{
	/* UART parity error interrupt occurred -------------------------------------*/
	if (((isrflags & USART_ISR_PE) != 0U) && ((cr1its & USART_CR1_PEIE) != 0U))
	{
		__HAL_UART_CLEAR_FLAG(m_huart, UART_CLEAR_PEF);
	}

	/* UART frame error interrupt occurred --------------------------------------*/
	if (((isrflags & USART_ISR_FE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
	{
		__HAL_UART_CLEAR_FLAG(m_huart, UART_CLEAR_FEF);
	}

	/* UART noise error interrupt occurred --------------------------------------*/
	if (((isrflags & USART_ISR_NE) != 0U) && ((cr3its & USART_CR3_EIE) != 0U))
	{
		__HAL_UART_CLEAR_FLAG(m_huart, UART_CLEAR_NEF);
	}

	/* UART Over-Run interrupt occurred -----------------------------------------*/
	if (((isrflags & USART_ISR_ORE) != 0U)
			&& (((cr1its & USART_CR1_RXNEIE) != 0U) ||
					((cr3its & USART_CR3_EIE) != 0U)))
	{
		__HAL_UART_CLEAR_FLAG(m_huart, UART_CLEAR_OREF);
	}

	return;

	} /* End if some error occurs */

	/* UART wakeup from Stop mode interrupt occurred ---------------------------*/
	if (((isrflags & USART_ISR_WUF) != 0U) && ((cr3its & USART_CR3_WUFIE) != 0U))
	{
		__HAL_UART_CLEAR_FLAG(m_huart, UART_CLEAR_WUF);

		return;
	}

	/* UART in mode Transmitter ------------------------------------------------*/
	if (((isrflags & USART_ISR_TXE) != 0U)
			&& ((cr1its & USART_CR1_TXEIE) != 0U))
	{
		if(fifo_available(m_fifo.tx_buffer, m_fifo.tx_buffer_size) && !fifo_empty(m_fifo.tx_incnt, m_fifo.tx_outcnt))
		{
			uint8_t byte;
			fifo_out(byte, m_fifo.tx_buffer, m_fifo.tx_outcnt, m_fifo.tx_buffer_size);
			m_huart->Instance->TDR = byte;
		}
		else
		{
			/* Disable the UART Transmit Data Register Empty Interrupt */
			CLEAR_BIT(m_huart->Instance->CR1, USART_CR1_TXEIE);
		}
		return;
	}
}

// ----------------------------------------------------------------------------
size_t STM32_FIFO_UART::rx_fifo_size()
{
	return fifo_space_occupied(m_fifo.rx_incnt, m_fifo.rx_outcnt, m_fifo.rx_buffer_size);
}

// ----------------------------------------------------------------------------
size_t STM32_FIFO_UART::tx_fifo_size()
{
	return fifo_space_occupied(m_fifo.tx_incnt, m_fifo.tx_outcnt, m_fifo.tx_buffer_size);
}

// ----------------------------------------------------------------------------
void STM32_FIFO_UART::rx_start()
{
    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(m_huart->Instance->CR3, USART_CR3_EIE);

    /* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
    SET_BIT(m_huart->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

// ----------------------------------------------------------------------------
void STM32_FIFO_UART::write(DataRef block)
{
	while(block.size-- > 0)
	{
		while (fifo_full(m_fifo.tx_incnt, m_fifo.tx_outcnt, m_fifo.tx_buffer_size));

		__disable_irq();

		fifo_in(*(block.data++), m_fifo.tx_buffer, m_fifo.tx_incnt, m_fifo.tx_buffer_size);

		__enable_irq();

	    /* Enable the Transmit Data Register Empty interrupt */
	    SET_BIT(m_huart->Instance->CR1, USART_CR1_TXEIE);
	}
}

// ----------------------------------------------------------------------------
uint8_t STM32_FIFO_UART::read()
{
	while(fifo_empty(m_fifo.rx_incnt, m_fifo.rx_outcnt)) {}

	__disable_irq();

	uint8_t byte;
	fifo_out(byte, m_fifo.rx_buffer, m_fifo.rx_outcnt, m_fifo.rx_buffer_size);

	__enable_irq();

	return byte;
}
















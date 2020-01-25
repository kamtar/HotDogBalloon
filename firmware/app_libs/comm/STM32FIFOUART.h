
#ifndef COMM_STM32FIFOUART_H_
#define COMM_STM32FIFOUART_H_

#include <cstdint>
#include "fifo.h"

#include "stm32l0xx_hal.h"

#include <DataRef.hpp>

class STM32_FIFO_UART
{
public:
	STM32_FIFO_UART(UART_HandleTypeDef * huart, DataRef rx_buf, DataRef tx_buf);

	void irq();

	size_t rx_fifo_size();
	size_t tx_fifo_size();

	void rx_start();

	void write(DataRef data);
	uint8_t read();

	void enable();
	void disable();

private:
	UART_HandleTypeDef * m_huart;

	struct
	{
		uint8_t * 	rx_buffer;
		size_t		rx_buffer_size;
		uint32_t	rx_incnt;
		uint32_t	rx_outcnt;

		uint8_t * 	tx_buffer;
		size_t		tx_buffer_size;
		uint32_t	tx_incnt;
		uint32_t	tx_outcnt;

	}m_fifo;
};

inline STM32_FIFO_UART::STM32_FIFO_UART(UART_HandleTypeDef * huart, DataRef rx_buf, DataRef tx_buf) :
		m_huart(huart),
		m_fifo { rx_buf.data, rx_buf.size, 0, 0, tx_buf.data, tx_buf.size, 0, 0 }
		{}

#endif /* COMM_STM32FIFOUART_H_ */

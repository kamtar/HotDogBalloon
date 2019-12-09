/*
 * SamD21Spi_cpp.c
 *
 * Created: 18/01/2019 20:44:50
 *  Author: Kamtar
 */ 
 #include "STM32Spi.hpp"



STM32Spi::STM32Spi(SPI_TypeDef* spi_pointer, IOutputPin& select_pin) : m_spi_pointer(spi_pointer), m_select_pin(select_pin)
{
	m_is_ready = false;
}

void STM32Spi::Init()
{
	if(m_is_ready)
		return;

	 m_spi.Instance = m_spi_pointer;
	 m_spi.Init.Mode = SPI_MODE_MASTER;
	 m_spi.Init.Direction = SPI_DIRECTION_2LINES;
	 m_spi.Init.DataSize = SPI_DATASIZE_8BIT;
	 m_spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	 m_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	 m_spi.Init.NSS = SPI_NSS_SOFT;
	 m_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	 m_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	 m_spi.Init.TIMode = SPI_TIMODE_DISABLE;
	 m_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	 m_spi.Init.CRCPolynomial = 10;

	  if (HAL_SPI_Init(&m_spi) == HAL_OK)
	  {
		  m_is_ready = true;
	  }
}
 	
void STM32Spi::Write(DataRef data)
{
	//assert(m_spi_desc == nullptr);
	if( !m_is_ready)
		return;

	m_select_pin.clear();
	HAL_SPI_Transmit(&m_spi, data.data, data.size, 100);
	m_select_pin.set();
}
 	
void STM32Spi::Read(DataRef data_rq, DataRef data_out)
{
	//assert(m_spi_desc == nullptr);
	if(!m_is_ready)
		return;

	m_select_pin.clear();

	if(data_rq.size > 0)
		  HAL_SPI_Transmit(&m_spi, data_rq.data, data_rq.size, 100);

	HAL_SPI_Receive(&m_spi, data_out.data, data_out.size, 100);
	m_select_pin.set();
}

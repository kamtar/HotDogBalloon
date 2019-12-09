/*
 * SamD21Spi_hpp.h
 *
 * Created: 18/01/2019 20:45:02
 *  Author: Kamtar
 */ 


#ifndef SAMD21SPI_HPP_H_
#define SAMD21SPI_HPP_H_
#include "main.h"
#include "ISpi.hpp"
#include "IOutputPin.hpp"


class STM32Spi : public ISpi
{
public:
	STM32Spi(SPI_TypeDef* spi_pointer, IOutputPin& select_pin);
	virtual void Init() override;
	virtual void Write(DataRef data) override;
	virtual void Read(DataRef data_rq, DataRef data_out) override;

private:
	SPI_TypeDef* m_spi_pointer;
	SPI_HandleTypeDef m_spi;
	IOutputPin& m_select_pin;
	bool m_is_ready;
};

#endif /* SAMD21SPI_HPP_H_ */

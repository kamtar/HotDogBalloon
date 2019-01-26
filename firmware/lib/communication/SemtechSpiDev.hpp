/*
 * SemtechSpiDev.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_COMMUNICATION_SEMTECHSPIDEV_HPP_
#define LIB_COMMUNICATION_SEMTECHSPIDEV_HPP_

#include <stdint.h>
#include <ISpiRegDev.hpp>
#include <ISpi.hpp>
#include <IOutputPin.hpp>

class SemtechSpiDev : public ISpiRegDev
{
public:
	SemtechSpiDev(ISpi& spi, IOutputPin& reset_pin, IOutputPin& di0_pin);

	//ISpiRegDev
	virtual void Init() override;
	virtual void Reset() override;
	virtual void writeReg8(uint8_t address, uint8_t value) override;
	virtual uint8_t readReg8(uint8_t address) override;
	//ISpiRegDev

	SemtechSpiDev( const SemtechSpiDev& ) = delete; // non construction-copyable
	SemtechSpiDev& operator=( const SemtechSpiDev& ) = delete; // non copyable

private:
	ISpi &m_spi;
	uint8_t cmd_buffer[2];

	IOutputPin& m_reset_pin;
	IOutputPin& m_di0_pin;
};

#endif /* LIB_COMMUNICATION_SEMTECHSPIDEV_HPP_ */

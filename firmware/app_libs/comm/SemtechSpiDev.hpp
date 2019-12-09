/*
 * SemtechSpiDev_hpp.h
 *
 * Created: 18/01/2019 21:09:52
 *  Author: Kamtar
 */ 


#ifndef SEMTECHSPIDEV_HPP_H_
#define SEMTECHSPIDEV_HPP_H_
#include <stdint.h>
#include "ISpiRegDev.hpp"
#include "ISpi.hpp"
#include "IOutputPin.hpp"
#include "ContextLock.hpp"

class SemtechSpiDev : public ISpiRegDev
{
public:
	SemtechSpiDev(ISpi& spi, IOutputPin& reset_pin, IOutputPin& di0_pin);
	virtual void Init() override;
	virtual void Reset() override;
	virtual void writeReg8(uint8_t address, uint8_t value) override;
	virtual uint8_t readReg8(uint8_t address) override;

	SemtechSpiDev( const SemtechSpiDev& ) = delete; // non construction-copyable
	SemtechSpiDev& operator=( const SemtechSpiDev& ) = delete; // non copyable
private:
	ISpi &m_spi;
	uint8_t cmd_buffer[2];

	IOutputPin& m_reset_pin;
	IOutputPin& m_di0_pin;

};



#endif /* SEMTECHSPIDEV_HPP_H_ */
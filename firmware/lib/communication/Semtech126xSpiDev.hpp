/*
 * SemtechSpiDev.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_COMMUNICATION_SEMTECH126xSPIDEV_HPP_
#define LIB_COMMUNICATION_SEMTECH126xSPIDEV_HPP_

#include <DataRef.hpp>
#include <stdint.h>
#include <ISpiRegDev.hpp>
#include <ISpi.hpp>
#include <IOutputPin.hpp>

class Semtech126xSpiDev : public ISpiRegDev
{
public:
	Semtech126xSpiDev(ISpi& spi, IOutputPin& reset_pin, IOutputPin& di0_pin);

	//ISpiRegDev
	virtual void Init() override;
	virtual void Reset() override;
	virtual void writeReg8(uint8_t address, uint8_t value) override;
	virtual uint8_t readReg8(uint8_t address) override;
	//ISpiRegDev

	inline const IDataContainer& getCommandBuffer() { return cmd_buffer; };
	Semtech126xSpiDev( const Semtech126xSpiDev& ) = delete; // non construction-copyable
	Semtech126xSpiDev& operator=( const Semtech126xSpiDev& ) = delete; // non copyable

private:
	ISpi &m_spi;

	DataContainer<48> cmd_buffer;

	IOutputPin& m_reset_pin;
	IOutputPin& m_di0_pin;
};

#endif /* LIB_COMMUNICATION_SEMTECH126xSPIDEV_HPP_ */

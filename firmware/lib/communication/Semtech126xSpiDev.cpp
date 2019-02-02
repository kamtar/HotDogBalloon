/*
 * SemtechSpiDev.cpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#include <Semtech126xSpiDev.hpp>

#if __has_include("main.h")
#include <main.h>
#else
#warning "No main.h found!"
#endif

Semtech126xSpiDev::Semtech126xSpiDev(ISpi& spi, IOutputPin& reset_pin, IOutputPin& di0_pin) : m_spi(spi),  cmd_buffer(), m_reset_pin(reset_pin), m_di0_pin(di0_pin)
{

}

void Semtech126xSpiDev::Init()
{
	m_spi.Init();
	Reset();
}

void Semtech126xSpiDev::Reset()
{
	m_reset_pin.clear();
	delay_ms(10);
	m_reset_pin.set();
	delay_ms(10);
}

void Semtech126xSpiDev::writeReg8(uint8_t address, uint8_t value)
{


}

uint8_t Semtech126xSpiDev::readReg8(uint8_t address)
{

	return 0;
}

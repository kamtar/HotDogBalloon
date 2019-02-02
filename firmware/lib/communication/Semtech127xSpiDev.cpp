/*
 * SemtechSpiDev.cpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#include <Semtech127xSpiDev.hpp>

#if __has_include("main.h")
#include <main.h>
#else
#warning "No main.h found!"
#endif

Semtech127xSpiDev::Semtech127xSpiDev(ISpi& spi, IOutputPin& reset_pin, IOutputPin& di0_pin) : m_spi(spi), m_reset_pin(reset_pin), m_di0_pin(di0_pin)
{
	cmd_buffer[0] = 0;
	cmd_buffer[1] = 0;
}

void Semtech127xSpiDev::Init()
{
	m_spi.Init();
	Reset();
}

void Semtech127xSpiDev::Reset()
{
	m_reset_pin.clear();
	delay_ms(10);
	m_reset_pin.set();
	delay_ms(10);
}

void Semtech127xSpiDev::writeReg8(uint8_t address, uint8_t value)
{
	cmd_buffer[0] = address | 0x80;
	cmd_buffer[1] = value;

	m_spi.Write(SimpleDataRef{cmd_buffer ,2});
}

uint8_t Semtech127xSpiDev::readReg8(uint8_t address)
{
	cmd_buffer[0] = address;
	m_spi.Read(SimpleDataRef{cmd_buffer ,1}, SimpleDataRef{&cmd_buffer[1] ,1});
	return cmd_buffer[1];
}

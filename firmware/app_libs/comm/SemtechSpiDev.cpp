/*
 * SemtechSpiDev_cpp.c
 *
 * Created: 18/01/2019 21:09:41
 *  Author: Kamtar
 */ 
#include "SemtechSpiDev.hpp"
#include <main.h>

SemtechSpiDev::SemtechSpiDev(ISpi& spi, IOutputPin& reset_pin, IOutputPin& di0_pin ) : 
	m_spi(spi), m_reset_pin(reset_pin), m_di0_pin(di0_pin)
{
	cmd_buffer[0] = 0;
	cmd_buffer[1] = 0;
}

void SemtechSpiDev::Init( )
{
	Reset();
}

void SemtechSpiDev::Reset()
{	
	m_reset_pin.clear();
	delay_ms(10);
	m_reset_pin.set();
	delay_ms(10);
}

void SemtechSpiDev::writeReg8(uint8_t address, uint8_t value)
{
	cmd_buffer[0] = address | 0x80; 
	cmd_buffer[1] = value;

	m_spi.Write(DataRef{cmd_buffer ,2});
}

uint8_t SemtechSpiDev::readReg8(uint8_t address)
{
	cmd_buffer[0] = address;
	m_spi.Read(DataRef{cmd_buffer ,1}, DataRef{&cmd_buffer[1] ,1});
	return cmd_buffer[1];
}

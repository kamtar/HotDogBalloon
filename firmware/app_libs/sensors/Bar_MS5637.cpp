/*
 * Bar_MS5637.cpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#include "Bar_MS5637.hpp"

Bar_MS5637::Bar_MS5637(I_I2C& i2c) : m_i2c(i2c)
{
	// TODO Auto-generated constructor stub

}

bool Bar_MS5637::Init()
{
	  uint16_t adc = 0;
	 uint8_t command = 0x1E;


	 m_i2c.Init();
	 m_i2c.Write(m_addr, DataRef{&command,1});
	 delay_ms(100);
	 command = 0x40;
	 m_i2c.Write(m_addr, DataRef{&command,1});
	 delay_ms(10);
	 command = 0x50;
		 m_i2c.Write(m_addr, DataRef{&command,1});
		 delay_ms(10);
	 command=0;
	m_i2c.Read(m_addr,DataRef{&command,1}, DataRef{(uint8_t*)&adc,2});

	command = 0;
	return true;
}


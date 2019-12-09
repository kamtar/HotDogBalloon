/*
 * Bar_MS5637.cpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#include "Temp_PCT2075.hpp"

Temp_PCT2075::Temp_PCT2075(I_I2C& i2c) : m_i2c(i2c)
{
	// TODO Auto-generated constructor stub

}

bool Temp_PCT2075::Init()
{
	  uint16_t adc = 0;
	 uint8_t command = 0x1E;


	 m_i2c.Init();

	return true;
}

float Temp_PCT2075::get_temp()
{
	 uint8_t command = 0;
	  uint16_t data = 0;

		m_i2c.Read(m_addr,DataRef{&command,1}, DataRef{(uint8_t*)&data,2});

		float res = (((int16_t)((uint8_t*)&data)[1] | (uint16_t)(((uint8_t*)&data)[0] <<8))>>5) * 0.125f;
		return res;
}


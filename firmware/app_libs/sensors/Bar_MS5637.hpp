/*
 * Bar_MS5637.hpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#ifndef SENSORS_BAR_MS5637_HPP_
#define SENSORS_BAR_MS5637_HPP_
#include <I_I2C.hpp>
#include <main.h>
class Bar_MS5637
{
public:
	Bar_MS5637(I_I2C& i2c);
	bool Init();
private:
	I_I2C& m_i2c;
	const uint8_t m_addr = 0xEC;
};

#endif /* SENSORS_BAR_MS5637_HPP_ */

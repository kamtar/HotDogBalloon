/*
 * Temp_PCT2075.hpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#ifndef SENSORS_Temp_PCT2075_HPP_
#define SENSORS_Temp_PCT2075_HPP_
#include <I_I2C.hpp>
#include <main.h>
class Temp_PCT2075
{
public:
	Temp_PCT2075(I_I2C& i2c);
	bool Init();

	void shutdown();
	void normal();

	int get_temp();
private:
	I_I2C& m_i2c;
	const uint8_t m_addr = 0x90;
};

#endif /* SENSORS_BAR_MS5637_HPP_ */

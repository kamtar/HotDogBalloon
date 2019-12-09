/*
 * STM32_I2C.hpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#ifndef SAMD21_I2C_HPP_H_
#define SAMD21_I2C_HPP_H_
#include "main.h"
#include "I_I2C.hpp"


class STM32_I2C : public I_I2C
{
public:
	STM32_I2C(I2C_TypeDef* i2c_pointer);
	virtual void Init() override;
	virtual void Write(uint8_t addr,DataRef data) override;
	virtual void Read(uint8_t addr,DataRef data_rq, DataRef data_out) override;

private:

	I2C_HandleTypeDef m_i2c;
	bool m_is_ready;
};

#endif /* SAMD21_I2C_HPP_H_ */



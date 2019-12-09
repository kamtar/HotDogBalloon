/*
 * I_I2C.hpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#ifndef COMM_I_I2C_HPP_
#define COMM_I_I2C_HPP_

 #include "DataRef.hpp"

 class I_I2C
 {
 public:
	virtual void Init()=0;
	virtual void Write(uint8_t addr, DataRef data)=0;
	virtual void Read(uint8_t addr, DataRef data_rq, DataRef data_out)=0;

	I_I2C() {};
	I_I2C( const I_I2C& ) = delete; // non construction-copyable
	I_I2C& operator=( const I_I2C& ) = delete; // non copyable
 };

#endif /* COMM_I_I2C_HPP_ */

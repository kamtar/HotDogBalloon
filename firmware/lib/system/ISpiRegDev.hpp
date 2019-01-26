/*
 * ISpiRegDev.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_SYSTEM_ISPIREGDEV_HPP_
#define LIB_SYSTEM_ISPIREGDEV_HPP_

#include <stdint.h>

class ISpiRegDev
{
 public:
	virtual void Init() = 0;
	virtual void Reset() = 0;
	virtual void writeReg8(uint8_t address, uint8_t value) = 0;
	virtual uint8_t readReg8(uint8_t address) = 0;
};

#endif /* LIB_SYSTEM_ISPIREGDEV_HPP_ */

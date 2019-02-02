/*
 * IInputPin.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_SYSTEM_IINPUTPIN_HPP_
#define LIB_SYSTEM_IINPUTPIN_HPP_

class IInputPin
{
public:
	virtual bool read() = 0;
};

#endif /* LIB_SYSTEM_IINPUTPIN_HPP_ */

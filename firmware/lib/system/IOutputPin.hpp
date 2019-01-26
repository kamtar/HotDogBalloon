/*
 * IOutputPin.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_SYSTEM_IOUTPUTPIN_HPP_
#define LIB_SYSTEM_IOUTPUTPIN_HPP_

class IOutputPin
{
public:
	virtual void set() = 0;
	virtual void clear() = 0;
};

#endif /* LIB_SYSTEM_IOUTPUTPIN_HPP_ */

/*
 * DataRef.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_UTILS_DATAREF_HPP_
#define LIB_UTILS_DATAREF_HPP_
#include <stdint.h>

struct DataRef
{
	uint8_t* data;
	size_t size;
};

#endif /* LIB_UTILS_DATAREF_HPP_ */

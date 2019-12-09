/*
 * DataRef_hpp.h
 *
 * Created: 18/01/2019 20:36:20
 *  Author: Kamtar
 */ 


#ifndef DATAREF_HPP_H_
#define DATAREF_HPP_H_
#include <stdint.h>
#include <cstddef>

struct DataRef
{
	uint8_t* data;
	size_t size;
};



#endif /* DATAREF_HPP_H_ */
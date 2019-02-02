/*
 * ISpi.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_COMMUNICATION_ISPI_HPP_
#define LIB_COMMUNICATION_ISPI_HPP_

#include <DataRef.hpp>

 class ISpi
 {
 public:
	virtual void Init()=0;
	virtual void Write(SimpleDataRef data)=0;
	virtual void Read(SimpleDataRef data_rq, SimpleDataRef data_out)=0;
 };

#endif /* LIB_COMMUNICATION_ISPI_HPP_ */

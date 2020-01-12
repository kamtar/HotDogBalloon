/*
 * ISpi_hpp.h
 *
 * Created: 18/01/2019 20:33:10
 *  Author: Kamtar
 */ 
 #ifndef ISPI_HPP_H_
 #define ISPI_HPP_H_
 #include "DataRef.hpp"

 class ISpi
 {
 public:
	virtual void Init()=0;
	virtual void Write(DataRef data)=0;
	virtual void Read(DataRef data_rq, DataRef data_out)=0;
	virtual void WriteByte(uint8_t byte)=0;
	virtual uint8_t ReadByte()=0;
	virtual void select()=0;
	virtual void unselect()=0;

	ISpi() {};
	ISpi( const ISpi& ) = delete; // non construction-copyable
	ISpi& operator=( const ISpi& ) = delete; // non copyable
 };

 #endif /* ISPI_HPP_H_ */

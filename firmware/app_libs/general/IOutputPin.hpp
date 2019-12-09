/*
 * IOutputPin_hpp.h
 *
 * Created: 19/01/2019 02:03:18
 *  Author: Kamtar
 */ 


#ifndef IOUTPUTPIN_HPP_H_
#define IOUTPUTPIN_HPP_H_

class IOutputPin
{
public:
	virtual void set() = 0;
	virtual void clear() = 0;

	// non-copyable
	IOutputPin() {};
	IOutputPin( const IOutputPin& ) = delete; 
	IOutputPin& operator=( const IOutputPin& ) = delete; 
};



#endif /* IOUTPUTPIN_HPP_H_ */
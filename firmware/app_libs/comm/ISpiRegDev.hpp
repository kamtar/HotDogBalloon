/*
 * ISpiDev_hpp.h
 *
 * Created: 18/01/2019 20:23:10
 *  Author: Kamtar
 */ 

 #ifndef ISPIREGDEV_HPP_
 #define ISPIREGDEV_HPP_
 #include <stdint.h>

 class ISpiRegDev
 {
 public:
	virtual void Init() = 0;
	virtual void Reset() = 0;
	virtual void writeReg8(uint8_t address, uint8_t value) = 0;
	virtual uint8_t readReg8(uint8_t address) = 0;

	// non-copyable
	ISpiRegDev() {};
	ISpiRegDev( const ISpiRegDev& ) = delete; 
	ISpiRegDev& operator=( const ISpiRegDev& ) = delete; 
 };

 #endif /* ISPIREGDEV_HPP_ */
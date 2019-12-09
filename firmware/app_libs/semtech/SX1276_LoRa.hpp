/*
 * SX1276_LoRa_hpp.h
 *
 * Created: 18/01/2019 22:02:14
 *  Author: Kamtar
 */ 


#ifndef SX1276_LORA_HPP_H_
#define SX1276_LORA_HPP_H_
#include "ISpiRegDev.hpp"
#include "sx1276_regs.h"
#include "DataRef.hpp"

class Sx1276_Lora
{
public:
	static constexpr uint8_t SxVersion = 0x12; //Sx1276

	Sx1276_Lora(ISpiRegDev& dev);

	bool InitLoraRadio(uint64_t frq);
	void setFrequency(uint64_t frq);
	void setTxPower(int level);
	bool txActive();
	bool sendPacket(const DataRef data);
	int parsePacket(DataRef& packet);
	inline void sleep() { m_dev.writeReg8(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP); };
	inline void idle()  { m_dev.writeReg8(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY); };

	Sx1276_Lora( const Sx1276_Lora& ) = delete; // non construction-copyable
	Sx1276_Lora& operator=( const Sx1276_Lora& ) = delete; // non copyable

private:
	void setOCP(uint8_t mA);
	void setLdoFlag();

	ISpiRegDev& m_dev;
	uint32_t _packetIndex;
};



#endif /* SX1276_LORA_HPP_H_ */

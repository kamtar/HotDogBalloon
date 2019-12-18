/*
 * LoRaWANPacket_hpp.h
 *
 * Created: 19/01/2019 23:54:02
 *  Author: Kamtar
 */ 


#ifndef LORAWANPACKET_HPP_H_
#define LORAWANPACKET_HPP_H_

#include <DataRef.hpp>

class LoRaWANPacketGen
{
public:

	LoRaWANPacketGen(const uint8_t* networkKey, const uint8_t* appKey, uint32_t device_addr);

	bool Gen_PacketDataUp(bool confirmed, const DataRef data_in, DataRef& out_buffer);

private:
	
	void encrypt_data(const DataRef data_in);

	const uint8_t* m_networkKey; 
	const uint8_t* m_appKey;
	uint32_t device_addr;

	uint8_t output_packet_buffer[150];
	uint8_t packet_len;

	uint32_t frame_counter;

	enum MACMessageType
	{
		join_rq = 0,
		join_accept = 1,
		unconfirmed_data_up = 2,
		unconfirmed_data_down = 3,
		confirmed_data_up = 4,
		confirmed_data_down = 5,
		rejoin_rq = 6,
		proprietary = 7
	};

	struct __attribute__((packed))  MacHeader 
	{
		uint8_t major : 2,
				rfu : 3,
				m_type: 3; 	
	};

	struct __attribute__((packed))  FCtrlUp
	{
		uint8_t FOptsLen : 4,
			ClassB : 1,
			ACK: 1,
			ADRACKReq: 1,
			ADR : 1;	
	};

	 struct __attribute__((packed)) FrameHeader_NoFOpts
	{
		uint32_t dev_addr;
		FCtrlUp frame_control;
		uint16_t frame_counter;
		// - 0-15 bytes for FOpts
	};

	 struct PHYPayload
	{
		MacHeader mac_header;
		FrameHeader_NoFOpts header;
		uint8_t port;
		DataRef encrypted_frame_data; 
		uint32_t MIC;
	};
};



#endif /* LORAWANPACKET_HPP_H_ */

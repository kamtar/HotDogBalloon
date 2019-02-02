/*
 * LoRaWANPacketGen.hpp
 *
 *  Created on: 2 Feb 2019
 *      Author: Kamtar
 */

#ifndef NETWORK_LORAWANSTACK_HPP_
#define NETWORK_LORAWANSTACK_HPP_

#include <DataRef.hpp>
#include <ICryptoContext.hpp>


class LoRaWANStack
{
public:

	LoRaWANStack(ICryptoContext& network_crypto, ICryptoContext& app_crypto, const uint32_t device_addr, IDataContainer& packet_buffer);

	bool Gen_PacketDataUp(bool confirmed, const SimpleDataRef data_in, SimpleDataRef& out_buffer, uint32_t frame_number);

private:

	ICryptoContext& m_network_crypto;
	ICryptoContext& m_app_crypto;

	const uint32_t m_device_addr;

	IDataContainer& m_output_packet_buffer;

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
		SimpleDataRef encrypted_frame_data;
		uint32_t MIC;
	};
};


#endif /* NETWORK_LORAWANSTACK_HPP_ */

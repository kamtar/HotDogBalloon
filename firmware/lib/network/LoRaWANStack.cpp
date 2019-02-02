/*
 * LoRaWANPacketGen.cpp
 *
 *  Created on: 2 Feb 2019
 *      Author: Kamtar
 */

#include "LoRaWANStack.hpp"

#include "string.h"
#include <stdint.h>
#include <IcryptoContext.hpp>
#include <math.h>

 LoRaWANStack::LoRaWANStack(ICryptoContext& network_crypto, ICryptoContext& app_crypto, const uint32_t device_addr, IDataContainer& packet_buffer)
 	 : m_network_crypto(network_crypto),
	   m_app_crypto(app_crypto),
	   m_device_addr(device_addr),
	   m_output_packet_buffer(packet_buffer)
{

}

 bool LoRaWANStack::Gen_PacketDataUp(bool confirmed, const SimpleDataRef data_in, SimpleDataRef& out_buffer, uint32_t frame_number)
 {
	PHYPayload payload;

	uint8_t crypto_block[16];
	uint8_t out_enc_buff[16];

	uint32_t mic = 0;
	m_output_packet_buffer.Write(SimpleDataRef{crypto_block, sizeof(crypto_block)}); //offset for crytpto block

	payload.mac_header.rfu = 0;
	payload.mac_header.major = 0;

	if(confirmed)
		payload.mac_header.m_type = MACMessageType::confirmed_data_up;
	else
		payload.mac_header.m_type = MACMessageType::unconfirmed_data_up;

	m_output_packet_buffer.Write( SimpleDataRef{(uint8_t*)&payload.mac_header, sizeof(payload.mac_header)});

	payload.header.dev_addr = m_device_addr;
	memset(&payload.header.frame_control,0,sizeof(payload.header.frame_control));
	payload.header.frame_counter = frame_number;

	m_output_packet_buffer.Write( SimpleDataRef{(uint8_t*)&payload.header, sizeof(payload.header)});

	payload.port = 1;
	m_output_packet_buffer.WriteByte(payload.port);//port 1

	crypto_block[0] = 0x01;
	crypto_block[1] = 0x00;
	crypto_block[2] = 0x00;
	crypto_block[3] = 0x00;
	crypto_block[4] = 0x00;
	crypto_block[5] = 0x00;//dir

	crypto_block[6] = (uint8_t)(m_device_addr>>0);
	crypto_block[7] = (uint8_t)(m_device_addr>>8);
	crypto_block[8] = (uint8_t)(m_device_addr>>16);
	crypto_block[9] = (uint8_t)(m_device_addr>>24);

	crypto_block[10] = (uint8_t)(frame_number>>0);
	crypto_block[11] = (uint8_t)(frame_number>>8);
	crypto_block[12] = (uint8_t)(frame_number>>16);
	crypto_block[13] = (uint8_t)(frame_number>>24);
	crypto_block[14] = 0x00;

	uint8_t len = ceil(((double)data_in.size)/16.00);

	uint8_t data_i = 0;
	for(uint8_t i=1; i<=len; i++)
	{
		crypto_block[15] = i;
		m_app_crypto.Ecrypt(SimpleDataRef{crypto_block, 16}, SimpleDataRef{out_enc_buff, 16});

		for(uint8_t e=0; (e<16 && data_i < data_in.size); e++)
		{
			m_output_packet_buffer.WriteByte(data_in.data[data_i] ^ out_enc_buff[e]);
			data_i++;
		}
	}

	//crypto block for MIC (message integrity code)
	crypto_block[0] = 0x49;
	crypto_block[1] = 0x00;
	crypto_block[2] = 0x00;
	crypto_block[3] = 0x00;
	crypto_block[4] = 0x00;
	crypto_block[5] = 0x00;//dir

	crypto_block[6] = (uint8_t)(m_device_addr>>0);
	crypto_block[7] = (uint8_t)(m_device_addr>>8);
	crypto_block[8] = (uint8_t)(m_device_addr>>16);
	crypto_block[9] = (uint8_t)(m_device_addr>>24);

	crypto_block[10] = (uint8_t)(frame_number>>0);
	crypto_block[11] = (uint8_t)(frame_number>>8);
	crypto_block[12] = (uint8_t)(frame_number>>16);
	crypto_block[13] = (uint8_t)(frame_number>>24);
	crypto_block[14] = 0x00;
	crypto_block[15] = m_output_packet_buffer.Len()-16;

	m_output_packet_buffer.Write( SimpleDataRef{crypto_block, sizeof(crypto_block)}, 0);

	uint8_t mic_buff[16];
	//generate message integrity code
	//aes_cmac((unsigned char*)output_packet_buffer, packet_len, (unsigned char*)mic_buff, (unsigned char*)m_networkKey);
	mic = mic_buff[0] | (mic_buff[1]<<8) | (mic_buff[2]<<16) | (mic_buff[3]<<24);

	m_output_packet_buffer.Write( SimpleDataRef{(uint8_t*)&mic, sizeof(mic)});

	out_buffer = m_output_packet_buffer.GetDataRef(16); //offset because of MIC cryptoblock

	return true;
}



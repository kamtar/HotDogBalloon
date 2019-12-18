/*
 * LoRaWANPacket_cpp.c
 *
 * Created: 19/01/2019 23:54:14
 *  Author: Kamtar
 */ 
 #include "LoRaWANPacketGen.hpp"
 #include <string.h>
 #include <stdint.h>
 #include <aes/aes128.h>
 #include <math.h>
 #include <aes/cmac.h>

 LoRaWANPacketGen::LoRaWANPacketGen(const uint8_t* networkKey, const uint8_t* appKey, uint32_t device_addr) : m_networkKey(networkKey), m_appKey(appKey), device_addr(device_addr)
 {
	packet_len = 0;
	frame_counter = 0;
 }

 bool LoRaWANPacketGen::Gen_PacketDataUp(bool confirmed, const DataRef data_in, DataRef& out_buffer)
 {
	PHYPayload payload;

	uint8_t crypto_block[16];
	uint8_t out_enc_buff[16];

	uint32_t mic = 0;
	packet_len = 16;//offset for crytpto block
	frame_counter++;
	
	payload.mac_header.rfu = 0;
	payload.mac_header.major = 0;

	if(confirmed)
		payload.mac_header.m_type = MACMessageType::confirmed_data_up;
	else
		payload.mac_header.m_type = MACMessageType::unconfirmed_data_up;

	memcpy(&output_packet_buffer[packet_len], &payload.mac_header, sizeof(payload.mac_header));
	packet_len += sizeof(payload.mac_header);

	payload.header.dev_addr = device_addr;
	memset(&payload.header.frame_control,0,sizeof(payload.header.frame_control));
	payload.header.frame_counter = frame_counter;

	memcpy(&output_packet_buffer[packet_len], &payload.header, sizeof(payload.header));
	packet_len += sizeof(payload.header);

	payload.port = 1;
	output_packet_buffer[packet_len] = 1;//port
	packet_len++;

	crypto_block[0] = 0x01;
	crypto_block[1] = 0x00;
	crypto_block[2] = 0x00;
	crypto_block[3] = 0x00;
	crypto_block[4] = 0x00;
	crypto_block[5] = 0x00;//dir

	crypto_block[6] = (uint8_t)(device_addr>>0);
	crypto_block[7] = (uint8_t)(device_addr>>8);
	crypto_block[8] = (uint8_t)(device_addr>>16);
	crypto_block[9] = (uint8_t)(device_addr>>24);

	crypto_block[10] = (uint8_t)(frame_counter>>0);
	crypto_block[11] = (uint8_t)(frame_counter>>8);
	crypto_block[12] = (uint8_t)(frame_counter>>16);
	crypto_block[13] = (uint8_t)(frame_counter>>24);
	crypto_block[14] = 0x00;

	uint8_t len = data_in.size/16;
	if((data_in.size % 16) != 0)
		len++;

	uint8_t data_i = 0;
	for(uint8_t i=1; i<=len; i++)
	{
		crypto_block[15] = i;
		AES_ECB_encrypt(crypto_block,m_appKey,out_enc_buff,16);

		for(uint8_t e=0; (e<16 && data_i < data_in.size); e++)
		{
			output_packet_buffer[packet_len] = data_in.data[data_i] ^ out_enc_buff[e];
			packet_len++;
			data_i++;
		}
	}

	//crypto block for MIC (message integrity code)
	output_packet_buffer[0] = 0x49;
	output_packet_buffer[1] = 0x00;
	output_packet_buffer[2] = 0x00;
	output_packet_buffer[3] = 0x00;
	output_packet_buffer[4] = 0x00;
	output_packet_buffer[5] = 0x00;//dir

	output_packet_buffer[6] = (uint8_t)(device_addr>>0);
	output_packet_buffer[7] = (uint8_t)(device_addr>>8);
	output_packet_buffer[8] = (uint8_t)(device_addr>>16);
	output_packet_buffer[9] = (uint8_t)(device_addr>>24);

	output_packet_buffer[10] = (uint8_t)(frame_counter>>0);
	output_packet_buffer[11] = (uint8_t)(frame_counter>>8);
	output_packet_buffer[12] = (uint8_t)(frame_counter>>16);
	output_packet_buffer[13] = (uint8_t)(frame_counter>>24);
	output_packet_buffer[14] = 0x00;
	output_packet_buffer[15] = packet_len-16;

	uint8_t mic_buff[16];

	//generate message integrity code
	aes_cmac((unsigned char*)output_packet_buffer, packet_len, (unsigned char*)mic_buff, (unsigned char*)m_networkKey);
	mic = mic_buff[0] | (mic_buff[1]<<8) | (mic_buff[2]<<16) | (mic_buff[3]<<24);

	memcpy(&output_packet_buffer[packet_len], (void*)&mic, sizeof(mic));
	packet_len += sizeof(mic);

	out_buffer.data = &output_packet_buffer[16];//offset because of MIC cryptoblock
	out_buffer.size = packet_len-16;

	return true;
}

 
 void LoRaWANPacketGen::encrypt_data(const DataRef data_in)
 {

 }

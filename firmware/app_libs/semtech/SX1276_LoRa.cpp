/*
 * SX1276_LoRa_cpp.c
 *
 * Created: 18/01/2019 22:02:23
 *  Author: Kamtar
 */ 
 #include "sx1276_regs.h"
 #include "SX1276_LoRa.hpp"
 #include <main.h>

 Sx1276_Lora::Sx1276_Lora(ISpiRegDev& dev) : m_dev(dev)
 {
 
 }

 bool Sx1276_Lora::InitLoraRadio(uint64_t frq)
 {	
	m_dev.Init();

	uint8_t version = m_dev.readReg8(REG_VERSION);
	if (version != SxVersion)
		return false;

	// put in sleep mode
	sleep();

	// set frequency
	setFrequency(frq);

	// set base addresses
	m_dev.writeReg8(REG_FIFO_TX_BASE_ADDR, 0);
	m_dev.writeReg8(REG_FIFO_RX_BASE_ADDR, 0);

	// set LNA boost
	//m_dev.writeReg8(REG_LNA,  m_dev.readReg8(REG_LNA) | 0x03);

	// set auto AGC
	m_dev.writeReg8(REG_MODEM_CONFIG_3, 0x04);
	//m_dev.writeReg8(REG_MODEM_CONFIG_1, m_dev.readReg8(REG_MODEM_CONFIG_1) | 0x01);
	m_dev.writeReg8(REG_MODEM_CONFIG_1,  m_dev.readReg8(REG_MODEM_CONFIG_1) & 0xfe); //explicit header mode
	m_dev.writeReg8(REG_MODEM_CONFIG_2, m_dev.readReg8(REG_MODEM_CONFIG_2) | 0x04); //enable crc
	m_dev.writeReg8(REG_SYNC_WORD, 0x34);//sync word
	m_dev.writeReg8(REG_PREAMBLE_MSB, 0); //preamble
	m_dev.writeReg8(REG_PREAMBLE_LSB, 4);
	m_dev.writeReg8(REG_MODEM_CONFIG_1, (m_dev.readReg8(REG_MODEM_CONFIG_1) & 0x0f) | (7 << 4)); //BW125khz

	m_dev.writeReg8(REG_DETECTION_OPTIMIZE, 0xc3);
	m_dev.writeReg8(REG_DETECTION_THRESHOLD, 0x0a);

	uint8_t sf = 12;
	m_dev.writeReg8(REG_MODEM_CONFIG_2, (m_dev.readReg8(REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));

	// set output power to 17 dBm
	setTxPower(14);
	setLdoFlag();
	// put in standby mode
	idle();
	sleep();

	return true;
 }

 void Sx1276_Lora::setFrequency(uint64_t frq)
 {
	 uint64_t frf = (frq << 19) / 32000000;

	 m_dev.writeReg8(REG_FRF_MSB, static_cast<uint8_t>(frf >> 16));
	 m_dev.writeReg8(REG_FRF_MID, static_cast<uint8_t>(frf >> 8));
	 m_dev.writeReg8(REG_FRF_LSB, static_cast<uint8_t>(frf >> 0));
 }

 
 void Sx1276_Lora::setTxPower(int level)
 {

	 // PA BOOST
	 m_dev.writeReg8(REG_PA_CONFIG, 0x70 | level);

	 // PA BOOST
	 if (level > 17) {
		 if (level > 20) {
			 level = 20;
		 }

		 // subtract 3 from level, so 18 - 20 maps to 15 - 17
		 level -= 3;

		 // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
		 m_dev.writeReg8(REG_PA_DAC, 0x87);
		 setOCP(140);
		 } else {
		 if (level < 2) {
			 level = 2;
		 }
		 //Default value PA_HF/LF or +17dBm
		 m_dev.writeReg8(REG_PA_DAC, 0x84);
		 setOCP(100);
	 }

	 if(level > 14)
		 m_dev.writeReg8(REG_PA_CONFIG, PA_BOOST | 0x70 | (level - 2));


 }

void Sx1276_Lora::setOCP(uint8_t mA)
 {
	 uint8_t ocpTrim = 27;

	 if (mA <= 120) {
		 ocpTrim = (mA - 45) / 5;
		 } else if (mA <=240) {
		 ocpTrim = (mA + 30) / 10;
	 }

	m_dev.writeReg8(REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void Sx1276_Lora::setLdoFlag()
{
	// Section 4.1.1.5
	long symbolDuration = 1000 / ( 125000 / (1L << (m_dev.readReg8(REG_MODEM_CONFIG_2) >> 4)) ) ;

	uint8_t config3 = m_dev.readReg8(REG_MODEM_CONFIG_3);

	if(symbolDuration > 16)
		config3 |= 0x0C;
	else
		config3 &= ~0x0C;

	m_dev.writeReg8(REG_MODEM_CONFIG_3, config3);
}

bool Sx1276_Lora::txActive()
{
	if ((m_dev.readReg8(REG_OP_MODE) & MODE_TX) == MODE_TX) {
		return true;
	}

	if (m_dev.readReg8(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) {
		// clear IRQ's
		m_dev.writeReg8(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
	}

	return false;
}

bool Sx1276_Lora::sendPacket(const DataRef data)
{
	if (txActive())
		return false;

	// put in standby mode
	idle();
	m_dev.writeReg8(REG_INVERTIQ,  0x27);
	m_dev.writeReg8(REG_INVERTIQ2, 0x1d);

	// reset FIFO address and paload length
	m_dev.writeReg8(REG_FIFO_ADDR_PTR, 0);
	m_dev.writeReg8(REG_PAYLOAD_LENGTH, 0);

	// check size
	if ((data.size) > 150) {
		return false;
	}

	// write data
	for (size_t i = 0; i < data.size; i++) {
		m_dev.writeReg8(REG_FIFO, data.data[i]);
	}

	// update length
	m_dev.writeReg8(REG_PAYLOAD_LENGTH, data.size);
	volatile uint8_t currentLength = m_dev.readReg8(REG_PAYLOAD_LENGTH);

	m_dev.writeReg8(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
	while ((m_dev.readReg8(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0);
	m_dev.writeReg8(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
	m_dev.writeReg8(REG_INVERTIQ,  0x66);
	m_dev.writeReg8(REG_INVERTIQ2, 0x19);
	return true;

}

int Sx1276_Lora::parsePacket(DataRef& packet)
{
  int packetLength = 0;
  int irqFlags = m_dev.readReg8(REG_IRQ_FLAGS);
/*
  if (size > 0) {
    implicitHeaderMode();

    writeRegister(REG_PAYLOAD_LENGTH, size & 0xff);
  } else {
    explicitHeaderMode();
  }*/

  // clear IRQ's
  m_dev.writeReg8(REG_IRQ_FLAGS, irqFlags);

  if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {
    // received a packet
    _packetIndex = 0;


      packetLength = m_dev.readReg8(REG_RX_NB_BYTES);


    // set FIFO address to current RX address
    m_dev.writeReg8(REG_FIFO_ADDR_PTR, m_dev.readReg8(REG_FIFO_RX_CURRENT_ADDR));

    // put in standby mode
    idle();
  } else if (m_dev.readReg8(REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE)) {
    // not currently in RX mode

    // reset FIFO address
	  m_dev.writeReg8(REG_FIFO_ADDR_PTR, 0);

    // put in single RX mode
	  m_dev.writeReg8(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
  }

  if(packet.size < packetLength || packetLength == 0)
	  return 0;

  packet.size = packetLength;
  for(int i=0;i<packetLength;i++)
	  packet.data[i] = m_dev.readReg8(REG_FIFO);

  return packetLength;
}

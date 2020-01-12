/*
 * AT25SF041.cpp
 *
 *  Created on: 12. 1. 2020
 *      Author: Jenda
 */

#include "AT25SF041.h"

#include <STM32Spi.hpp>


bool AT25SF041::init()
{
	m_size = 0x80000;
	m_page = 0x1000;

	// Identifikace
	uint32_t id = 0;

	m_spi.select();
	m_spi.WriteByte(0x9F);
	id |= ((uint32_t)m_spi.ReadByte()) << 16;
	id |= ((uint32_t)m_spi.ReadByte()) << 8;
	id |= ((uint32_t)m_spi.ReadByte()) << 0;
	m_spi.unselect();

	if(id != 0x1f8401)
		return false;

	// Unlock
	m_spi.select();
	m_spi.WriteByte((uint8_t)0x06);
	m_spi.unselect();

	m_spi.select();
	m_spi.WriteByte((uint8_t)0x01);
	m_spi.WriteByte(0);
	m_spi.unselect();

	while(is_busy()) {}

	return true;
}

bool AT25SF041::is_busy()
{
	bool busy;

	m_spi.unselect();
	m_spi.select();
	m_spi.WriteByte((uint8_t)0x05);
	busy = !!(m_spi.ReadByte() & 1);
	m_spi.unselect();

	return busy;
}

void AT25SF041::seq_read_start(uint32_t addr)
{
	m_read_addr = addr;

	m_spi.unselect();
	m_spi.select();
	m_spi.WriteByte((uint8_t)0x03);
	m_spi.WriteByte(addr >> 16);
	m_spi.WriteByte(addr >> 8);
	m_spi.WriteByte(addr);
}

uint8_t AT25SF041::seq_read()
{
	if (++m_read_addr >= m_size)
		m_read_addr = 0;
	return m_spi.ReadByte();
}

void AT25SF041::seq_read_stop()
{
	m_spi.unselect();
}

void AT25SF041::seq_write_start(uint32_t addr)
{
	m_write_addr = addr;

	m_spi.unselect();
	m_spi.select();
	m_spi.WriteByte((uint8_t)0x06);
	m_spi.unselect();

	m_spi.select();
	m_spi.WriteByte((uint8_t)0x02);
	m_spi.WriteByte(addr >> 16);
	m_spi.WriteByte(addr >> 8);
	m_spi.WriteByte(addr);
}

void AT25SF041::seq_write(uint8_t data)
{
	if(m_write_addr >= m_size)
		return;

	m_spi.WriteByte(data);
	++m_write_addr;

	if (m_write_addr % m_page == 0)
		return seq_write_stop();
}

void AT25SF041::seq_write_stop()
{
	m_spi.unselect();
}

bool AT25SF041::erase_page(uint32_t addr)
{
	if(addr & 0x0FFF)
		return false;

	m_spi.unselect();
	m_spi.select();
	m_spi.WriteByte((uint8_t)0x06);
	m_spi.unselect();

	m_spi.select();
	m_spi.WriteByte((uint8_t)0x20);
	m_spi.WriteByte(addr >> 16);
	m_spi.WriteByte(addr >> 8);
	m_spi.WriteByte(addr);
	m_spi.unselect();

	return true;
}

void AT25SF041::erase_all()
{
	m_spi.unselect();
	m_spi.select();
	m_spi.WriteByte((uint8_t)0x06);
	m_spi.unselect();

	m_spi.select();
	m_spi.WriteByte((uint8_t)0xC7);
	m_spi.unselect();
}





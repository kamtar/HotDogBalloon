/*
 * AT25SF041.h
 *
 *  Created on: 12. 1. 2020
 *      Author: Jenda
 */

#ifndef AT25SF041_H_
#define AT25SF041_H_

#include <stdint.h>
#include "ISpiRegDev.hpp"
#include "ISpi.hpp"
#include "IOutputPin.hpp"

class AT25SF041
{
public:
	AT25SF041(ISpi& spi);

	bool init();

	bool is_busy();

	void seq_read_start(uint32_t addr);
	uint8_t seq_read();
	void seq_read_stop();

	void seq_write_start(uint32_t addr);
	void seq_write(uint8_t data);
	void seq_write_stop();

	bool erase_page(uint32_t addr);
	void erase_all();

private:
	ISpi &m_spi;
	uint32_t	m_size;
	uint32_t	m_page;

	uint32_t	m_read_addr;
	uint32_t	m_write_addr;
};

inline AT25SF041::AT25SF041(ISpi& spi) : m_spi(spi) {}

#endif /* AT25SF041_H_ */

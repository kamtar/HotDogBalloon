
#include "main.hpp"

#include "Gps.h"
#include "Parsing.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "debug.h"
#include "uart.h"

// CMD
const uint8_t PMTK_TEST[] 					= "$PMTK000*32\r\n";
const uint8_t PMTK_CMD_FULL_COLD_START[] 	= "$PMTK104*37\r\n";
const uint8_t PMTK_CMD_HOT_START[]			= "$PMTK101*32\r\n";
const uint8_t PMTK_API_SET_FIX_CTL[] 		= "$PMTK300,1000,0,0,0,0*1C\r\n";
const uint8_t PMTK_API_SET_NMEA_OUTPUT[] 	= "$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,1,0*2D\r\n";
const uint8_t PMTK_CMD_STANDBY_MODE[]		= "$PMTK161,0*28\r\n";

// MSG
const uint8_t PMTK_SYS_MSG[] 	= "$PMTK010";
const uint8_t PMTK_TXT_MSG[] 	= "$PMTK011";
const uint8_t GGA[] 			= "$GPGGA";
const uint8_t GLL[]				= "$GPGLL";


// ----------------------------------------------------------------------------
void Gps::pmtk_test()
{
	send_cmd((uint8_t *)PMTK_TEST, (size_t)sizeof(PMTK_TEST));
}

// ----------------------------------------------------------------------------
void Gps::cold_start()
{
	send_cmd((uint8_t *)PMTK_CMD_FULL_COLD_START, (size_t)sizeof(PMTK_CMD_FULL_COLD_START));
}

// ----------------------------------------------------------------------------
void Gps::hot_start()
{
	send_cmd((uint8_t *)PMTK_CMD_HOT_START, (size_t)sizeof(PMTK_CMD_HOT_START));
}

// ----------------------------------------------------------------------------
void Gps::set_fix()
{
	send_cmd((uint8_t *)PMTK_API_SET_FIX_CTL, (size_t)sizeof(PMTK_API_SET_FIX_CTL));
}

// ----------------------------------------------------------------------------
void Gps::set_nmea_sentense_output()
{
	send_cmd((uint8_t *)PMTK_API_SET_NMEA_OUTPUT, (size_t)sizeof(PMTK_API_SET_NMEA_OUTPUT));
}

// ----------------------------------------------------------------------------
void Gps::standby_mode()
{
	send_cmd((uint8_t *)PMTK_CMD_STANDBY_MODE, (size_t)sizeof(PMTK_CMD_STANDBY_MODE));
}

// ----------------------------------------------------------------------------
void Gps::task()
{
	while(gps_uart.rx_fifo_size())
	{
		uint8_t byte = gps_uart.read();

		if(m_debug.raw)
		{
			enable_sys_uart(SYS_UART_CMD_TIMEOUT);
			sys_uart.write({ &byte, 1 });
		}

		if(byte == '\r')
			m_cmd.cr = true;
		else if(byte == '\n')
			m_cmd.lf = true;
		else
		{
			if(!fifo_full(m_fifo.incnt, m_fifo.outcnt, m_fifo.buffer_size))
			{
				fifo_in(byte, m_fifo.buffer, m_fifo.incnt, m_fifo.buffer_size);
			}
		}

		if(m_cmd.cr && m_cmd.lf)
		{
			prepare_command();
			parse_command();
			clear_command();
		}
	}
}

// ----------------------------------------------------------------------------
void Gps::prepare_command()
{
	size_t size = cmd_size();

	if(!size)
		return;

	bool new_param = true;

	for(size_t i=0; i<size; i++)
	{
		if(new_param)
		{
			new_param = false;

			m_cmd.params[m_cmd.params_cnt] = &m_fifo.buffer[i];
			m_cmd.params_cnt++;

			if(m_cmd.params_cnt > MAX_PARAMS)
				break;
		}

		if(m_fifo.buffer[i] == ',' || m_fifo.buffer[i] == '*')
		{// Replace , or * with \0

			m_fifo.buffer[i] = '\0';
			new_param = true;
		}
	}
}

// ----------------------------------------------------------------------------
void Gps::parse_command()
{
	if(cmd_size() > 0 && m_cmd.params_cnt > 0)
	{
		if(compare_string(m_cmd.params[0], (uint8_t *)PMTK_SYS_MSG))
		{
			if(m_debug.app)
			{
				debug("sys msg\r\n");
			}
		}
		else if(compare_string(m_cmd.params[0], (uint8_t *)PMTK_TXT_MSG))
		{
			if(m_debug.app)
			{
				debug("txt msg\r\n");
			}
		}
		else if(compare_string(m_cmd.params[0], (uint8_t *)GGA))
		{
			if(m_debug.app)
			{
				debug("GGA data:\r\n");

				debug("UTC time               : %s\r\n", m_cmd.params[1]);
				debug("Latitude               : %s\r\n", m_cmd.params[2]);
				debug("N/S indicator          : %s\r\n", m_cmd.params[3]);
				debug("Longitude              : %s\r\n", m_cmd.params[4]);
				debug("E/W indicator          : %s\r\n", m_cmd.params[5]);
				debug("Position fix indicator : %s\r\n", m_cmd.params[6]);
				debug("Satelites used         : %s\r\n", m_cmd.params[7]);
				debug("HDOP                   : %s\r\n", m_cmd.params[8]);
				debug("MSL Altitude           : %s\r\n", m_cmd.params[9]);
				debug("Units                  : %s\r\n", m_cmd.params[10]);
				debug("Geoid Separation       : %s\r\n", m_cmd.params[11]);
				debug("Units                  : %s\r\n", m_cmd.params[12]);
				debug("Age of Diff. Corr.     : %s\r\n", m_cmd.params[13]);
				debug("Diff. Ref. Station ID  : %s\r\n", m_cmd.params[14]);

				debug("---------------------------------------\r\n");
			}

			st.gps_fixed = (bool)Parsing::parse_uint(m_cmd.params[6]);

			Parsing::decimal d;

			if(!Parsing::string_empty(m_cmd.params[2]))
			{// Latitude

				d = Parsing::parse_decimal(m_cmd.params[2]);

				st.gps_position.latitude[0] = d.num[0];
				st.gps_position.latitude[1] = d.num[1];
			}

			if(!Parsing::string_empty(m_cmd.params[3]))
			{// N/S indicator

				st.gps_position.n_s = m_cmd.params[3][0];
			}

			if(!Parsing::string_empty(m_cmd.params[4]))
			{// Longtitude

				d = Parsing::parse_decimal(m_cmd.params[4]);

				st.gps_position.longitude[0] = d.num[0];
				st.gps_position.longitude[1] = d.num[1];
			}

			if(!Parsing::string_empty(m_cmd.params[5]))
			{// E/W indicator

				st.gps_position.e_w = m_cmd.params[5][0];
			}

			if(!Parsing::string_empty(m_cmd.params[9]))
			{// MSL Altitude

				d = Parsing::parse_decimal(m_cmd.params[9]);

				st.gps_position.altitude[0] = d.num[0];
				st.gps_position.altitude[1] = d.num[1];
			}
		}
		else if(compare_string(m_cmd.params[0], (uint8_t *)GLL))
		{
			if(m_debug.app)
			{
				debug("GLL data:\r\n");

				debug("Latitude               : %s\r\n", m_cmd.params[1]);
				debug("N/S indicator          : %s\r\n", m_cmd.params[2]);
				debug("Longitude              : %s\r\n", m_cmd.params[3]);
				debug("E/W indicator          : %s\r\n", m_cmd.params[4]);
				debug("UTC time               : %s\r\n", m_cmd.params[5]);
				debug("Status                 : %s\r\n", m_cmd.params[6]);
				debug("Mode                   : %s\r\n", m_cmd.params[7]);

				debug("---------------------------------------\r\n");
			}

			st.gps_valid = (m_cmd.params[6][0] == 'A');
		}
	}
}

// ----------------------------------------------------------------------------
void Gps::clear_command()
{
	memset(m_fifo.buffer, 0, m_fifo.buffer_size);
	m_fifo.incnt = 0;
	m_fifo.outcnt = 0;

	for(size_t i=0; i<MAX_PARAMS; i++)
		m_cmd.params[i] = m_fifo.buffer;

	m_cmd.cr = false;
	m_cmd.params_cnt = 0;
}

// ----------------------------------------------------------------------------
size_t Gps::cmd_size()
{
	return fifo_space_occupied(m_fifo.incnt, m_fifo.outcnt, m_fifo.buffer_size);
}

// ----------------------------------------------------------------------------
void Gps::send_cmd(uint8_t * cmd, size_t size)
{
	gps_uart.write({ cmd, size });

	if(m_debug.raw)
	{
		debug("%s", (char *)cmd);
	}
}

// ----------------------------------------------------------------------------
bool Gps::compare_string(uint8_t * s1, uint8_t * s2)
{
	return strcmp((char *)s1, (char *)s2) == 0;
}













#include "Commands.h"
#include "Parsing.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "debug.h"
#include "uart.h"
#include "spi.h"

// ----------------------------------------------------------------------------
void Commands::implicit_lf(bool en)
{
	m_cmd.implicit_lf = en;
	m_cmd.lf = en;
}

// ----------------------------------------------------------------------------
void Commands::task()
{
	while(sys_uart.rx_fifo_size())
	{
		enable_sys_uart(SYS_UART_CMD_TIMEOUT);

		uint8_t byte = sys_uart.read();

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
void Commands::prepare_command()
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

		if(m_fifo.buffer[i] == ',')
		{// Replace , with \0

			m_fifo.buffer[i] = '\0';
			new_param = true;
		}
	}
}

// ----------------------------------------------------------------------------
void Commands::parse_command()
{
	if(cmd_size() > 0)
	{
		uint8_t rq = Parsing::parse_uint(m_cmd.params[0]);

		switch(rq)
		{
		case CMD_READ_STATE:
		{
			read_state();
			break;
		}
		case CMD_READ_EXTFLASH:
		{
			if(m_cmd.params_cnt >= 3)
			{
				read_extflash(Parsing::parse_hex(m_cmd.params[1]), Parsing::parse_uint(m_cmd.params[2]));
			}
			else
			{
				send_error();
			}
			break;
		}
		case CMD_ERASE_EXTFLASH_ALL:
		{
			erase_extflash_all();
			break;
		}
		case CMD_ERASE_EXTFLASH_PAGE:
		{
			if(m_cmd.params_cnt >= 2)
			{
				erase_extflash_page(Parsing::parse_hex(m_cmd.params[1]));
			}
			else
			{
				send_error();
			}
			break;
		}
		case CMD_WRITE_EXTFLASH:
		{
			if(m_cmd.params_cnt >= 3)
			{
				write_extflash(Parsing::parse_hex(m_cmd.params[1]), m_cmd.params[2]);
			}
			else
			{
				send_error();
			}
			break;
		}
		default:
		{
			send_error();
			break;
		}
		}
	}
	else
	{
		send_error();
	}
}

// ----------------------------------------------------------------------------
void Commands::clear_command()
{
	memset(m_fifo.buffer, 0, m_fifo.buffer_size);
	m_fifo.incnt = 0;
	m_fifo.outcnt = 0;

	for(size_t i=0; i<MAX_PARAMS; i++)
		m_cmd.params[i] = m_fifo.buffer;

	m_cmd.cr = false;
	m_cmd.lf = m_cmd.implicit_lf;
	m_cmd.params_cnt = 0;
}

// ----------------------------------------------------------------------------
size_t Commands::cmd_size()
{
	return fifo_space_occupied(m_fifo.incnt, m_fifo.outcnt, m_fifo.buffer_size);
}

// ----------------------------------------------------------------------------
void Commands::read_state()
{
	printf("uptime : %lu\r\n", st.uptime);
}

// ----------------------------------------------------------------------------
void Commands::send_ok()
{
	printf("OK\r\n");
}

// ----------------------------------------------------------------------------
void Commands::send_error()
{
	printf("ERROR\r\n");
}

// ----------------------------------------------------------------------------
void Commands::read_extflash(uint32_t addr, size_t size)
{
	bool was_disabled = enable_spi();

	extflash.seq_read_start(addr);

	for(size_t i=0; i<size; i++)
	{
		printf("%02X", extflash.seq_read());
	}

	extflash.seq_read_stop();

	disable_spi(was_disabled);

	printf("\r\n");
}

// ----------------------------------------------------------------------------
void Commands::erase_extflash_all()
{
	bool was_disabled = enable_spi();

	extflash.erase_all();

	while(extflash.is_busy()) {}

	disable_spi(was_disabled);

	send_ok();
}

// ----------------------------------------------------------------------------
void Commands::erase_extflash_page(uint32_t addr)
{
	bool was_disabled = enable_spi();

	if(!extflash.erase_page(addr))
	{
		disable_spi(was_disabled);

		send_error();
		return;
	}

	while(extflash.is_busy()) {}

	disable_spi(was_disabled);

	send_ok();
}

// ----------------------------------------------------------------------------
void Commands::write_extflash(uint32_t addr, uint8_t * data)
{
	size_t size = Parsing::convert_to_bytes(data);

	if(!size)
	{
		send_error();
		return;
	}

	bool was_disabled = enable_spi();

	extflash.seq_write_start(addr);

	for(size_t i=0; i<size; i++)
	{
		extflash.seq_write(data[i]);
	}

	extflash.seq_write_stop();

	disable_spi(was_disabled);

	send_ok();
}



















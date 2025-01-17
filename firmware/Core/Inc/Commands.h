
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "config.h"

class Commands
{
public:
	Commands(DataRef buf);

	void implicit_lf(bool en);

	void task();

private:
	void prepare_command();
	void parse_command();
	void clear_command();

	void read_state();
	void read_extflash(uint32_t addr, size_t size);
	void erase_extflash_all();
	void erase_extflash_page(uint32_t addr);
	void write_extflash(uint32_t addr, uint8_t * data);

	void send_ok();
	void send_error();

	size_t cmd_size();

	struct
	{
		uint8_t * 	buffer;
		size_t		buffer_size;
		uint32_t	incnt;
		uint32_t	outcnt;

	}m_fifo;

	struct
	{
		bool cr;
		bool lf;
		bool implicit_lf;

		uint8_t * params[MAX_PARAMS];
		uint16_t params_cnt;

	}m_cmd;
};

inline Commands::Commands(DataRef buf) :
	m_fifo { buf.data, buf.size, 0, 0 }
	{clear_command();}

#endif /* COMMANDS_H_ */

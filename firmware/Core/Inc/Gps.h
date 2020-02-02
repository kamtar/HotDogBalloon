
#ifndef GPS_H_
#define GPS_H_

#include "main.hpp"
#include "config.h"

class Gps
{
public:
	Gps(DataRef buf);

	void task();

	void raw_debug(bool en) { m_debug.raw = en; }
	void app_debug(bool en) { m_debug.app = en; }

	void pmtk_test();

	void cold_start();
	void set_fix();
	void set_nmea_sentense_output();

private:
	void prepare_command();
	void parse_command();
	void clear_command();

	size_t cmd_size();

	void send_cmd(uint8_t * cmd, size_t size);

	bool compare_string(uint8_t * s1, uint8_t * s2);

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

		uint8_t * params[MAX_PARAMS];
		uint16_t params_cnt;

	}m_cmd;

	struct
	{
		bool raw;
		bool app;

	}m_debug;
};

inline Gps::Gps(DataRef buf) :
		m_fifo { buf.data, buf.size, 0, 0 }
		{clear_command();}

#endif /* GPS_H_ */

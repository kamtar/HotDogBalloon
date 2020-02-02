
#ifndef BALLOON_H_
#define BALLOON_H_

#include "config.h"

class Balloon
{
public:
	Balloon();

	void init();

	void task();
	void ms_task();
	void sec_task();

	void state_debug(bool en) { m_debug = en; }

	bool busy() { return m_state != IDLE; }

private:
	void set_state(uint8_t value) { m_state = value; };
	void set_next_state(uint8_t value) { m_next_state = value; }
	void set_state_timeout(uint32_t value) { m_state_tmr = value; }
	bool state_timeouted() { return !m_state_tmr; }

	enum states
	{
		INITIAL_GPS_FIX = 0,
		NORMAL_GPS_FIX,
		WAIT_FOR_GPS_FIX,
		IDLE
	};

	uint8_t m_state;
	uint8_t m_next_state;

	volatile uint32_t m_state_tmr;

	bool m_debug;
};

inline Balloon::Balloon() {}


#endif /* BALLOON_H_ */


#include "main.hpp"

#include "Balloon.h"
#include "debug.h"
#include "uart.h"


// ----------------------------------------------------------------------------
void Balloon::init()
{
	m_state = INITIAL_GPS_FIX;
}

// ----------------------------------------------------------------------------
void Balloon::task()
{
	switch(m_state)
	{
	case INITIAL_GPS_FIX:
	{
		if(m_debug)
			debug("INITIAL_GPS_FIX start\r\n");

		enable_gps_uart();

		gps_on.set();
		delay_ms(50);
		gps_on.clear();
		delay_ms(100);

		gps.cold_start();
		gps.set_fix();
		gps.set_nmea_sentense_output();

		st.gps_fixed = false;
		st.gps_valid = false;

		set_state(WAIT_FOR_GPS_FIX);
		set_state_timeout(GPS_FIX_TIMEOUT);

		break;
	}
	case NORMAL_GPS_FIX:
	{
		if(m_debug)
			debug("NORMAL_GPS_FIX start\r\n");

		enable_gps_uart();

		gps_on.set();
		delay_ms(50);
		gps_on.clear();
		delay_ms(100);

		gps.hot_start();
		gps.set_fix();
		gps.set_nmea_sentense_output();

		st.gps_fixed = false;
		st.gps_valid = false;

		set_state(WAIT_FOR_GPS_FIX);
		set_state_timeout(GPS_FIX_TIMEOUT);

		break;
	}
	case WAIT_FOR_GPS_FIX:
	{
		if(state_timeouted() || (st.gps_fixed && st.gps_valid))
		{
			gps_on.set();
			disable_gps_uart(true);

			set_state(IDLE);

			if(state_timeouted())
			{
				if(m_debug)
					debug("WAIT_FOR_GPS_FIX timeout\r\n");

				set_next_state(INITIAL_GPS_FIX);
			}
			else if(st.gps_fixed && st.gps_valid)
			{
				if(m_debug)
					debug("GPS FIX success after %lu s\r\n", GPS_FIX_TIMEOUT - m_state_tmr);

				set_next_state(NORMAL_GPS_FIX);
			}

			set_state_timeout(GPS_FIX_PERIOD);
		}

		break;
	}
	case IDLE:
	{
		if(state_timeouted())
		{
			m_state = m_next_state;
		}

		break;
	}
	default:
		break;
	}
}

// ----------------------------------------------------------------------------
void Balloon::ms_task()
{

}

// ----------------------------------------------------------------------------
void Balloon::sec_task()
{
	if(m_state_tmr)
		m_state_tmr--;

	if(m_debug)
	{
		switch(m_state)
		{
		case WAIT_FOR_GPS_FIX:
		{
			debug("WAIT_FOR_GPS_FIX (%lu)\r\n", m_state_tmr);
			break;
		}
		case IDLE:
		{
			debug("IDLE (%lu)\r\n", m_state_tmr);
			break;
		}
		default:
			break;
		}
	}
}











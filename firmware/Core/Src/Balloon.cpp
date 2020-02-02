
#include "main.hpp"

#include "Balloon.h"
#include "debug.h"
#include "uart.h"
#include "system_init.h"

// ----------------------------------------------------------------------------
void Balloon::init()
{
	m_cfg.measure_cnt = MEASURE_CNT;
	m_cfg.last_gps_fix = false;

	set_state(IDLE);
	set_state_timeout(MEASURE_PERIOD);
}

// ----------------------------------------------------------------------------
void Balloon::task()
{
	switch(m_state)
	{
	case INITIAL_GPS_FIX:
	{
		if(m_cfg.debug)
			debug("INITIAL_GPS_FIX start\r\n");

		enable_gps_uart();

		gps_on.set();
		delay_ms(50);
		gps_on.clear();
		delay_ms(100);

		gps.full_cold_start();
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
		if(m_cfg.debug)
			debug("NORMAL_GPS_FIX start\r\n");

		enable_gps_uart();

		gps_on.set();
		delay_ms(50);
		gps_on.clear();
		delay_ms(100);

		gps.warm_start();
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

			if(state_timeouted())
			{
				if(m_cfg.debug)
					debug("WAIT_FOR_GPS_FIX timeout\r\n");

				m_cfg.last_gps_fix = false;
			}
			else if(st.gps_fixed && st.gps_valid)
			{
				if(m_cfg.debug)
					debug("GPS FIX success after %lu s [%lu.%lu %c %lu.%lu %c %lu.%lu]\r\n",
							GPS_FIX_TIMEOUT - m_state_tmr,
							st.gps_position.latitude[0],
							st.gps_position.latitude[1],
							(char)st.gps_position.n_s,
							st.gps_position.longitude[0],
							st.gps_position.longitude[1],
							(char)st.gps_position.e_w,
							st.gps_position.altitude[0],
							st.gps_position.altitude[1]);

				m_cfg.last_gps_fix = true;
			}

			set_state(LORA_SEND);
		}

		break;
	}
	case LORA_SEND:
	{
		if(m_cfg.debug)
			debug("LORA_SEND\r\n");

		// TODO Lora send

		set_state(LORA_WAIT);

		break;
	}
	case LORA_WAIT:
	{
		if(m_cfg.debug)
			debug("LORA_WAIT\r\n");

		// TODO Lora wait

		set_state(IDLE);
		set_state_timeout(MEASURE_PERIOD);

		break;
	}
	case TEMP_AND_PRESSURE:
	{
		I2C_MUX(true);

		temp_sensor.normal(); // prepnuti do normalu, tim zacne merit

		delay_ms(50); // min. 26 ms trva mereni

		st.temp = temp_sensor.get_temp();

		temp_sensor.shutdown();

		I2C_MUX(false);

		// TODO mereni tlaku

		if(m_cfg.debug)
			debug("TEMP_AND_PRESSURE [%u] : temp : %ld °C\r\n", m_cfg.measure_cnt, st.temp);

		m_cfg.measure_cnt--;

		if(!m_cfg.measure_cnt)
		{// Posledni mereni, urcime polohu

			m_cfg.measure_cnt = MEASURE_CNT;

			if(m_cfg.last_gps_fix)
			{// Posledni urceni polohy bylo uspesne
				set_state(NORMAL_GPS_FIX);
			}
			else
			{// Posledni urceni polohy vytimeoutovalo
				set_state(INITIAL_GPS_FIX);
			}
		}
		else
		{// Neni posledni mereni, odesleme data
			set_state(LORA_SEND);
		}

		break;
	}
	case IDLE:
	{
		if(state_timeouted())
		{
			set_state(TEMP_AND_PRESSURE);
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

	if(m_cfg.debug)
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











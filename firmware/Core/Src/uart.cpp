
#include "uart.h"
#include "main.hpp"

#include "system_init.h"

static bool sys_uart_enabled = false;

// ----------------------------------------------------------------------------
bool enable_sys_uart(uint32_t timeout)
{
	bool was_disabled = !sys_uart_enabled;

	if(!sys_uart_enabled)
	{
		USART1_MUX(true);

		sys_uart.enable();

		sys_uart_enabled = true;
	}

	if(ms_tmr.sys_uart_tmr < timeout)
		ms_tmr.sys_uart_tmr = timeout;

	return was_disabled;
}

// ----------------------------------------------------------------------------
void disable_sys_uart(bool was_disabled)
{
	if(sys_uart_enabled && was_disabled)
	{
		sys_uart.disable();

		USART1_MUX(false);

		sys_uart_enabled = false;
	}
}

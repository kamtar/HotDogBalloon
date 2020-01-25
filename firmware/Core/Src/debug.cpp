
#include "debug.h"
#include "main.h"

#include "uart.h"

#include "STM32FIFOUART.h"

extern STM32_FIFO_UART sys_uart;

// For printf redirect
extern "C" int _write(int file, const char * p_char, int len)
{
	enable_sys_uart(SYS_UART_PRINTF_TIMEOUT);

	sys_uart.write({ (uint8_t *)p_char, (size_t)len });
    return len;
}

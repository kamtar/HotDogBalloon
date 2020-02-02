
#ifndef UART_H_
#define UART_H_

#include <stdint.h>

bool enable_sys_uart(uint32_t timeout);
void disable_sys_uart(bool was_disabled);

bool enable_gps_uart();
void disable_gps_uart(bool was_disabled);

#endif /* UART_H_ */

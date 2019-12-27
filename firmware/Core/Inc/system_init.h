/*
 * system_init.h
 *
 *  Created on: 27. 12. 2019
 *      Author: Jenda
 */

#ifndef INC_SYSTEM_INIT_H_
#define INC_SYSTEM_INIT_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_ADC_Init(void);
void MX_CRC_Init(void);
void MX_RTC_Init(void);
void MX_SPI1_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTEM_INIT_H_ */

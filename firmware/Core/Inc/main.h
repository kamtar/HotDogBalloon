
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l0xx_hal.h"
#include <stdbool.h>

#define delay_ms HAL_Delay

// ----------------------------------------------------------------------------
typedef struct
{
	volatile uint32_t red;

}Led_struct;

// ----------------------------------------------------------------------------
typedef struct
{
	volatile uint32_t ms;

}Ms_tmr_struct;

// ----------------------------------------------------------------------------
typedef struct
{
	volatile uint32_t sec;
	volatile uint32_t measurement;

}Sec_tmr_struct;

// ----------------------------------------------------------------------------
typedef struct
{
	volatile bool sec;
	volatile bool measurement;

}Flag_struct;

extern ADC_HandleTypeDef hadc;
extern CRC_HandleTypeDef hcrc;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern Ms_tmr_struct 	ms_tmr;
extern Sec_tmr_struct 	sec_tmr;
extern Led_struct 		led;
extern Flag_struct		flags;

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);

void systick_handler();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

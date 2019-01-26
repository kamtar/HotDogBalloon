
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define delay_ms(x) HAL_Delay(x)
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#ifdef DEBUG
#define USE_FULL_ASSERT
#endif

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


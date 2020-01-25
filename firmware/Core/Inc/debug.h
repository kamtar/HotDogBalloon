/*
 * debug.h
 *
 *  Created on: 25. 1. 2020
 *      Author: Jenda
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include <stdio.h>
#include "config.h"

#if DEBUG_UART
#define debug(...) do { printf(__VA_ARGS__); }while(0)
#else
#define debug(...)
#endif

#endif /* INC_DEBUG_H_ */

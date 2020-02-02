
#ifndef MAIN_HPP_
#define MAIN_HPP_

#include "main.h"

#include <STM32OutputPin.hpp>
#include <STM32_I2C.hpp>
#include <Bar_MS5637.hpp>
#include <Temp_PCT2075.hpp>
#include <STM32Spi.hpp>
#include <SemtechSpiDev.hpp>
#include <SX1276_LoRa.hpp>
#include <AT25SF041.h>
#include <STM32FIFOUART.h>
#include <LoRaWANPacketGen.hpp>

#include "Commands.h"
#include "Gps.h"

// Pins
extern STM32OutputPin led_gpio;
extern STM32OutputPin gps_on;
extern STM32OutputPin gps_rst;
extern STM32OutputPin lora_select;
extern STM32OutputPin lora_reset;
extern STM32OutputPin extflash_select;

// Peripherals
extern STM32_I2C 		i2c2;
extern STM32Spi 		lora_spi;
extern STM32Spi 		extflash_spi;
extern STM32_FIFO_UART 	sys_uart;
extern STM32_FIFO_UART 	gps_uart;

// Devices
extern Bar_MS5637 		barometer;
extern Temp_PCT2075 	temp_sensor;
extern SemtechSpiDev 	semtech_dev;
extern Sx1276_Lora 		lora_radio;
extern AT25SF041 		extflash;

// Others
extern LoRaWANPacketGen lorawan_packet_gen;

#endif /* MAIN_HPP_ */

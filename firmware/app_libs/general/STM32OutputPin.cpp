/*
 * SamD21OutputPin_cpp.c
 *
 * Created: 19/01/2019 02:15:30
 *  Author: Kamtar*/
#include "STM32OutputPin.hpp"

STM32OutputPin::STM32OutputPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) : m_gpio(GPIOx)
{
	m_pin.Mode = GPIO_MODE_OUTPUT_PP;
	m_pin.Pin = GPIO_Pin;
	m_pin.Pull = 0;
	m_pin.Speed = GPIO_SPEED_FREQ_HIGH;
	m_ready = false;
}

void STM32OutputPin::Init()
{
	HAL_GPIO_Init(m_gpio, &m_pin);
	m_ready = true;
}

 void STM32OutputPin::set()
{ 
	HAL_GPIO_WritePin(m_gpio, m_pin.Pin, GPIO_PinState::GPIO_PIN_SET);
}
  
 void STM32OutputPin::clear()
{ 
	HAL_GPIO_WritePin(m_gpio, m_pin.Pin, GPIO_PinState::GPIO_PIN_RESET);
}

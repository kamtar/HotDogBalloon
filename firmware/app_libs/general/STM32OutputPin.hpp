/*
 * SamD21OutputPin_hpp.h
 *
 * Created: 19/01/2019 02:05:08
 *  Author: Kamtar
 */ 


#ifndef SAMD21OUTPUTPIN_HPP_H_
#define SAMD21OUTPUTPIN_HPP_H_

#include <main.h>
#include "IOutputPin.hpp"


class STM32OutputPin : public IOutputPin
{
public:
	STM32OutputPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);

	void Init();

	 virtual void set() override;
	 virtual void clear() override;
	
private:
	
	GPIO_InitTypeDef m_pin;
	GPIO_TypeDef* m_gpio;
	bool m_ready;
};



#endif /* SAMD21OUTPUTPIN_HPP_H_ */

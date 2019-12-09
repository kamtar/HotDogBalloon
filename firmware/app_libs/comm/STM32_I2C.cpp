/*
 * STM32_I2C.cpp
 *
 *  Created on: 9 Dec 2019
 *      Author: Kamtar
 */

#include "STM32_I2C.hpp"
STM32_I2C::STM32_I2C(I2C_TypeDef* i2c_pointer)
{
	m_i2c.Instance = I2C2;
}

void STM32_I2C::Init()
{


	if(m_is_ready == true)
		return;
	m_i2c.Init.Timing = 0x00707CBB;
	m_i2c.Init.OwnAddress1 = 0;
	m_i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	m_i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	m_i2c.Init.OwnAddress2 = 0;
	m_i2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	m_i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	m_i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&m_i2c) != HAL_OK)
	  {
	    return;
	  }
	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&m_i2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
		  return;
	  }
	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&m_i2c, 0) != HAL_OK)
	  {
		  return;
	  }
	  m_is_ready = true;
}

void STM32_I2C::Write(uint8_t addr, DataRef data)
{
	if(m_is_ready == false)
		return;

	addr = (~0x01) & addr;

	HAL_I2C_Master_Transmit(&m_i2c, addr, data.data, data.size, 10);
}

void STM32_I2C::Read(uint8_t addr, DataRef data_rq, DataRef data_out)
{
	if(m_is_ready == false)
		return;

	HAL_I2C_Master_Transmit(&m_i2c, addr, data_rq.data, data_rq.size, 500000);
	HAL_I2C_Master_Receive(&m_i2c, addr, data_out.data, data_out.size, 50000);

}



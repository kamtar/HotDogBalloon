/*
 * MainApp.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef INCLUDE_MAINAPP_HPP_
#define INCLUDE_MAINAPP_HPP_
#include <ITask.hpp>
#include <stdint.h>

class MainApp : public ITask
{
private:
	static constexpr uint64_t devAddr = 0x2601163D;
	static constexpr uint8_t nwkSKey[16] = {0xBE,0xB8,0xC0,0x7F,0xA0,0x85,0xAA,0x6B,0x6D,0xD5,0xB1,0x46,0xC6,0xC5,0xC1,0x8D}; //"BEB8C07FA085AA6B6DD5B146C6C5C18D";
	static constexpr uint8_t appSKey[16] = {0x2C,0xF1,0xAE,0xA8,0x78,0xF9,0xCB,0x80,0x25,0xA4,0xF7,0x85,0x1A,0xA2,0x4A,0xFF}; //"2CF1AEA878F9CB8025A4F7851AA24AFF";

public:
	MainApp();
	//ITask impl
	virtual bool Init() override;
	virtual void Task() override;
	virtual void ms_tick() override;
	virtual void sec_tick() override;
};



#endif /* INCLUDE_MAINAPP_HPP_ */

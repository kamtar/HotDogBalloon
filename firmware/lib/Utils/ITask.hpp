/*
 * ITask.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_UTILS_ITASK_HPP_
#define LIB_UTILS_ITASK_HPP_
#include <stdint.h>

class ITask
{
	virtual bool Init() = 0;
	virtual void Task() = 0;
	virtual void ms_tick() = 0;
	virtual void sec_tick() = 0;
};




#endif /* LIB_UTILS_ITASK_HPP_ */

/*
 * DataRef.hpp
 *
 *  Created on: 26 Jan 2019
 *      Author: Kamtar
 */

#ifndef LIB_UTILS_DATAREF_HPP_
#define LIB_UTILS_DATAREF_HPP_
#include <stdint.h>
#include <stddef.h>

struct SimpleDataRef
{
	uint8_t* data;
	size_t size;
};

class IDataContainer
{
public:
	virtual bool WriteByte(uint8_t d) = 0;
	virtual const uint8_t ReadByte(size_t pos) = 0;

	virtual bool Write(const SimpleDataRef& data) = 0;
	virtual bool Write(const SimpleDataRef& data, size_t offset) = 0;
	virtual const bool ReadCopy(SimpleDataRef& data, size_t offset = 0) = 0;

	virtual const SimpleDataRef GetDataRef(size_t offset = 0) = 0;

	virtual const size_t Len() = 0;
	virtual void Clear() = 0;

	virtual const size_t MaxSize() = 0;

	IDataContainer( const IDataContainer& ) = delete; // non construction-copyable
	IDataContainer& operator=( const IDataContainer& ) = delete; // non copyable
};

template <const size_t size>
class DataContainer : public IDataContainer
{
public:
	DataContainer();
	virtual bool WriteByte(uint8_t d) override;
	virtual const uint8_t ReadByte(size_t pos) override;

	virtual bool Write(const SimpleDataRef& data) override;
	virtual bool Write(const SimpleDataRef& data, size_t offset) override;
	virtual const bool ReadCopy(SimpleDataRef& data, size_t offset = 0) override;

	virtual const SimpleDataRef GetDataRef(size_t offset = 0) override;

	virtual const size_t Len() override;
	virtual void Clear() override;

	virtual inline const size_t MaxSize() override { return size; } ;

	DataContainer( const DataContainer& ) = delete; // non construction-copyable
	DataContainer& operator=( const DataContainer& ) = delete; // non copyable

private:
	uint8_t m_data[size];
	size_t m_valid_data_num;
};

#endif /* LIB_UTILS_DATAREF_HPP_ */

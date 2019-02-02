/*
 * DataRef.cpp
 *
 *  Created on: 2 Feb 2019
 *      Author: Kamtar
 */
#include <DataRef.hpp>
#include <cassert>

template <const size_t size>
DataContainer<size>::DataContainer()
{
	m_valid_data_num = 0;
}

template <const size_t size>
bool DataContainer<size>::WriteByte(uint8_t d)
{
	if(m_valid_data_num >= size)
	{
		assert(false);//bad usage
		return false;
	}

	m_data[m_valid_data_num] = d;
	m_valid_data_num++;
	return true;
}

template <const size_t size>
const uint8_t DataContainer<size>::ReadByte(size_t pos)
{
	if(pos >= size)
		return 0;

	return m_data[pos];
}

template <const size_t size>
bool DataContainer<size>::Write(const SimpleDataRef& data)
{
	if((m_valid_data_num + data.size) >= size)
	{
		assert(false);//bad usage
		return false;
	}

	for(int i=0; (i < data.size) && ((m_valid_data_num) < size); i++)
	{
		m_data[m_valid_data_num] = data.data[i];
		m_valid_data_num++;
	}

	return true;
}

template <const size_t size>
bool DataContainer<size>::Write(const SimpleDataRef& data, size_t offset)
{
	if(offset + data.size >= size)
	{
		assert(false);//bad usage
		return false;
	}

	if(offset + data.size > m_valid_data_num)
		m_valid_data_num = offset + data.size;

	for(int i=0; (i < data.size) && ((i+offset) < size); i++)
	{
		m_data[i+offset] = data.data[i];
	}

	return true;
}

template <const size_t size>
const bool DataContainer<size>::ReadCopy(SimpleDataRef& data, size_t offset)
{
	if(offset+ data.size >= size)
			return false;

	for(int i=0; (i < data.size) && ((i + offset) < size); i++)
	{
		data.data[i] = m_data[i+offset];
	}

	return true;
}

template <const size_t size>
const SimpleDataRef DataContainer<size>::GetDataRef(size_t offset)
{
  return SimpleDataRef{ &m_data[offset], m_valid_data_num-offset};
}

template <const size_t size>
void DataContainer<size>::Clear()
{
	m_valid_data_num = 0;
}

template <const size_t size>
const size_t DataContainer<size>::Len()
{
	return m_valid_data_num;
}


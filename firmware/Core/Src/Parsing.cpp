
#include "Parsing.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>


// ----------------------------------------------------------------------------
uint32_t Parsing::parse_uint(uint8_t * src)
{
	return strtoul((const char *)src, NULL, 10);
}

// ----------------------------------------------------------------------------
int32_t Parsing::parse_int(uint8_t * src)
{
	return strtoul((const char *)src, NULL, 10);
}

// ----------------------------------------------------------------------------
uint32_t Parsing::parse_hex(uint8_t * src)
{
	return strtoul((const char *)src, NULL, 16);
}

// ----------------------------------------------------------------------------
size_t Parsing::convert_to_bytes(uint8_t * src)
{
	if(*src == '\0')
		return 0;

	uint8_t * target = src;

	char hex_array[3];
	hex_array[2] = '\0';

	size_t target_size = 0;

	while(1)
	{
		hex_array[0] = (char)(*src);
		src++;

		if(*src == '\0')
			return 0;

		hex_array[1] = (char)(*src);
		src++;

		*target = (uint8_t)strtoul(hex_array, NULL, 16);
		target++;
		target_size++;

		if(*src == '\0')
			break;
	}

	return target_size;
}

// ----------------------------------------------------------------------------
bool Parsing::string_empty(uint8_t * src)
{
	return strlen((char *)src) == 0;
}

// ----------------------------------------------------------------------------
Parsing::decimal Parsing::parse_decimal(uint8_t * src)
{
	decimal ret = { 0, 0 };

	uint8_t * n1 = src;
	uint8_t * n2 = nullptr;

	while(1)
	{
		if(*src == '\0')
			break;

		if(*src == '.')
		{
			n2 = src + 1;
			break;
		}

		src++;
	}

	ret.num[0] = parse_uint(n1);

	if(n2)
		ret.num[1] = parse_uint(n2);

	return ret;
}








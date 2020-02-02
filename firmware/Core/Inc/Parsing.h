
#ifndef PARSING_H_
#define PARSING_H_

#include "main.hpp"
#include "config.h"

class Parsing
{
public:
	static uint32_t parse_uint(uint8_t * src);
	static int32_t parse_int(uint8_t * src);
	static uint32_t parse_hex(uint8_t * src);
	static size_t convert_to_bytes(uint8_t * src);
	static bool string_empty(uint8_t * src);

	struct decimal
	{
		uint32_t num[2];
	};

	static decimal parse_decimal(uint8_t * src);
};

#endif /* PARSING_H_ */

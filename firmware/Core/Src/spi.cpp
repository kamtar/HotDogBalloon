
#include "spi.h"
#include "main.hpp"

#include "system_init.h"

static bool spi_enabled = false;

// ----------------------------------------------------------------------------
bool enable_spi()
{
	bool was_disabled = !spi_enabled;

	if(!spi_enabled)
	{
		SPI_MUX(true);

		spi_enabled = true;
	}

	return was_disabled;
}

// ----------------------------------------------------------------------------
void disable_spi(bool was_disabled)
{
	if(spi_enabled && was_disabled)
	{
		//SPI_MUX(false); TODO proc to zvysuje spotrebu misto aby ji to snizilo?

		spi_enabled = false;
	}
}

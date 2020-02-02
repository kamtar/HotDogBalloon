
#ifndef CONFIG_H_
#define CONFIG_H_

#define DEBUG_UART 	1

#define NO_SLEEP	0

#define SYS_UART_START_TIMEOUT 		30000	// [ms]
#define SYS_UART_PRINTF_TIMEOUT		100		// [ms]
#define SYS_UART_CMD_TIMEOUT		10000	// [ms]

#define MAX_PARAMS 30

// Commands								   info						parametry			popis parametru

#define CMD_READ_STATE 				0	// Vraci state
#define CMD_READ_EXTFLASH			1	// Vycte obsah flash 		[addr, size] 		addr = libovolna adresa ve flash, size = delka dat
#define CMD_ERASE_EXTFLASH_ALL		2	// Smaze celou flashku
#define CMD_ERASE_EXTFLASH_PAGE		3	// Smaze stranku 			[addr] 				addr = adresa zacatku 4 KB stranky
#define CMD_WRITE_EXTFLASH			4	// Zapise data do flashky	[addr, data]		addr = adresa, data = hex

// Balloon
#define GPS_FIX_TIMEOUT				3*60	// [s]
#define GPS_FIX_PERIOD				1*60	// [s]

#endif /* CONFIG_H_ */

/*
 *  I2C Middleware for CH32X035(rev1+)
 *  Copyright (C) 2022-2024  WuxiProject
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "stdint.h"

#ifdef I2C_REMAP
#undef I2C_REMAP
#endif
#ifdef I2C_RETRY_TIMEOUT
#undef I2C_RETRY_TIMEOUT
#endif
#ifdef I2C_BUSFREQ
#undef I2C_BUSFREQ
#endif

#define I2C_REMAP_NONE (0)
#define I2C_REMAP_1 (1)
#define I2C_REMAP_2 (2)
#define I2C_REMAP_3 (3)
#define I2C_REMAP_4 (4)
#define I2C_REMAP_5 (5)

// Portable configuration
// - You need to implement things below to integrate into your program.
// ##PORT_CONFIGURATION_BEGIN##

/*
 * I2C_REMAP: Defines which group of GPIO combination to use
 *  - I2C_REMAP_NONE			SDA/PA11, SCL/PA10
 *  - I2C_REMAP_1				SDA/PA14, SCL/PA13
 *  - I2C_REMAP_2				SDA/PC17, SCL/PC16
 *  - I2C_REMAP_3				SDA/PC18, SCL/PC19
 *  - I2C_REMAP_4				SDA/PC16, SCL/PC17
 *  - I2C_REMAP_5				SDA/PC19, SCL/PC18
 */
#define I2C_REMAP 		I2C_REMAP_NONE

/*
 * I2C_RETRY_TIMEOUT: Defines max loop count while waiting for last operation done
 */
#define I2C_RETRY_TIMEOUT 11451

/*
 * I2C_BUSFREQ: Defines the SCL frequency used to communicate(Hz)
 */
#define I2C_BUSFREQ 100000

// ##PORT_CONFIGURATION_END##

typedef enum
{
	I2C_START = 0,
	I2C_SEND_ADDRESS,
	I2C_CLEAR_ADDRESS_FLAG,
	I2C_TRANSMIT_DATA,
	I2C_STOP
} I2C_Stage;

void I2C_BusInit();
void I2C_WriteBytes(uint8_t devAddr, uint8_t *pData, uint16_t dataLen);
void I2C_WriteReg8(uint8_t devAddr, uint8_t writeAddr, uint8_t *pData,
		uint16_t dataLen);
void I2C_ReadReg8(uint8_t devAddr, uint8_t readAddr, uint8_t *pData,
		uint16_t dataLen);

#endif /* _I2C_H_ */

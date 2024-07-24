/*
 *  SPI Middleware for CH32X035
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

#ifndef _SPI_H_
#define _SPI_H_

#include  "ch32x035.h"

#ifdef SPI_REMAP
#undef SPI_REMAP
#endif
#ifdef SPI_DIR
#undef SPI_DIR
#endif
#ifdef SPI_CLKEDGE
#undef SPI_CLKEDGE
#endif
#ifdef SPI_CLKPOL
#undef SPI_CLKPOL
#endif
#ifdef SPI_CLKPSC
#undef SPI_CLKPSC
#endif
#ifdef SPI_RETRY_TIMEOUT
#undef SPI_RETRY_TIMEOUT
#endif

#define SPI_REMAP_NONE (0)
#define SPI_REMAP_1 (1)
#define SPI_REMAP_2 (2)
#define SPI_REMAP_3 (3)
#define SPI_DIR_2LINE_FULLDUPLEX (0)
#define SPI_DIR_2LINE_RX (1)
#define SPI_DIR_1LINE_RX (2)
#define SPI_DIR_1LINE_TX (3)

// Portable configuration
// - You need to implement things below to integrate into your program.
// ##PORT_CONFIGURATION_BEGIN##

/*
 * SPI_REMAP: Defines which group of GPIO combination to use
 *  - SPI_REMAP_None			SCK/PA5, MISO/PA6, MOSI/PA7
 *  - SPI_REMAP_1				SCK/PB15, MISO/PA8, MOSI/PA9
 *  - SPI_REMAP_2				SCK/PA11, MISO/PA9, MOSI/PA10
 *  - SPI_REMAP_3				SCK/PC5, MISO/PC6, MOSI/PC7
 */
#define SPI_REMAP 		SPI_REMAP_NONE

/*
 * SPI_DIR: Defines how the SPI peripheral communicates
 *  - SPI_DIR_2LINE_FULLDUPLEX		2 Data lines, Full duplex
 *  - SPI_DIR_2LINE_RX	 			2 Data lines, Receive only
 *  - SPI DIR_1LINE_RX				1 Data line, Receive only
 *  - SPI DIR_1LINE_RX				1 Data line, Transmit only
 */
#define SPI_DIR 		SPI_DIR_2LINE_FULLDUPLEX

/*
 * SPI_CLKEDGE: Defines when to sample data (or when target is expected to sample data)
 *  - SPI_CPHA_1Edge				At the first SCK edge
 *  - SPI_CPHA_2Edge 				At the second SCK edge
 */
#define SPI_CLKEDGE 	SPI_CPHA_2Edge

/*
 * SPI_CLKPOL: Defines the SCK pin level in idle state
 *  - SPI_CPOL_High			SCK is high while idle
 *  - SPI_CPOL_Low			SCK is low while idle
 */
#define SPI_CLKPOL	 	SPI_CPOL_High

/*
 * SPI_CLKPSC: Defines the SCK frequency
 * 	- SPI_BaudRatePrescaler_2			F(SCK)=F(HCLK)/2
 *  - SPI_BaudRatePrescaler_4			F(SCK)=F(HCLK)/4
 *  - SPI_BaudRatePrescaler_8			F(SCK)=F(HCLK)/8
 *  - SPI_BaudRatePrescaler_16			F(SCK)=F(HCLK)/16
 * 	- SPI_BaudRatePrescaler_32			F(SCK)=F(HCLK)/32
 * 	- SPI_BaudRatePrescaler_64			F(SCK)=F(HCLK)/64
 * 	- SPI_BaudRatePrescaler_128			F(SCK)=F(HCLK)/128
 *  - SPI_BaudRatePrescaler_256			F(SCK)=F(HCLK)/256
 */
#define SPI_CLKPSC		SPI_BaudRatePrescaler_8 // gives ~6MHz at default

/*
 * SPI_RETRY_TIMEOUT: Defines max loop count while waiting for last transaction done
 */
#define SPI_RETRY_TIMEOUT 11451U

// ##PORT_CONFIGURATION_END##

extern void SPI_QuickInit();
extern uint8_t SPI_TinyTransaction(uint8_t dataTx);
extern void SPI_HalfDuplexTransaction(uint8_t *dataTx, uint16_t cntTx,
		uint8_t *dataRx, uint16_t cntRx);
extern void SPI_FullDuplexTransaction(uint8_t *dataTx, uint8_t *dataRx,
		uint16_t cnt);
extern uint32_t SPI_WaitForTransaction();

#endif /* _SPI_H_ */

# SPI/Master

A simple SPI master mode library.  

## Feature

- 8-Bit data length
- Full/Half Duplex
- GPIO Remap Config

## Port guide

This library requires some configuration to integrate into your program.  

Just modify configuration items as your need.  

```c
// Portable configuration
// - You need to implement things below to integrate into your program.
// ##PORT_CONFIGURATION_BEGIN##

/*
 * SPI_REMAP: Defines which group of GPIO combination to use
 *  - SPI_Remap_None			SCK/PA5, MISO/PA6, MOSI/PA7
 *  - SPI_Remap_1				SCK/PB15, MISO/PA8, MOSI/PA9
 *  - SPI_Remap_2				SCK/PA11, MISO/PA9, MOSI/PA10
 *  - SPI_Remap_3				SCK/PC5, MISO/PC6, MOSI/PC7
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
```

## Usage demo

This library does not handle chip select (CS, or CE in some docs) signal. Be sure to control them on yourself.

```c
// more code above....

// Before using any other library finctions, call quick init function first.
SPI_QuickInit();
// Then you can start your SPI transaction.
_CS_EN();
// Send 1 byte and read 1 byte.
uint8_t dataRx = SPI_TinyTransaction(dataTx);
// Send bytes, then read bytes.
SPI_HalfDuplexTransaction(bufTx, bufTxLen, bufRx, bufRxLen);
// Also you can just send without receive.
SPI_HalfDuplexTransaction(bufTx, bufTxLen, NULL, 0);
// Don't forget to wait for the transaction done, or you may disable CS signal while data still in transaction.
SPI_WaitForTransaction();
_CS_DIS();

// more code below....
```

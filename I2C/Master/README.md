# I2C/Master

A simple I2C master mode library.  

## Feature

- 7-Bit Address support
- State-machine-based auto fault recovery
- GPIO Remap Config

## Port guide

This library requires some configuration to integrate into your program.  

Just modify configuration items as your need.  

```c
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
```

## Usage demo

This library uses 8-Bit format address, but the LSB is ignored.  For example, to access an EEPROM with address `0xA0`, you can pass `0xA0` or `0xA1`(8-Bit addr) but not `0x50` or `0x51`(7-Bit addr).  

```c
// more code above....

const uint8_t device_addr = 0xA0;
// Before using any other library finctions, call bus init function first.
I2C_BusInit();
// Have a read at register 0x11.
uint8_t result;
I2C_ReadReg8(device_addr, 0x11, &result, 1);

// more code below....
```

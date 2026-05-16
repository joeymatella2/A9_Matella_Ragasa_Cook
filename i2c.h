/*****************************************************************************
* EE 329 A5 DAC
******************************************************************************
* @file           : dac.h
* @brief          : DAC configuration and data word related functions
* authors         : Tyler Ragasa
******************************************************************************/

/* !!! double check this compiles !!! */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

#define EEPROM_ADDRESS (0b1010111) // Set by hardware, 7-bit
#define EEPROM_MEMORY_ADDR () // RNG, 16-bit

#endif /* INC_I2C_H_ */
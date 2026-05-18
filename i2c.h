/*
 *******************************************************************************
 * @file           : I2C.h
 * @brief          : X
 * project         : EE 329 S'26 AX
 * authors         : joeym
 * version         : 0.1
 * date            : May 15, 2026
 * compiler        : STM32CubeIDE v.1.19.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * Description: X
 *
 *******************************************************************************
 * GPIO Wiring
 * |   Component    | GPIO Identifier | Connector Location | Config
 *-----------------------------------------------------------------------------
 * | LCD - DB4 - 11 | PC0             | CN9-3              | OUT
 *******************************************************************************
 * Version History
 *  Ver.|   Date   |  Description
 *  ---------------------------------------------------------------------------
 *      |          | 
 *******************************************************************************
 *
 * Header format adapted from [Code Appendix by Kevin Vo] pg 5
 */

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

void EEPROM_init(void);
void EEPROM_write( uint8_t dataWrite, uint8_t targetAddr, uint16_t memoryAddr);
uint8_t EEPROM_read(uint16_t memoryAddr, uint8_t targetAddr);

#endif /* INC_I2C_H_ */

/**
  ******************************************************************************
  * @file           : I2C.h
  * @brief          : Header for I2C.c file.
  * project         : EE 329 S'26 A9
  * authors         : Joseph Matella, Nathan Cook, Tyler Ragasa
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

#define EEPROM_ADDRESS (0b1010111) // Set by hardware, 7-bit

void EEPROM_init(void);
void EEPROM_write(uint8_t targetAddr, uint16_t memoryAddr, uint8_t dataWrite);
uint8_t EEPROM_read(uint8_t targetAddr, uint16_t memoryAddr);

#endif /* INC_I2C_H_ */

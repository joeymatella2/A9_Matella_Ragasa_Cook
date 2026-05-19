/**
  ******************************************************************************
  * @file           : RNG.h
  * @brief          : Header for RNG.h file.
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



#ifndef INC_RNG_H_
#define INC_RNG_H_

#include "stm32l4xx.h"
#include <stdint.h>

void RNG_Init(void);
uint32_t RNG_ReadRaw(void);
uint8_t RNG_ReadByte(void);
uint16_t RNG_Read15BitAddress(void);

#endif /* INC_RNG_H_ */

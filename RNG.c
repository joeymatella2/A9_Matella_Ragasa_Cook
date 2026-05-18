/*
 *******************************************************************************
 * @file           : RNG.c
 * @brief          : X
 * project         : EE 329 S'26 AX
 * authors         : joeym
 * version         : 0.1
 * date            : May 17, 2026
 * compiler        : STM32CubeIDE v.1.19.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * Description: Random number generation functions
 *
 *******************************************************************************
 * Version History
 *  Ver.|   Date   |  Description
 *  ---------------------------------------------------------------------------
 *      |          | 
 *******************************************************************************
 *
 * Header format adapted from [Code Appendix by Kevin Vo] pg 5
 */

#include "RNG.h"

#include "rng.h"

/* -----------------------------------------------------------------------------
 * function : RNG_Init()
 * INs      : none
 * OUTs     : none
 * action   : Initializes the STM32 hardware random number generator using HSI48
 * -------------------------------------------------------------------------- */
void RNG_Init(void) {
    // Enable RNG peripheral clock
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;

    // Enable HSI48 clock for RNG
    RCC->CRRCR |= RCC_CRRCR_HSI48ON;

    // Wait until HSI48 is ready
    while (!(RCC->CRRCR & RCC_CRRCR_HSI48RDY)) {
        ; // nop
    }

    // Select HSI48 as 48 MHz clock source
    RCC->CCIPR &= ~RCC_CCIPR_CLK48SEL;

    // Enable random number generator
    RNG->CR |= RNG_CR_RNGEN;
}

/* -----------------------------------------------------------------------------
 * function : RNG_ReadRaw()
 * INs      : none
 * OUTs     : 32-bit random number from RNG data register
 * action   : returns raw random data
 * -------------------------------------------------------------------------- */
uint32_t RNG_ReadRaw(void) {
    while ((RNG->SR & RNG_SR_DRDY) == 0) {
        ; // nop
    }

    return RNG->DR;
}

/* -----------------------------------------------------------------------------
 * function : RNG_ReadByte()
 * INs      : none
 * OUTs     : random 8-bit value from 0x00 to 0xFF
 * action   : Generates a random byte
 * -------------------------------------------------------------------------- */
uint8_t RNG_ReadByte(void) {
    return (uint8_t)(RNG_ReadRaw() & 0xFF);
}

/* -----------------------------------------------------------------------------
 * function : RNG_Read15BitAddress()
 * INs      : none
 * OUTs     : random 15-bit address from 0x0000 to 0x7FFF
 * action   : Generates a valid 24LC256 EEPROM address
 * -------------------------------------------------------------------------- */
uint16_t RNG_Read15BitAddress(void) {
    return (uint16_t)(RNG_ReadRaw() & 0x7FFF);
}

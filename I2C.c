/*
 *******************************************************************************
 * @file           : I2C.c
 * @brief          : I2C EEPROM Interface
 * project         : EE 329 S'26 A9
 * authors         : Joseph Matella, Nathan Cook, Tyler Ragasa
 * version         : 0.1
 * date            : May 18, 2026
 * compiler        : STM32CubeIDE v.1.19.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2026 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * Description: Configures I2C communication between the STM32L4 MCU and the
 * 24LC256 EEPROM. Provides EEPROM initialization, single-byte write, and
 * single-byte read functions using a 15-bit EEPROM memory address. The EEPROM
 * uses I2C device addressing and requires two memory address bytes before data
 * can be written or read.
 *******************************************************************************
 * GPIO Wiring
 * |    Component     | GPIO Identifier | Connector Location | Config
 *-------------------------------------------------------------------------------
 * | EEPROM - SCL     | PB8             | CN7-2              | I2C SCL
 * | EEPROM - SDA     | PB9             | CN7-4              | I2C SDA
 *******************************************************************************
 * Header format adapted from [Code Appendix by Kevin Vo] pg 5
 */
#include "I2C.h"

/* -----------------------------------------------------------------------------
 * function : EEPROM_init()
 * INs      : none
 * OUTs     : none
 * action   : Configuration of GPIO pins and registers for I2C1 peripheral
 * Citation : adapted from [Lab Manual] pg 39
 * -------------------------------------------------------------------------- */
void EEPROM_init( void ){
   /* USER configure GPIO pins for I2C alternate functions SCL and SDA */

	// Configure PB9 as I2C1_SDA (AF4)
	// Configure PB8 as I2C1_SCL (AF4)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->AFR[1] &= ~((0xF << 0) | (0XF << 4));
	GPIOB->AFR[1] |=  ((4   << 0) | (4   << 4));

	// open drain, very high speed, no pullup no pulldown
	GPIOB->OTYPER  |= (GPIO_OTYPER_OT9
			            | GPIO_OTYPER_OT8);

	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED9
			             | GPIO_OSPEEDR_OSPEED8);

	GPIOB->PUPDR   &= ~(GPIO_PUPDR_PUPD9
			              | GPIO_PUPDR_PUPD8);

	// Alternate function mode
	GPIOB->MODER   &= ~(GPIO_MODER_MODE9
			              | GPIO_MODER_MODE8);

	GPIOB->MODER   |= (GPIO_MODER_MODE9_1
			            | GPIO_MODER_MODE8_1);

   RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;  // enable I2C bus clock
   I2C1->CR1   &= ~( I2C_CR1_PE );        // put I2C into reset
   I2C1->CR1   &= ~( I2C_CR1_ANFOFF );    // filters: enable analog
   I2C1->CR1   &= ~( I2C_CR1_DNF );       // filters: disable digital
   I2C1->TIMINGR = 0x00000103;            // timing from .ioc
   I2C1->CR2   |=  ( I2C_CR2_AUTOEND );   // auto send STOP after transmission
   I2C1->CR2   &= ~( I2C_CR2_ADD10 );     // 7-bit address mode
   I2C1->CR1   |=  ( I2C_CR1_PE );        // enable I2C

}

/* -----------------------------------------------------------------------------
 * function : EEPROM_write()
 * INs      : I2C target address, EEPROM memory address, Data to write
 * OUTs     : none
 * action   : Writes byte to specified EEPROM memory address
 * Citation : adapted from [Lab Manual] pg 39
 * -------------------------------------------------------------------------- */
void EEPROM_write(uint8_t targetAddr, uint16_t memoryAddr, uint8_t dataWrite){
   I2C1->CR2 &= ~( I2C_CR2_RD_WRN );         // CLR CR = write
   I2C1->CR2 &= ~( I2C_CR2_NBYTES );         // Reset # of bytes
   // 2 bytes mem. address & 1 byte data
   I2C1->CR2 |= ( 3 << I2C_CR2_NBYTES_Pos);  // Transmit 3 bytes

   I2C1->CR2   |=  ( I2C_CR2_AUTOEND );     // auto send STOP after transmission

   // target address
   I2C1->CR2 &= ~( I2C_CR2_SADD );          // Clear target address

   I2C1->CR2 |= ((targetAddr & 0x7F)<<(I2C_CR2_SADD_Pos+1));  // Set address
   I2C1->CR2 |= I2C_CR2_START;              // Start
   while(!(I2C1->ISR & I2C_ISR_TXIS));      // Wait for TXDR
   // 16 bit EEPROM_MEMORY ADDR
   I2C1->TXDR = (memoryAddr >> 8);          // High byte of MEM REG


   while(!(I2C1->ISR & I2C_ISR_TXIS)) ;      // Wait for TXDR
   I2C1->TXDR = (memoryAddr & 0xFF);  // lower addr byte

   while(!(I2C1->ISR & I2C_ISR_TXIS)) ;      // Wait for TXDR
   I2C1->TXDR = (dataWrite);  // data to write

   while(!(I2C1->ISR & I2C_ISR_STOPF))  ;  // wait for STOP
   I2C1->ICR = I2C_ICR_STOPCF; // Clear flag
}

/* -----------------------------------------------------------------------------
 * function : EEPROM_read()
 * INs      : I2C target address, EEPROM memory address
 * OUTs     : none
 * action   : Reads byte from specified EEPROM memory address
 * Citation : adapted from [Lab Manual] pg 39
 * -------------------------------------------------------------------------- */
uint8_t EEPROM_read(uint8_t targetAddr, uint16_t memoryAddr) {
	uint8_t memoryData;
	// build EEPROM transaction
   I2C1->CR2   &=  ~( I2C_CR2_AUTOEND );   // no auto STOP
	I2C1->CR2   &= ~( I2C_CR2_RD_WRN );    // set WRITE mode
	I2C1->CR2   &= ~( I2C_CR2_NBYTES );    // clear Byte count
	I2C1->CR2   |=  ( 2 << I2C_CR2_NBYTES_Pos); // write 2 bytes (2 addr
	I2C1->CR2   &= ~( I2C_CR2_SADD );      // clear device address
	// device addr SHL 1
	I2C1->CR2   |=  ( (targetAddr & 0x7F) << (I2C_CR2_SADD_Pos+1) );
	I2C1->CR2   |=    I2C_CR2_START;       // start I2C WRITE op

	while(!(I2C1->ISR & I2C_ISR_TXIS)) ;   // wait for start condition to transmit
	I2C1->TXDR = (memoryAddr >> 8); // xmit upper byte of memory address

	while(!(I2C1->ISR & I2C_ISR_TXIS)) ;    // Wait for TXDR
	I2C1->TXDR = (memoryAddr & 0xFF); // xmit lower byte of memory address

	// Wait until 2-byte writing phase is complete
   while (!(I2C1->ISR & I2C_ISR_TC)) ;

	// Configure control byte for read phase
   I2C1->CR2   |=  ( I2C_CR2_AUTOEND );   // auto send STOP after transmission
   I2C1->CR2   |= ( I2C_CR2_RD_WRN );    // set Read mode
	I2C1->CR2   &= ~( I2C_CR2_NBYTES );    // clear Byte count
	I2C1->CR2   |=  ( 1 << I2C_CR2_NBYTES_Pos); // read 1 byte
	I2C1->CR2   |=    I2C_CR2_START;       // start I2C read op


	while(!(I2C1->ISR & I2C_ISR_RXNE)) ; // Wait for RXNE
	memoryData = I2C1->RXDR & I2C_RXDR_RXDATA; // Read data

   while(!(I2C1->ISR & I2C_ISR_STOPF))  ;  // wait for STOP
   I2C1->ICR = I2C_ICR_STOPCF; // Clear flag

	return memoryData;
}



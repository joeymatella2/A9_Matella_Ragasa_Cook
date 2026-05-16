/*******************************************************************************
 * 
 * Block Header
 * 
 ******************************************************************************/

#include "i2c.h"

void I2C_init( void ){
   /* USER configure GPIO pins for I2C alternate functions SCL and SDA */
   // Configure I2C 
   RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;  // enable I2C bus clock
   I2C1->CR1   &= ~( I2C_CR1_PE );        // put I2C into reset (release SDA, SCL)
   I2C1->CR1   &= ~( I2C_CR1_ANFOFF );    // filters: enable analog
   I2C1->CR1   &= ~( I2C_CR1_DNF );       // filters: disable digital
   I2C1->TIMINGR = 0x00000103;            // 1 MHz SYSCLK timing from CubeMX
   I2C1->CR2   |=  ( I2C_CR2_AUTOEND );   // auto send STOP after transmission
   I2C1->CR2   &= ~( I2C_CR2_ADD10 );     // 7-bit address mode
   I2C1->CR1   |=  ( I2C_CR1_PE );        // enable I2C

}

void I2C_write( uint8_t dataWrite, uint8_t targetAddr ){
   I2C1->CR2 &= ~( I2C_CR2_RD_WRN );         // CLR CR = write
   I2C1->CR2 &= ~( I2C_CR2_NBYTES );         // Reset # of bytes
   // 2 bytes mem. address & 1 byte data
   I2C1->CR2 |= ( 3 << I2C_CR2_NBYTES_Pos);  // Transmit 3 bytes
   // target address
   I2C1->CR2 &= ~( I2C_CR2_SADD );           // Clear target address

   /*
    ! Different than manual. mckell suggested generalizing this by
    ! passing address as 
    */
   I2C1->CR2 |= ((targetAddr & 0x7F)<<(I2C_CR2_SADD_Pos+1));  // Set address
   I2C1->CR2 |= I2C_CR2_START; // Start
   while(!(I2C1->ISR & I2C_ISR_TXIS)) ;      // Wait for TXDR 

   // 16 bit EEPROM_MEMORY ADDR
   I2C1->TXDR = (EEPROM_MEMORY_ADDR >> 8);   // High byte of MEM REG
   I2C1->TXDR = (EEPROM_MEMORY_ADDR & 0xFF);  // lower byte
   I2C1->TXDR = ();
   // if autoend set, dont need to send stop condition

   // 4 bytes on bus
   // STM should get 4 ACK
   // extern. LED for debug if 
}

void I2C_read( void ){

}


/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * project         : EE 329 S'26 A9
  * authors         : Joseph Matella, Tyler Ragasa, Nathan Cook
  * version         : 0.1
  * date            : May 11, 2026
  *
  * Description     : Initializes the EEPROM, RNG, SysTick, and LED modules.
  *                   Generates a random EEPROM memory address and data byte,
  *                   writes the byte to EEPROM, reads it back, and turns on the
  *                   on-board LED if the read value matches the written value.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is provided for academic use as part of EE 329 S'26 A5.
  *
  ******************************************************************************
  * Header format adapted from [Code Appendix by Kevin Vo] pg 5
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{

  HAL_Init();
  SystemClock_Config();

  // Initializations
  EEPROM_init();
  RNG_Init();
  SysTick_Init();
  LED_Init();

  // Declare variables
  uint16_t memoryAddress;
  uint8_t writeData;
  uint8_t readData;

  while (1)
  {
	  // Generate random memory address and data
	  memoryAddress = RNG_Read15BitAddress();
	  writeData = RNG_ReadByte();
	  // Write to EEPROM
	  EEPROM_write(EEPROM_ADDRESS, memoryAddress, writeData);
	  delay_us(5000);
	  // Read from EEPROM
	  readData = EEPROM_read(EEPROM_ADDRESS, memoryAddress);
	  delay_us(5000);
	  // Check if write data matches read data
	  if (writeData == readData) {
		  // Turn on on-board LED
		  LED_ON();
	  } else {
		  // turn off on-board LED
		  LED_OFF();
	  }
  }
}

/* -----------------------------------------------------------------------------
 * function : LED_Init()
 * INs      : none
 * OUTs     : none
 * action   : Initializes the GPIO pin connected to the on-board LED as an output
 * -------------------------------------------------------------------------- */
void LED_Init(void) {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);
	LED_PORT->MODER &= ~(GPIO_MODER_MODE7);
	LED_PORT->MODER |= (GPIO_MODER_MODE7_0);
	LED_PORT->OTYPER &= ~(GPIO_OTYPER_OT7);
	LED_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD7);
	LED_PORT->OSPEEDR |= (GPIO_OSPEEDR_OSPEED7);

	// Reset Pin
	LED_PORT->BRR |= (LED_PIN);
}

/* -----------------------------------------------------------------------------
 * function : LED_ON()
 * INs      : none
 * OUTs     : none
 * action   : Turns on the on-board LED
 * -------------------------------------------------------------------------- */
void LED_ON(void) {
	LED_PORT->BSRR |= (LED_PIN);
}

/* -----------------------------------------------------------------------------
 * function : LED_OFF()
 * INs      : none
 * OUTs     : none
 * action   : Turns off the on-board LED
 * -------------------------------------------------------------------------- */
void LED_OFF(void) {
	LED_PORT->BRR |= (LED_PIN);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

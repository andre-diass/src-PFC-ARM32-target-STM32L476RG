/*
 * stm32l476rg.h
 *
 *  Created on: Aug 20, 2024
 *      Author: andre
 */

#ifndef STM32L476RG_H_
#define STM32L476RG_H_

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASEADDR						0x08000000U
#define SRAM1_BASEADDR						0x20000000U
#define ROM_BASEADDR						0x1FFF0000U
#define SRAM 								SRAM1_BASEADDR


/*
 * base addresses of peripheral buses
 */

#define APB1_PERIPHBASE                     0x40000000U
#define APB2_PERIPHBASE                     0x40010000U
#define AHB1_PERIPHBASE                     0x40020000U
#define AHB2_PERIPHBASE                     0x48000000U
#define AHB3_PERIPHBASE                     0xA0000000U

/*
 * Base addresses of peripherals which are hanging on AHB2 bus
 */

#define GPIOA_BASEADDR                   (AHB2_PERIPHBASE + 0x0000)
#define GPIOB_BASEADDR                   (AHB2_PERIPHBASE + 0x0400)
#define GPIOC_BASEADDR 					 (AHB2_PERIPHBASE + 0x0800)
#define GPIOD_BASEADDR 					 (AHB2_PERIPHBASE + 0x0C00)
#define GPIOE_BASEADDR 					 (AHB2_PERIPHBASE + 0x1000)
#define GPIOF_BASEADDR 					 (AHB2_PERIPHBASE + 0x1400)
#define GPIOG_BASEADDR 					 (AHB2_PERIPHBASE + 0x1800)
#define GPIOH_BASEADDR 					 (AHB2_PERIPHBASE + 0x1C00)

/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */

#define RCC_BASEADDR                     (AHB1_PERIPHBASE + 0x1000)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */

#define USART1_BASEADDR                   (APB2_PERIPHBASE + 0x3800)
#define EXTI_BASEADDR                     (APB2_PERIPHBASE + 0x0400)


/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */

#define SPI2_BASEADDR                    (APB1_PERIPHBASE + 0x3800)
#define SPI3_BASEADDR                    (APB1_PERIPHBASE + 0x3C00)
#define USART2_BASEADDR 				 (APB1_PERIPHBASE + 0x4400)
#define USART3_BASEADDR 		         (APB1_PERIPHBASE + 0x4800)
#define UART4_BASEADDR 					 (APB1_PERIPHBASE + 0x4C00)
#define UART5_BASEADDR 					 (APB1_PERIPHBASE + 0x5000)





#endif /* STM32L476RG_H_ */

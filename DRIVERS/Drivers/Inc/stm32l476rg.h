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



#endif /* STM32L476RG_H_ */

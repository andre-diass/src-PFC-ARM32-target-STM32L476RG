/*
 * stm32l476rg.h
 *
 *  Created on: Aug 20, 2024
 *      Author: andre
 */

#ifndef STM32L476RG_H_
#define STM32L476RG_H_

#include<stddef.h>
#include<stdint.h>

#define __vo volatile

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASEADDR						0x08000000U
#define SRAM1_BASEADDR						0x20000000U
#define ROM_BASEADDR					  	0x1FFF0000U
#define SRAM 								      SRAM1_BASEADDR


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
#define GPIOC_BASEADDR 					         (AHB2_PERIPHBASE + 0x0800)
#define GPIOD_BASEADDR 					         (AHB2_PERIPHBASE + 0x0C00)
#define GPIOE_BASEADDR 					         (AHB2_PERIPHBASE + 0x1000)
#define GPIOF_BASEADDR 					         (AHB2_PERIPHBASE + 0x1400)
#define GPIOG_BASEADDR 					         (AHB2_PERIPHBASE + 0x1800)
#define GPIOH_BASEADDR 					         (AHB2_PERIPHBASE + 0x1C00)


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
#define USART2_BASEADDR 			        	 (APB1_PERIPHBASE + 0x4400)
#define USART3_BASEADDR 		             (APB1_PERIPHBASE + 0x4800)
#define UART4_BASEADDR 					         (APB1_PERIPHBASE + 0x4C00)
#define UART5_BASEADDR 					         (APB1_PERIPHBASE + 0x5000)

/*
***************peripheral register definition structures************************
*/

typedef struct
{
	__vo uint32_t MODER;                        
	__vo uint32_t OTYPER;                       
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];					 
}GPIO_RegDef_t;

/*
 * peripheral register definition structure for USART
 */
typedef struct
{
  __vo uint32_t CR1;         					
	__vo uint32_t CR2;         					
	__vo uint32_t CR3; 
	__vo uint32_t BRR;
	__vo uint32_t GTPR;        					
	__vo uint32_t RTOR;        				
	__vo uint32_t RQR;
	__vo uint32_t ISR;
	__vo uint32_t ICR;
	__vo uint32_t RDR; 
	__vo uint32_t TDR;        			
} USART_RegDef_t;

/*
 * peripheral register definition structure for RCC
 */

typedef struct
{
  __vo uint32_t CR;            
  __vo uint32_t PLLCFGR;       
  __vo uint32_t CFGR;          
  __vo uint32_t CIR;           
  __vo uint32_t AHB1RSTR;      
  __vo uint32_t AHB2RSTR;      
  __vo uint32_t AHB3RSTR;      
  uint32_t      RESERVED0;     
  __vo uint32_t APB1RSTR;      
  __vo uint32_t APB2RSTR;      
  uint32_t      RESERVED1[2];  
  __vo uint32_t AHB1ENR;       
  __vo uint32_t AHB2ENR;       
  __vo uint32_t AHB3ENR;       
  uint32_t      RESERVED2;     
  __vo uint32_t APB1ENR;       
  __vo uint32_t APB2ENR;       
  uint32_t      RESERVED3[2];  
  __vo uint32_t AHB1LPENR;     
  __vo uint32_t AHB2LPENR;     
  __vo uint32_t AHB3LPENR;     
  uint32_t      RESERVED4;     
  __vo uint32_t APB1LPENR;     
  __vo uint32_t APB2LPENR;     
  uint32_t      RESERVED5[2];  
  __vo uint32_t BDCR;          
  __vo uint32_t CSR;           
  uint32_t      RESERVED6[2];  
  __vo uint32_t SSCGR;         
  __vo uint32_t PLLI2SCFGR;    
  __vo uint32_t PLLSAICFGR;    
  __vo uint32_t DCKCFGR;       
  __vo uint32_t CKGATENR;      
  __vo uint32_t DCKCFGR2;      

} RCC_RegDef_t;

/*
* how will these structures be used.
*
* 
GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t*)0X40020000;
GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t*)GPIOA_BASEADDR;
#define GPIOA           (GPIO_RegDef_t*)GPIOA_BASEADDR
GPIO_RegDef_t *pGPIOA = GPIOA;
*/


/*
 * peripheral definitions ( Peripheral base addresses typecasted to xxx_RegDef_t)
 */

#define GPIOA  				((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB  				((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC  				((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD  				((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE  				((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF  				((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG  				((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH  				((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC           ((RCC_RegDef_t*)RCC_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_EN()    	(RCC->AHB2ENR |= (1 << 0))
#define GPIOB_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 1))
#define GPIOC_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 2))
#define GPIOD_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 3))
#define GPIOE_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 4))
#define GPIOF_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 5))
#define GPIOG_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 6))
#define GPIOH_PCLK_EN()		  (RCC->AHB2ENR |= (1 << 7))

#endif /* STM32L476RG_H_ */

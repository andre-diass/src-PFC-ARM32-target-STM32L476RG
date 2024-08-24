/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>


int main(void)
{
	uint32_t *pClkCtrlReg =   (uint32_t*)0x4002104c;
	uint32_t *pPortBModeReg = (uint32_t*)0x48000400;
	uint32_t *pPortBOutReg =  (uint32_t*)0x48000414;

	//1. enable the clock for GPOIb peripheral in the AHB2 (SET the 2nd bit position)
	*pClkCtrlReg |= 0x02;

	//2. configure the mode of the IO pin as output
	//a. clear the 24th and 25th bit positions (CLEAR)
	*pPortBModeReg &= 0xFCFFFFFF;
	//b. make 24th bit position as 1 (SET)
	*pPortBModeReg |= 0x01000000;

	//3.SET 12th bit of the output data register to make I/O pin-12 as HIGH
	*pPortBOutReg |= 0x1000;


    while(1);
}
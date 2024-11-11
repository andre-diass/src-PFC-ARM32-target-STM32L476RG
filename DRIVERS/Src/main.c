/*
 * 001led_toggle.c
 *
 *  Created on: Feb 1, 2019
 *      Author: admin
 */

#include "stm32l476rg.h"
#include "stm32l476rg_gpio_driver.h"
#include<stdio.h>
#include<string.h>

char msg[1024] = "AT\n\r";

USART_Handle_t usart1_handle;

void USART1_Init(void)
{
	usart1_handle.pUSARTx = USART1;
	usart1_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart1_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart1_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	usart1_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart1_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart1_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_initialize(&usart1_handle);
}

void USART1_GPIOInit(void)
{
	GPIO_Handle_t usart_gpios;
	GPIO_PeriClockControl(GPIOA, ENABLE);

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode =7;


	usart_gpios.GPIO_PinConfig.GPIO_PinNumber  = GPIO_PIN_NO_9;
	GPIO_Init(&usart_gpios);

	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&usart_gpios);

}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


int main(void)
{

	USART1_GPIOInit();
	USART1_Init();
	USART_peripheral_control(USART1,ENABLE);
    while (1)
    {
        USART_send_data(&usart1_handle,(uint8_t*)msg,strlen(msg));
    }
    return 0;
}

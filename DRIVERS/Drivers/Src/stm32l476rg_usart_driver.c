#include "stm32l4076rg_usart_driver.h"
#include "stm32l476rg.h"


void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t Cmd)
{
	if(Cmd == ENABLE)
	{
		pUSARTx->CR1 |= (1 << 0); 
	}else
	{
		pUSARTx->CR1 &= ~(1 << 0);
	}

}

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pUSARTx == USART1)
		{
			USART1_PCCK_EN();
		}else if (pUSARTx == USART2)
		{
			USART2_PCCK_EN();
		}else if (pUSARTx == USART3)
		{
			USART3_PCCK_EN();
		}
		else if (pUSARTx == UART4)
		{
			UART4_PCCK_EN();
		}
	}
	else
	{
		//TODO
	}

}

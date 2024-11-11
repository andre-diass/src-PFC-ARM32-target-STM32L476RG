#include "stm32l4076rg_usart_driver.h"
#include "stm32l476rg.h"

uint16_t AHB1_PRESCALER[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PRESCALAR[4] = { 2, 4 , 8, 16};


void USART_set_baud_rate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

  uint32_t tempreg=0;

  //Get the value of APB bus clock in to the variable PCLKx
  if(pUSARTx == USART1 )
  {
	   //USART1 is hanging on APB2 bus
	   PCLKx = RCC_get_clock_value();
  }
  
  //Check for OVER8 configuration bit
  if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
  {
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
  }else
  {
	   //over sampling by 16
	   usartdiv = ((25 * PCLKx) / (4 *BaudRate));
  }

  //Calculate the Mantissa part
  M_part = usartdiv/100;

  //Place the Mantissa part in appropriate bit position . refer USART_BRR
  tempreg |= M_part << 4;

  //Extract the fraction part
  F_part = (usartdiv - (M_part * 100));

  //Calculate the final fractional
  if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
   {
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

   }else
   {
	   //over sampling by 16
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

   }

  //Place the fractional part in appropriate bit position . refer USART_BRR
  tempreg |= F_part;

  //copy the value of tempreg in to BRR register
  pUSARTx->BRR = tempreg;
}



void USART_peripheral_control(USART_RegDef_t *pUSARTx, uint8_t Cmd)
{
	if(Cmd == ENABLE)
	{
		pUSARTx->CR1 |= (1 << 0); 
	}else
	{
		pUSARTx->CR1 &= ~(1 << 0);
	}

}

void USART_peri_clock_control(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
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

void USART_initialize(USART_Handle_t *pUSARTHandle)
{

	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	//enable the Clock for given USART peripheral
	 USART_peri_clock_control(pUSARTHandle->pUSARTx,ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration
	if ( pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		tempreg|= (1 << USART_CR1_RE);
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		tempreg |= ( 1 << USART_CR1_TE );

	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		tempreg |= (USART_CR1_RE | USART_CR1_TE);
	}

    //configure the Word length configuration item
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M ;


    //Configuration of parity control bit fields
	if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		tempreg |= ( 1 << USART_CR1_PCE);

		

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		//enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    //enable ODD parity
	    tempreg |= ( 1 << USART_CR1_PS);

	}

   //Program the CR1 register
	pUSARTHandle->pUSARTx->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//configure the number of stop bits inserted during USART frame transmission
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

	//Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);


	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//enable RTS flow control
		tempreg |= ( 1 << USART_CR3_RTSE);

	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//enable both CTS and RTS Flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}


	pUSARTHandle->pUSARTx->CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/
USART_set_baud_rate(pUSARTHandle->pUSARTx,pUSARTHandle->USART_Config.USART_Baud);
	
	

}

uint8_t USART_get_flag_status(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName)
{
    if(pUSARTx->ISR & StatusFlagName)
    {
    	return SET;
    }

   return RESET;
}


void USART_send_data(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;

   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the ISR register
		// TXE: Transmit data register empty. This bit is set by hardware when the content of the USART_TDR register has been transferred into the shift register. It is cleared by a write to the USART_TDR register.

		//enquanto a função retornar 1(TXE =1), o loop vai rodar
		while(!USART_get_flag_status(pUSARTHandle->pUSARTx,USART_FLAG_TXE));

		//Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->TDR = (*pdata & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used in this transfer , so 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->TDR = (*pTxBuffer  & (uint8_t)0xFF);

			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while(!USART_get_flag_status(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}

uint32_t RCC_get_clock_value(void)
{
    uint32_t SystemClock = 0, tmp, pclk2;
    uint8_t clk_src = (RCC->CFGR >> 0) & 0x3; // Bits 1:0 SW: System clock switch

    // Determine the system clock source
    switch (clk_src)
    {
        case 0x00: // MSI oscillator used as system clock
            SystemClock = 4000000; // Default MSI clock (4 MHz, adjustable in some configurations)
            break;
        case 0x01: // HSI16 oscillator used as system clock
            SystemClock = 16000000; // 16 MHz HSI
            break;
        case 0x02: // HSE used as system clock
            SystemClock = 8000000; // HSE clock (8 MHz, might vary with setup)
            break;
        case 0x03: // PLL used as system clock
            SystemClock = 80000000; // Assumed PLL output; verify with actual PLL settings
            break;
    }

    // AHB prescaler
    tmp = (RCC->CFGR >> 4) & 0xF; // Bits 7:4 HPRE: AHB prescaler
    uint8_t ahbp = (tmp < 8) ? 1 : (1 << (tmp - 7)); // Decode prescaler value

    // APB2 prescaler
    tmp = (RCC->CFGR >> 13) & 0x7; // Bits 13:11 PPRE2: APB2 high-speed prescaler
    uint8_t apb2p = (tmp < 4) ? 1 : (1 << (tmp - 3)); // Decode prescaler value

    // Calculate PCLK2 (APB2 clock)
    pclk2 = (SystemClock / ahbp) / apb2p;

    return pclk2;
}



// void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
// {
//    //Loop over until "Len" number of bytes are transferred
// 	for(uint32_t i = 0 ; i < Len; i++)
// 	{
//      while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE));
// 				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->RDR  & (uint8_t)0xFF);
// 				
// 			//Now , increment the pRxBuffer
// 			pRxBuffer++;
// 		}
// 	}
//   

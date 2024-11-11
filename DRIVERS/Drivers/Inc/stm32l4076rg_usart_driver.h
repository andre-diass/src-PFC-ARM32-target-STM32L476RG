/*
 * STM32F4x_gpio_driver.h
 *
 *  Created on: 27-Nov-2018
 *      Author: kiran
 */

#ifndef STM32F446X_UART_DRIVER_H_
#define STM32F446X_UART_DRIVER_H_

#include "stm32l476rg.h"


/*
 * Configuration structure for USARTx peripheral
 User configurable items
 */
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
	uint32_t ErrorCode;
}USART_Handle_t;


/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 	0
#define USART_MODE_ONLY_RX 	1
#define USART_MODE_TXRX  	2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3


/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3


/*
 * USART flags
 */

#define USART_FLAG_TXE 			( 1 << 7)
#define USART_FLAG_RXNE 		USART_ISR_RXNE 
#define USART_FLAG_TC 			( 1 << 6)

/*
 * Application states
 */
#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0


#define 	USART_EVENT_TX_CMPLT   0
#define		USART_EVENT_RX_CMPLT   1
#define		USART_EVENT_IDLE      2
#define		USART_EVENT_CTS       3
#define		USART_EVENT_PE        4
#define		USART_ERR_FE     	5
#define		USART_ERR_NE    	 6
#define		USART_ERR_ORE    	7

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/


/*
 * Peripheral Clock setup
 */
void USART_peri_clock_control(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
void USART_peripheral_control(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
/*
 * Init
 */
void USART_initialize(USART_Handle_t *pUSARTHandle);
/*
 * Data Send and Receive
 */
void USART_send_data(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
// StatusTypeDef USART_ReceiveData(USART_Handle_t *huart, uint8_t *pData, uint16_t Size);
uint8_t USART_get_flag_status(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);

uint32_t RCC_get_clock_value(void);
void USART_set_baud_rate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

#endif /* STM32F446X_UART_DRIVER_H_ */

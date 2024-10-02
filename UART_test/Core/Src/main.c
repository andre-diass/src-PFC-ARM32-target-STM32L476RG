/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t buffer[100] = {0};
char IMEI[30];
char ATcommand[80];
char coordinates[200];
const char APN[]  = "virtueyes.com.br";
char USER[] = "virtu";
char PASSWORD[] = "virtu";
const char resource[] = "/prd/api/products";
const char server[] = "";
uint16_t distance = 12;
char content[80];





void SIMTransmit(char *cmd)
{
  memset(buffer,0,sizeof(buffer));
  HAL_UART_Transmit(&huart1,(uint8_t *)cmd,strlen(cmd),1000);
  HAL_UART_Receive (&huart1, buffer, 100, 1000);
  printf("Response: %s\n", buffer);
}



// Extract IMEI from buffer and save it to the IMEI variable
void extractIMEI(void)
{
  SIMTransmit("AT+GSN\r\n");

  if (strstr((char *)buffer, "OK"))
  {
    char *imei_start = strstr((char *)buffer, "\r\n") + 2; // The IMEI starts after the "\r\n"
    strncpy(IMEI, imei_start, 15);  // IMEI is typically 15 digits long
    IMEI[15] = '\0';  // Null-terminate the string
    printf("IMEI: %s\n", IMEI);
  }
  else
  {
    printf("Failed to retrieve IMEI.\n");
  }
}

void getGPSCoordinates(void)
{
    bool gpsDataReceived = false;

    // Command to request GNSS information
    const char *gpsCommand = "AT+CGNSINF\r\n";

    // Clear the coordinates buffer
    memset(coordinates, 0, sizeof(coordinates));
    SIMTransmit("AT+CGNSPWR=1\r\n");

    while (!gpsDataReceived)
    {
        // Transmit the GPS command
        SIMTransmit(gpsCommand);

        // Check if the response contains valid GPS data
        if (strstr((char *)buffer, "+CGNSINF: 1,1,"))
        {
        	// Copy the entire buffer into the coordinates variable
        	            strncpy(coordinates, (char *)buffer, sizeof(coordinates) - 1);
        	            coordinates[sizeof(coordinates) - 1] = '\0';  // Ensure null-termination

        	            gpsDataReceived = true;
        	            printf("GPS coordinates received: %s\n", coordinates);
        }
        else
        {
            printf("Waiting for valid GPS data...\n");
            HAL_Delay(4000);  // Wait before sending the command again
        }
    }
}


void httpPost(void)
{
  bool ATisOK = false;
  bool CGREGisOK = false;
  bool NETOPENisOK = false;
  bool REQUESTSENT = false;

  //Check if module is receiving and responding to commands
  while(!ATisOK)
  {
    SIMTransmit("AT\r\n");
    if(strstr((char *)buffer,"OK"))
    {
      ATisOK = true;
      printf("AT is OK\n");
    }
  }

  // Check for network registration.
  if(ATisOK)
  {

       SIMTransmit("AT+CMEE=0\r\n"); //Enables the use of result code.
       SIMTransmit("AT+CLTS=1\r\n"); //Enable the local time stamp. The module will update its internal clock to the network-provided time, if available
       SIMTransmit("AT+CBATCHK=1\r\n"); //Enable the battery voltage checking feature.
       SIMTransmit("AT+CPIN?\r\n"); //Queries the current status of the SIM card's PIN and provides information about whether the SIM card is ready for use or requires a PIN
       SIMTransmit("AT+CGSN\r\n"); //Request Product Serial Number Identification
       SIMTransmit("AT+CFUN=0\r\n"); //Sets the functionality level of the modem  by controlling its RF (radio frequency) transmission and other functionalities to minimum.
       SIMTransmit("AT+CNMP=2\r\n"); // Sets preferred Mode (GSM, LTE) Selection to automatic.
       SIMTransmit("AT+CMNB=1\r\n"); // Sets preferred Selection between CAT-M and NB-IoT to CAT-M
       SIMTransmit("AT+CFUN=1\r\n"); // Sets functionalities to full.
       sprintf(ATcommand,"AT+CGDCONT=1,\"IP\",\"%s\",\"0.0.0.0\",0,0,0,0\r\n",APN);
       SIMTransmit(ATcommand); // Define a Packet Data Protocol (PDP) context with  AccesSIMTransmit("AT+CGREG?\r\n"); // Query the Network Registration status for GPRS.
       sprintf(ATcommand,"AT+CGDCONT=13,\"IP\",\"%s\",\"0.0.0.0\",0,0,0,0\r\n",APN);
       SIMTransmit(ATcommand);
       extractIMEI();          // Extract IMEI number


  }

  // Query the Network Registration status for GPRS.
  if(ATisOK)
    {
      while(!CGREGisOK)
      {
        SIMTransmit("AT+CGREG?\r\n");
        if(strstr((char *)buffer,"+CGREG: 0,1"))
        {
          CGREGisOK = true;
          printf("Network registration OK\n");
        }
      }
    }



  // Check for Internet IP Connection
  if(ATisOK && CGREGisOK)
  {

    while(!NETOPENisOK )
    {
	    SIMTransmit("AT+CSQ\r\n");
    	SIMTransmit("AT+CIPSHUT\r\n"); // Close connections
    	SIMTransmit("AT+CGATT=0\r\n"); // Detach from the GPRS (General Packet Radio Service) network
    	sprintf(ATcommand,"AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
    	SIMTransmit(ATcommand); // Set the connection type to GPRS:
    	sprintf(ATcommand,"AT+SAPBR=3,1,\"APN\",\"%s\"\r\n",APN);
    	SIMTransmit(ATcommand); // Set the APN
    	sprintf(ATcommand,"AT+SAPBR=3,1,\"USER\",\"%s\"\r\n",USER);
    	SIMTransmit(ATcommand); // Set user
    	sprintf(ATcommand,"AT+SAPBR=3,1,\"PWD\",\"%s\"\r\n",PASSWORD);
    	SIMTransmit(ATcommand); // Set password
    	sprintf(ATcommand,"AT+CGDCONT=1,\"IP\",\"%s\"\r\n",APN);
    	SIMTransmit(ATcommand); // Set APN again
    	SIMTransmit("AT+CGATT=1\r\n"); // Again but now attach
    	SIMTransmit("AT+CGACT=1,1\r\n"); // Again but now Specifies the context identifier (CID) to activate with the seconda parameter
    	SIMTransmit("AT+SAPBR=1,1\r\n"); // To open the bearer connection
    	SIMTransmit("AT+SAPBR=2,1\r\n"); // Query Bearer Status
    	SIMTransmit("AT+CIPMUX=1\r\n"); // Configure the "Multiple IP Connection Mode", allows multiple simultaneous connections
    	SIMTransmit("AT+CIPQSEND=1\r\n"); // Controls the mode for sending data over an IP connection to 'Quick Send Mode'.
    	SIMTransmit("AT+CIPRXGET=1\r\n"); // Instructing the module to check how much data has been received from the network but has not yet been read or processed from the buffer
    	//sprintf(ATcommand,"AT+CGDCONT=1,\"IP\",\"%s\"\r\n",APN);
    	//SIMTransmit(ATcommand); // Set APN again
    	SIMTransmit("AT+CSTT=\"virtueyes.com.br\",\"virtu\",\"virtu\"\r\n");
    	SIMTransmit("AT+CIICR\r\n");  // Bring Up Wireless Connection with GPRS
    	SIMTransmit("AT+CIFSR\r\n");  // Get Local IP Address
    	SIMTransmit("AT+CGATT?\r\n"); // Query GPRS status and expect it to be '+CGATT: 1'
    	//getGPSCoordinates();


      if(strstr((char *)buffer,"+CGATT: 1"))
      {
        NETOPENisOK = true;
        printf("Internet IP Connection is OK\n");
      }
    }
  }

  if(ATisOK && CGREGisOK && NETOPENisOK)
    {

      while(!REQUESTSENT )
		  {

    	  sprintf(content,"key=a@4K3&distance=%d",distance);

		  SIMTransmit("AT+CSQ\r\n");
    	  SIMTransmit("AT+CIPCLOSE=0\r\n");  // Get Local IP Address
    	  SIMTransmit("AT+CIPSTART=0,\"TCP\",\"d1c23ojg0wdyum.cloudfront.net\",80\r\n");


    	  // Perform http request
    	      sprintf(ATcommand,"AT+CIPSEND=0,%d\r\n",strlen(resource)+16);
    	      SIMTransmit(ATcommand);
    	      if(strstr((char *)buffer,">"))
    	      {
    	        sprintf(ATcommand,"POST %s HTTP/1.1\r\n",resource);
    	        SIMTransmit(ATcommand);
    	      }

    	      sprintf(ATcommand,"AT+CIPSEND=0,%d\r\n",strlen(server)+8);
    	      SIMTransmit(ATcommand);
    	      if(strstr((char *)buffer,">"))
    	      {
    	        sprintf(ATcommand,"Host: %s\r\n",server);
    	        SIMTransmit(ATcommand);
    	      }

    	      SIMTransmit("AT+CIPSEND=0,19\r\n");
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit("Connection: close\r\n");
    	      }

    	      SIMTransmit("AT+CIPSEND=0,49\r\n");
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit("Content-Type: application/x-www-form-urlencoded\r\n");
    	      }

    	      SIMTransmit("AT+CIPSEND=0,16\r\n");
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit("Content-Length: ");
    	      }

    	      char sLength[5];
    	      snprintf(sLength, 5,"%d",strlen(content));
    	      sprintf(ATcommand,"AT+CIPSEND=0,%d\r\n",strlen(sLength));
    	      SIMTransmit(ATcommand);
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit(sLength);
    	      }

    	      SIMTransmit("AT+CIPSEND=0,2\r\n");
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit("\r\n");
    	      }

    	      SIMTransmit("AT+CIPSEND=0,2\r\n");
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit("\r\n");
    	      }

    	      sprintf(ATcommand,"AT+CIPSEND=0,%d\r\n",strlen(content));
    	      SIMTransmit(ATcommand);
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit(content);
    	      }

    	      SIMTransmit("AT+CIPSEND=0,2\r\n");
    	      if(strstr((char *)buffer,">"))
    	      {
    	        SIMTransmit("\r\n");
    	      }
    	      HAL_Delay(2000);
    	      // Close connections
    	      SIMTransmit("AT+CIPCLOSE=0\r\n");
    	      SIMTransmit("AT++NETCLOSE\r\n");



                               int dataLength = 0;
                                           char *ptr;
                                           char x_value_str[10];

                                           // Loop until data is available
                                           do {
                                               SIMTransmit("AT+CIPRXGET=4,0\r\n");
                                               HAL_Delay(1000); // Wait for the response

                                               // Check if the response contains "+CIPRXGET: 4,0,<x>"
                                               if ((ptr = strstr((char *)buffer, "+CIPRXGET: 4,0,")) != NULL) {
                                                   ptr += strlen("+CIPRXGET: 4,0,"); // Move pointer to the start of <x>

                                                   // Extract the value of <x>
                                                   int i = 0;
                                                   while (*ptr >= '0' && *ptr <= '9' && i < sizeof(x_value_str) - 1) {
                                                       x_value_str[i++] = *ptr++;
                                                   }
                                                   x_value_str[i] = '\0';

                                                   dataLength = atoi(x_value_str);

                                                   if (dataLength > 1) {
                                                       break; // Data is available
                                                   }
                                               }

                                               printf("No data available yet. Waiting...\n");
                                               HAL_Delay(2000); // Wait before next attempt
                                           } while (1);

                                           // Now dataLength holds the number of bytes to read
                                           // Prepare command to read the data
                                           sprintf(ATcommand, "AT+CIPRXGET=2,0,%d\r\n", dataLength);
                                           SIMTransmit(ATcommand);

                                           // Parse and print the received data
                                           if ((ptr = strstr((char *)buffer, "+CIPRXGET: 2,0,")) != NULL) {
                                               ptr += strlen("+CIPRXGET: 2,0,");

                                               // Skip over <read_length> and <left_length>
                                               while (*ptr != '\r' && *ptr != '\n' && *ptr != '\0') ptr++;
                                               if (*ptr == '\r') ptr++;
                                               if (*ptr == '\n') ptr++;

                                               // Now ptr points to the start of the data
                                               char receivedData[1024];
                                               int dataToCopy = dataLength;
                                               if (dataToCopy > sizeof(receivedData) - 1) {
                                                   dataToCopy = sizeof(receivedData) - 1;
                                               }

                                               strncpy(receivedData, ptr, dataToCopy);
                                               receivedData[dataToCopy] = '\0';

                                               printf("Received Data: %s\n", receivedData);
                                           } else {
                                               printf("Failed to parse received data.\n");
                                           }

                                           // [Added code ends here]

                                           SIMTransmit("AT+CIPCLOSE=0\r\n");

                                           if(strstr((char *)buffer,"OK"))
                                           {
                                               REQUESTSENT = true;
                                               printf("Post request was sent\n");
                                           }



                                   SIMTransmit("AT+CIPCLOSE=0\r\n");




        if(strstr((char *)buffer,"+CGATT: 1"))
        {
          REQUESTSENT = true;
          printf("Post request was sent\n");
        }
      }
    }


}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  httpPost();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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

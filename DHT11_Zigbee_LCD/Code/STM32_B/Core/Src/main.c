#include "main.h"
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);

#define Dia_chi_LCD 0x4E

void Lcd_Ghi_Lenh (char lenh)
{
 char data_u, data_l;
 uint8_t data_t[4];
 data_u = (lenh&0xf0);
 data_l = ((lenh<<4)&0xf0);
 data_t[0] = data_u|0x0C; //en=1, rs=0
 data_t[1] = data_u|0x08; //en=0, rs=0
 data_t[2] = data_l|0x0C; //en=1, rs=0
 data_t[3] = data_l|0x08; //en=0, rs=0
 HAL_I2C_Master_Transmit (&hi2c1, Dia_chi_LCD,(uint8_t *) data_t, 4, 100);
} 

void Lcd_Ghi_Dulieu (char data)
{
 char data_u, data_l;
 uint8_t data_t[4];
 data_u = (data&0xf0);
 data_l = ((data<<4)&0xf0);
 data_t[0] = data_u|0x0D; //en=1, rs=1
 data_t[1] = data_u|0x09; //en=0, rs=1
 data_t[2] = data_l|0x0D; //en=1, rs=1
 data_t[3] = data_l|0x09; //en=0, rs=1
 HAL_I2C_Master_Transmit (&hi2c1, Dia_chi_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void)
{
 Lcd_Ghi_Lenh (0x03);
 HAL_Delay(50);
 Lcd_Ghi_Lenh (0x02);
 HAL_Delay(50);
 Lcd_Ghi_Lenh (0x06);
 HAL_Delay(50);
 Lcd_Ghi_Lenh (0x0c);
 HAL_Delay(50);
 Lcd_Ghi_Lenh (0x28);
 HAL_Delay(50);
 Lcd_Ghi_Lenh (0x80);
} 

void Lcd_Ghi_Chuoi (char *str)
{
 while (*str)
	 Lcd_Ghi_Dulieu (*str++);
}

void Lcd_xoa_manhinh (void)
{
 Lcd_Ghi_Lenh (0x01); //xoa man hinh
}

char M[100];
char buffer[10];      
char lastData[10] = "";  
float temperature = 0.0;
float humidity = 0.0;


int main(void){

 
	
	HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
	
lcd_init();

Lcd_xoa_manhinh();
HAL_Delay(2);
Lcd_Ghi_Lenh(0x84);
sprintf(M,"Wait.....");
Lcd_Ghi_Chuoi(M);
HAL_Delay(1000);
Lcd_xoa_manhinh();
HAL_Delay(2);

while (1)
{	

    if (HAL_UART_Receive(&huart1, buffer, 9, 100) == HAL_OK)
    {
			buffer[9] = '\0';  
      
			if (strcmp((char*)buffer, lastData) != 0)
      {
				strcpy(lastData, (char*)buffer);
				Lcd_xoa_manhinh();
				HAL_Delay(2);
				
//			if (strncmp((char*)buffer, "Error", 5) == 0)				
				if (strchr(buffer, ',') == NULL)
				{
						Lcd_Ghi_Lenh(0x82);
						sprintf(M,"Loi cam bien");
						Lcd_Ghi_Chuoi(M);
				}
				else
				{
					char *token = strtok((char*)buffer, ",");
					
					if (token != NULL)
						temperature = atof(token);
					token = strtok(NULL, ",");
					if (token != NULL)
						humidity = atof(token);
					
					
          Lcd_Ghi_Lenh(0x80);
					sprintf(M,"Nhiet do: %.1f'C",temperature);
					Lcd_Ghi_Chuoi(M);
					Lcd_Ghi_Lenh(0xC0);
					sprintf(M,"Do am: %.0f %%",humidity);
					Lcd_Ghi_Chuoi(M);
						
					
					if (temperature > 35.0)
          { 
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
          }
          else
          {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
          }
					
				}
				
			}
				
    }
    HAL_Delay(10);
}


}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

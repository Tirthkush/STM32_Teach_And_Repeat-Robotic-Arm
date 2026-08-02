/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pca9685.h"
#include <stdio.h>
#include <string.h>
#include "eeprom.h"
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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char data[128];
uint16_t adc[4];

uint16_t x;
uint16_t y;
uint8_t angleX;
uint8_t angleY;

uint16_t eepromAddress = 2;
uint16_t frameCount = 0;
uint8_t record = 0;
uint8_t play = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  PCA9685_Init(&hi2c1);
  EEPROM_Init(&hi2c1);
  PCA9685_SetPWMFreq(50);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc,4);
  uint8_t temp;
  EEPROM_ReadByte(0,&temp);
  uint8_t mode1;

  //----------------- PCA9685 -----------------
  if(HAL_I2C_Mem_Read(&hi2c1,PCA9685_ADDRESS,MODE1,I2C_MEMADD_SIZE_8BIT,&mode1,1,100) == HAL_OK){
      sprintf(data,"PCA9685 Connected! MODE1=0x%02X\r\n",mode1);
  }
  else {
      sprintf(data,"PCA9685 NOT Connected!\r\n");
  }

  HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);

  //----------------- EEPROM -----------------

  if(HAL_I2C_IsDeviceReady(&hi2c1,EEPROM_ADDRESS,3,100) == HAL_OK) {
      sprintf(data,"EEPROM Connected!\r\n");
  }
  else {
      sprintf(data,"EEPROM NOT Connected!\r\n");
  }
  HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      //---------------- Touch Buttons ----------------

	  //__________________Record_______________________

      if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)==GPIO_PIN_SET)
      {
          record = !record;

          if(record)
          {
        	  play = 0;
              eepromAddress = 2;
              frameCount = 0;

              sprintf(data,"RECORD START\r\n");
          }
          else
          {
        	    EEPROM_WriteByte(0, (frameCount >> 8) & 0xFF);
        	    EEPROM_WriteByte(1, frameCount & 0xFF);

        	    uint8_t high, low;

        	    EEPROM_ReadByte(0, &high);
        	    EEPROM_ReadByte(1, &low);

        	    sprintf(data,"RECORD STOP\r\nWritten=%d Read=%d\r\n",frameCount,(high << 8) | low);
        	    HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);
          }

          HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);

          while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)==GPIO_PIN_SET);
          HAL_Delay(200);
      }

      //________________PLAY_______________

      if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15)==GPIO_PIN_SET)
      {
          uint8_t high, low;

          EEPROM_ReadByte(0, &high);
          EEPROM_ReadByte(1, &low);

          frameCount = (high << 8) | low;
          sprintf(data,"High=%d Low=%d Frame=%d\r\n",high,low,frameCount);
          HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);

          if(frameCount > 0)
          {
              record = 0;          // Stop recording
              play = 1;
              eepromAddress = 2;

              sprintf(data,"PLAYBACK START (%d Frames)\r\n",frameCount);
          }
          else
          {
              play = 0;
              sprintf(data,"NO RECORDING FOUND\r\n");
          }

          HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);
          while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15)==GPIO_PIN_SET);
          HAL_Delay(200);
      }

      //---------------- Read Joysticks ----------------

      uint16_t x1=adc[0];
      uint16_t y1=adc[1];
      uint16_t x2=adc[2];
      uint16_t y2=adc[3];

      uint8_t angle0;
      uint8_t angle1;
      uint8_t angle2;
      uint8_t angle3;

      if(x1<1980)
          angle0=(x1*90)/1980;
      else
          angle0=90+((x1-1980)*90)/(4029-1980);

      if(y1<2007)
          angle1=(y1*90)/2007;
      else
          angle1=90+((y1-2007)*90)/(4029-2007);

      if(x2<1967)
          angle2=(x2*90)/1967;
      else
          angle2=90+((x2-1967)*90)/(4029-1967);

      if(y2<2000)
          angle3=(y2*90)/2000;
      else
          angle3=90+((y2-2000)*90)/(4029-2000);

      //---------------- Dead Zone ----------------

      if((x1>1930)&&(x1<2030)) angle0=90;
      if((y1>1957)&&(y1<2057)) angle1=90;
      if((x2>1917)&&(x2<2017)) angle2=90;
      if((y2>1950)&&(y2<2050)) angle3=90;

      //---------------- RECORD ----------------

      if(record && !play)
      {
          EEPROM_WriteByte(eepromAddress++,angle0);
          EEPROM_WriteByte(eepromAddress++,angle1);
          EEPROM_WriteByte(eepromAddress++,angle2);
          EEPROM_WriteByte(eepromAddress++,angle3);

          frameCount++;

          sprintf(data,"Saved Frame %d\r\n",frameCount);
          HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);
          HAL_Delay(100);
      }

      //---------------- PLAYBACK ----------------

      if(play)
      {
          uint8_t a0,a1,a2,a3;
          for(uint16_t i=0;i<frameCount;i++)
          {
              EEPROM_ReadByte(eepromAddress++,&a0);
              EEPROM_ReadByte(eepromAddress++,&a1);
              EEPROM_ReadByte(eepromAddress++,&a2);
              EEPROM_ReadByte(eepromAddress++,&a3);

              PCA9685_SetServoAngle(0,a0);
              PCA9685_SetServoAngle(1,a1);
              PCA9685_SetServoAngle(2,a2);
              PCA9685_SetServoAngle(3,a3);

              sprintf(data,"Playing %d/%d\r\n",i+1,frameCount);
              HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);
              HAL_Delay(100);
          }
          play=0;
          eepromAddress = 2;

          sprintf(data,"PLAYBACK COMPLETE\r\n");
          HAL_UART_Transmit(&huart1,(uint8_t*)data,strlen(data),100);
      }

      //---------------- Normal Servo Control ----------------

      if(!play)
      {
          PCA9685_SetServoAngle(0,angle0);
          PCA9685_SetServoAngle(1,angle1);
          PCA9685_SetServoAngle(2,angle2);
          PCA9685_SetServoAngle(3,angle3);
      }
      HAL_Delay(20);
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
#ifdef USE_FULL_ASSERT
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

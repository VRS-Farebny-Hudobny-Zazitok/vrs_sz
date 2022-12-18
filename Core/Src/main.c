/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "keyboard.h"
#include "ledlight.h"
#include "colors.h"
#include "musiclightdriver.h"
#include "tone.h"
#include "notes.h"
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

/* USER CODE BEGIN PV */
uint8_t pressedKey;
uint8_t flagKeyboard;
uint8_t keyboardStatus[16] = {0};
uint8_t octave = 3;

float notes3[7] = {C3, D3, E3, F3, G3, A3, H3};
float notes4[8] = {C4, D4, E4, F4, G4, A4, H4, C5};
float notes5[8] = {C5, D5, E5, F5, G5, A5, H5, C6};

Color mainColors[8] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, RED};
Color ledLight[NUMBER_OF_LEDS] = {0};
uint8_t tones[] = {NONE,NONE,NONE,NONE};
double freqs[] = {0,0,0,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t isPressed() {
	for (uint8_t i=1; i<16; i++) {
		if (keyboardStatus[i]) {
			return 1;
		}
	}
	return 0;
}


void setOctave(uint8_t octave) {
	uint8_t pressed = isPressed();

	for (uint8_t i=1; i<8; i++) {
		if (keyboardStatus[i]) {
			updateSingleTone(SAW, notes3[i-1]);
			setMainToneColor(ledLight, mainColors[i-1]);
			setBeatColor(ledLight, mainColors[i-1]);
			setBackingTrackColor(ledLight, mainColors[i-1]);
			sendLedData(ledLight);

		}
	}

	switch (octave) {
		case 3:
			for (uint8_t i=8; i<16; i++) {
				if (keyboardStatus[i]) {
					updateSingleTone(SINE, notes4[i-8]);
					setMainToneColor(ledLight, mainColors[i-8]);
					setBeatColor(ledLight, mainColors[i-8]);
					setBackingTrackColor(ledLight, mainColors[i-8]);
					sendLedData(ledLight);

				}
			}
			break;
		case 4:
			for (uint8_t i=8; i<16; i++) {
				if (keyboardStatus[i]) {
					updateSingleTone(SINE, notes5[i-8]);
					setMainToneColor(ledLight, mainColors[i-8]);
					setBeatColor(ledLight, mainColors[i-8]);
					setBackingTrackColor(ledLight, mainColors[i-8]);
					sendLedData(ledLight);

				}
			}
			break;

	}

	if (pressed == 0) {
		updateSingleTone(NONE, E4);
		setMainToneColor(ledLight, OFF);
		setBeatColor(ledLight, OFF);
		setBackingTrackColor(ledLight, OFF);
		sendLedData(ledLight);

	}
}
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
  MX_TIM1_Init();
  MX_DAC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  //initialLedReset(ledLight);
  //setBrightness(45);

  //
  srand((unsigned) time(NULL));
  initToneSystem();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	KB_Detect_KeyPress();
	HAL_Delay(100);
	if (keyboardStatus[0]) {
		octave = 4;
		setOctave(octave);

	} else {
		octave = 3;
		setOctave(octave);
	}

    /* USER CODE END WHILE */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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

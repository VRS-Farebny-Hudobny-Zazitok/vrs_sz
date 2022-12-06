/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PWM_DATA_BUFF_SIZE 24
#define NUM_OF_LEDS 3
#define PWM_TOTAL_DATA_SIZE ((NUM_OF_LEDS * PWM_DATA_BUFF_SIZE) + 50)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//uint16_t pwm[(NUM_OF_LEDS * PWM_DATA_BUFF_SIZE) + 50];
//uint8_t LED_Data[NUM_OF_LEDS][4];
//int sendFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
////	sendFlag = 1;
//}

//void LED_setColor(int ledIndex, int red, int green, int blue)
//{
//	LED_Data[ledIndex][0] = ledIndex;
//	LED_Data[ledIndex][1] = green;
//	LED_Data[ledIndex][2] = red;
//	LED_Data[ledIndex][3] = blue;
//}
//
//void LED_send()
//{
//	uint32_t index = 0;
//	uint32_t color = 0;
//
//	for (int led = 0; led < NUM_OF_LEDS; led++)
//	{
//		color = ( (LED_Data[led][1] << 16) | (LED_Data[led][2] << 8) | LED_Data[led][3] );
//
//		for (int bit = PWM_DATA_BUFF_SIZE - 1; bit >= 0; bit--)
//		{
//			if (color & (1 << bit)) pwm[index] = 53;
//			else pwm[index] = 26;
//			index++;
//		}
//	}
//	for (int i = 0; i < 50; i++)
//	{
//		pwm[index] = 0;
//		index++;
//	}
//	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwm, index);
//	while(!sendFlag);
//}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAX_LED 1
#define USER_BRIGHTNESS 1
#define PI 3.14159265
#include <math.h>


uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];

int dataSendFlag = 1;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	dataSendFlag = 1;
}

void setLed(int ledNum, int red, int green, int blue)
{
	LED_Data[ledNum][0] = ledNum;
	LED_Data[ledNum][1] = green;
	LED_Data[ledNum][2] = red;
	LED_Data[ledNum][3] = blue;
}

void setBrightness(int brightness)
{
#if USER_BRIGHTNESS
	if (brightness > 45) brightness = 45;
	for (int i = 0; i < MAX_LED; i++)
	{
		LED_Mod[i][0] = LED_Data[i][0];
		for (int j = 1; j < 4; j++)
		{
			float angle = 90 - brightness;
			angle = angle * PI;
			LED_Mod[i][j] = (LED_Data[i][j]) / tan(angle);
//			LED_Mod[i][j] = LED_Data[i][j];
		}
	}
#endif
}

uint16_t pwmData[(24 * MAX_LED) + 50];

void send(void)
{
	uint32_t index = 0;
	uint32_t color = 0;

	for (int i = 0; i < MAX_LED; i++)
	{
		color = ((LED_Mod[i][1] <<  16) | (LED_Mod[i][2] << 8) | (LED_Mod[i][3]));

		for (int i = 23; i >= 0; i--)
		{
			if (color & (1 << i)) pwmData[index] = 53;
			else pwmData[index] = 27;
			index++;
		}
	}
	for (int i = 0; i < 50; i++)
	{
		pwmData[index] = 0;
		index++;
	}
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *) pwmData, index);
	while(!dataSendFlag);
	dataSendFlag = 0;
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
	MX_DMA_Init();
	MX_TIM1_Init();
	/* USER CODE BEGIN 2 */
	setLed(0, 255, 0, 0);
	setBrightness(45);
	send();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

	}
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
			| RCC_CLOCKTYPE_PCLK2;
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

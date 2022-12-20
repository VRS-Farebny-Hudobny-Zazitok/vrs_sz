/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void playUpperOctave(uint8_t octave);
void playLowerOctave();
void resetTonesAndFreqs(uint8_t index);
void setOctave(uint8_t octave);

uint8_t isPressed(uint8_t startIndex, uint8_t endIndex);
uint8_t didChange();
uint8_t processOctave(uint8_t octave);

extern uint8_t flagKeyboard;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define C3_Pin GPIO_PIN_0
#define C3_GPIO_Port GPIOB
#define C0_Pin GPIO_PIN_1
#define C0_GPIO_Port GPIOB
#define R0_Pin GPIO_PIN_8
#define R0_GPIO_Port GPIOA
#define R1_Pin GPIO_PIN_11
#define R1_GPIO_Port GPIOA
#define R3_Pin GPIO_PIN_4
#define R3_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_5
#define R2_GPIO_Port GPIOB
#define C1_Pin GPIO_PIN_6
#define C1_GPIO_Port GPIOB
#define C2_Pin GPIO_PIN_7
#define C2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

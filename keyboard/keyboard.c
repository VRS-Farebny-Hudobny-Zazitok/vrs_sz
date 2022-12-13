#include "keyboard.h"
uint8_t pressedKey = 0;

uint8_t detect_column(uint8_t column_number)
{

	resetKeyboardColumns();

	switch (column_number)
	{
		case 0:
			HAL_GPIO_WritePin(C0_GPIO_Port, C0_Pin, GPIO_PIN_SET);
			break;

		case 1:
			HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_SET);
			break;

		case 2:
			HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_SET);
			break;

		case 3:
			HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_SET);
			break;
	}

	if(HAL_GPIO_ReadPin(R0_GPIO_Port, R0_Pin) == GPIO_PIN_SET)
	{
		pressedKey = column_number + 1;
		return pressedKey;
	}

	if(HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin) == GPIO_PIN_SET)
	{
		pressedKey = column_number + 5;
		return pressedKey;
	}

	if(HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin) == GPIO_PIN_SET)
	{
		pressedKey = column_number + 9;
		return pressedKey;
	}

	if(HAL_GPIO_ReadPin(R3_GPIO_Port, R3_Pin) == GPIO_PIN_SET)
	{
		pressedKey = column_number + 13;
		return pressedKey;
	}

	return 0;
}

void resetKeyboardColumns(){
	HAL_GPIO_WritePin(C0_GPIO_Port, C0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_RESET);
}

uint8_t KB_Detect_KeyPress(void)
{
	uint8_t key_press = 0;
	for(uint8_t col = 0; col < 4; col++)
	{
		key_press = detect_column(col);
		resetKeyboardColumns();
//
//		if(key_press != 0)
//		{
//			return key_press;
//		}
	}

	return pressedKey;
}

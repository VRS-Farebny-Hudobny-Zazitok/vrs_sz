#include "keyboard.h"

uint8_t lPressedKey = 0;

uint32_t rowGpioPort[NUMBER_OF_ROWS] = {R0_GPIO_Port, R1_GPIO_Port, R2_GPIO_Port, R3_GPIO_Port};
uint32_t rowPin[NUMBER_OF_ROWS] = {R0_Pin, R1_Pin, R2_Pin, R3_Pin};

uint8_t detect_column(uint8_t column_number)
{

	resetKeyboardColumns();
	setKeyboardColumnPin(column_number);

	for(int i = 0; i < NUMBER_OF_ROWS; i++) {
		if (HAL_GPIO_ReadPin(*(rowGpioPort+i), *(rowPin+i)) == GPIO_PIN_SET) {
			updateKeyboardStatus(i*4 + column_number, 1);
		} else {
			updateKeyboardStatus(i*4 + column_number, 0);
		}
	}
//
//
//	if(HAL_GPIO_ReadPin(R0_GPIO_Port, R0_Pin) == GPIO_PIN_SET)
//	{
//		lPressedKey = column_number + 1;
//		return lPressedKey;
//	}
//
//	if(HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin) == GPIO_PIN_SET)
//	{
//		lPressedKey = column_number + 5;
//		return lPressedKey;
//	}
//
//	if(HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin) == GPIO_PIN_SET)
//	{
//		lPressedKey = column_number + 9;
//		return lPressedKey;
//	}
//
//	if(HAL_GPIO_ReadPin(R3_GPIO_Port, R3_Pin) == GPIO_PIN_SET)
//	{
//		lPressedKey = column_number + 13;
//		return lPressedKey;
//	}

	return 0;
}

void setKeyboardColumnPin(uint8_t column_number) {
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
}

void updateKeyboardStatus(uint8_t key, uint8_t newKeyState) {
	keyboardStatus[key] = newKeyState;
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
	}

	return lPressedKey;
}
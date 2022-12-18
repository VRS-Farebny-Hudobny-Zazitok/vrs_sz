#include "main.h"
#define NUMBER_OF_ROWS 4

uint8_t detect_column(uint8_t column_number);
uint8_t KB_Detect_KeyPress(void);

void resetKeyboardColumns();
void updateKeyboardStatus(uint8_t key, uint8_t newKeyState);
void setKeyboardColumnPin(uint8_t column_number);

extern uint8_t keyboardStatus[16];

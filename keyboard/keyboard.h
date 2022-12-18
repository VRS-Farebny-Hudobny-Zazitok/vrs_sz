#include "main.h"
#define NUMBER_OF_ROWS 4

void KB_Detect_KeyPress(void);
void resetKeyboardColumns();
void updateKeyboardStatus(uint8_t key, uint8_t newKeyState);
void setKeyboardColumnPin(uint8_t column_number);
void detect_column(uint8_t column_number);

extern uint8_t keyboardStatus[16];

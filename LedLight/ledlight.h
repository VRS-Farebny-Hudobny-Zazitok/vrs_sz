/*
 * ledlight.h
 *
 *  Created on: Dec 7, 2022
 *      Author: jakub
 */

#ifndef LEDLIGHT_H_
#define LEDLIGHT_H_

#include <math.h>
#include "main.h"

#define NUMBER_OF_LEDS 8

// LED number macros
#define L1 0
#define L2 7
#define L3 3
#define L4 6
#define L5 2
#define L6 5
#define L7 1
#define L8 4

static const uint8_t ledIterator[] = {L1, L2, L3, L4, L5, L6, L7, L8};

typedef struct sColor
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

void setLed(uint8_t lightIndex, Color *ledLight, Color color);
void setBrightness(uint8_t value);
void prepareSend(Color *ledLight);
void sendLedData(Color *ledLight);

#endif /* LEDLIGHT_H_ */

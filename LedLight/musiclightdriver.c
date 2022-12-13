/*
 * musiclightdriver.c
 *
 *  Created on: 9. 12. 2022
 *      Author: jakub
 */

// Header file implementation
#include "musiclightdriver.h"

void setMainToneColor(Color *ledLight, Color color)
{
	for (uint8_t i = 0; i < mainToneLedSetSize; i++)
	{
		setLed(mainToneLedSet[i], ledLight, color);
	}
}

void setBeatColor(Color *ledLight, Color color)
{
	for (uint8_t i = 0; i < beatLedSetSize; i++)
	{
		setLed(beatLedSet[i], ledLight, color);
	}
}

void setBackingTrackColor(Color *ledLight, Color color)
{
	for (uint8_t i = 0; i < backingTrackLedSetSize; i++)
	{
		setLed(backingTrackLedSet[i], ledLight, color);
	}
}

/*
 * musiclightdriver.h
 *
 *  Created on: 9. 12. 2022
 *      Author: jakub
 */

#ifndef MUSICLIGHTDRIVER_H_
#define MUSICLIGHTDRIVER_H_

#include "colors.h"
#include "ledlight.h"

static const uint8_t mainToneLedSet[] = {L3, L4, L5, L6};
static const uint8_t mainToneLedSetSize = 4;

static const uint8_t beatLedSet[] = {L2, L7};
static const uint8_t beatLedSetSize = 2;

static const uint8_t backingTrackLedSet[] = {L1, L8};
static const uint8_t backingTrackLedSetSize = 2;

void setMainToneColor(Color *ledLight, Color color);
void setBeatColor(Color *ledLight, Color color);
void setBackingTrackColor(Color *ledLight, Color color);

#endif /* MUSICLIGHTDRIVER_H_ */

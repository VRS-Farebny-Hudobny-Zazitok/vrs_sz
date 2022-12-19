/*
 * colors.h
 *
 *  Created on: Dec 8, 2022
 *      Author: jakub
 */

#include "ledlight.h"

#ifndef COLORS_H_
#define COLORS_H_

static const Color RED = {255, 0, 0};
static const Color GREEN = {0, 255, 0};
static const Color BLUE = {0, 0, 255};
static const Color MAGENTA = {255, 0, 255};
static const Color CYAN = {0, 255, 255};
static const Color YELLOW = {255, 255, 0};
static const Color WHITE = {255, 255, 255};
static const Color RANDOM = {99, 99, 163};
static const Color OFF = {0, 0, 0};

static const Color colorIterator[] = {RED, GREEN, BLUE, MAGENTA, CYAN, YELLOW};
static const Color mainColors[8] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, RANDOM};
#endif /* COLORS_H_ */

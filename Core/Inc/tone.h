
#ifndef TONE_H
#define TONE_H


#include "main.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "math.h"


#define MIN_NUM_SAMPLES 8
#define DMA_TIME_PERIOD 0.000250
#define MAX_DMA_LENGTH 255
enum TONE
{
  NONE = 1,
  SAW,
  TRIANGLE,
  SQUARE,
  SINE,
  NOISE
};


void initToneSystem();

void updateTone(enum TONE tone1, double freq1,
		        enum TONE tone2, double freq2,
		        enum TONE tone3, double freq3,
	       	    enum TONE tone4, double freq4);

void updateSingleTone(enum TONE tone1, double freq);

void generateSine(uint8_t *buffer, uint16_t length, uint16_t period);

void generateSaw(uint8_t *buffer, uint16_t length, uint16_t period);

void generateSquare(uint8_t *buffer, uint16_t length, uint16_t period);

void generateTriangle(uint8_t *buffer, uint16_t length, uint16_t period);

void generateNoise(uint8_t *buffer, uint16_t length, uint16_t period);

void generateNone(uint8_t *buffer, uint16_t length, uint16_t period);

#endif

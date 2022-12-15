
#ifndef TONE_H
#define TONE_H


#include "main.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "math.h"
#include <stdlib.h>


//#define DMA_TIME_PERIOD 0.000250
#define DMA_TIME_PERIOD 0.000125
#define MAX_DMA_LENGTH 340
#define MAX_NUM_TONES 4

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

void updateMultipleTone(enum TONE tone[], double freq[]);

void updateSingleTone(enum TONE tone1, double freq);

void generateSine(uint8_t *buffer, uint16_t amount, uint16_t period);

void generateSaw(uint8_t *buffer, uint16_t amount, uint16_t period);

void generateSquare(uint8_t *buffer, uint16_t amount, uint16_t period);

void generateTriangle(uint8_t *buffer, uint16_t amount, uint16_t period);

void generateNoise(uint8_t *buffer, uint16_t amount, uint16_t period);

void generateNone(uint8_t *buffer, uint16_t amount, uint16_t period);

double AWGN_generator();

#endif

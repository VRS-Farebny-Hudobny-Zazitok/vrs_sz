/*
 * tone.c
 *
 *  Created on: 13. 12. 2022
 *      Author: micha
 */
#include "tone.h"

void initToneSystem()
{
    //launching DAC
    HAL_TIM_Base_Start(&htim3);
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
}

///@brief gets frequency in hertz, returns signal period in amount of dma_time_period
uint16_t freqToPeriod(double freq)
{
    double t = 1/freq;
    return ((uint16_t) (t/DMA_TIME_PERIOD));
}


/*void updateTone(TONE tone1, double freq1,
		        TONE tone2, double freq2,
				TONE tone3, double freq3,
				TONE tone4, double freq4)
{
    uint8_t amount = 0;
    uint8_t temp_buffer[MAX_DMA_LENGTH];



}
*/

void updateSingleTone(enum TONE tone1, double freq)
{

	uint8_t temp_buffer[MAX_DMA_LENGTH];

	uint16_t period = freqToPeriod(freq);
	switch (tone1)
	{
	    case NONE:  generateNone(temp_buffer, 1, period);break;
	    case SAW:  generateSaw(temp_buffer, 1, period);break;
	    case SINE:  generateSine(temp_buffer, 1, period);break;
	    case TRIANGLE: generateTriangle(temp_buffer, 1, period);break;
	}
	//generateSaw(temp_buffer, 1, period);


    HAL_DAC_Stop_DMA (&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)temp_buffer, period, DAC_ALIGN_8B_R);

}

///@brief Generate sawtooth signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
/// AMOUNT NOT YET IMPLEMENTED
///@param period - amount of dma_time_periods in one signal period, amount of samples per period
void generateSaw(uint8_t *buffer, uint16_t amount, uint16_t period)
{
  buffer[0] = 1;

  double slope_increment;

  slope_increment = 254 / (period-1);

  //TOTO REVERZNI ABY SIEL OD 255 DO NULY

  for (int i = 1; i<period; i++)
  {
    buffer[i] = 1 + (uint8_t) (i*slope_increment);
  }

}

///@brief Generate sawtooth signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
/// AMOUNT NOT YET IMPLEMENTED
///@param period - amount of dma_time_periods in one signal period, amount of samples per period
void generateTriangle(uint8_t *buffer, uint16_t amount, uint16_t period)
{
  uint8_t offset;
  uint8_t mid;

  if (period % 2 ==0)
  {//even number
	  offset = 0;
	  mid = period/2;
  }
  else
  {//odd number
	 offset = 1;
	 mid = (period-1)/2;
  }


  buffer[0] = 1;

  buffer[mid] = 255;


  double slope_increment;

  slope_increment = 254 / mid;



  for (int i = mid; i>1; i--)
  {
    buffer[i] = 255 - (uint8_t) (i*slope_increment);
  }

  for (int i = mid, j = mid; i>1; i--,j++)
  {
    buffer[j] = 255 - (uint8_t) (i*slope_increment);
  }

}


///@brief Generate sine signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
///@param period - amount of dma_time_periods in one signal period, amount of samples per period
void generateSine(uint8_t *buffer, uint16_t amount, uint16_t period)
{
  uint8_t median = 128;
  uint8_t amplitude = 127;

  buffer[0] = median;

  double slope_increment;

  slope_increment = 6.28 / (period-1);


  for (int i = 1; i<period; i++)
  {
    buffer[i] = median + (int16_t) (amplitude *  sin (i*slope_increment));
  }

}


///@brief Generate NONE signal into buffer, fills buffer with zeros
/// other params than buffer are not important right now
///@param buffer - return value from function, generated signal
void generateNone(uint8_t *buffer, uint16_t amount, uint16_t period)
{
  buffer[0] = 0;


  for (int i = 1; i<period; i++)
  {
    buffer[i] = 0;
  }

}

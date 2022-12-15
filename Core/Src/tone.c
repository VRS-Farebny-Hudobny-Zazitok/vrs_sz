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

///
void updateMultipleTone(enum TONE tone[], double freq[])
{

    uint8_t tone_amount = 0;

    for (int i = 0; i<MAX_NUM_TONES; i++)
    {
    	if (tone[i]!=NONE)
    	    	tone_amount++;
    }


    //PRIROB DEFAULT AK SU VSETKY NULA

    uint8_t amount[MAX_NUM_TONES];
    uint16_t period[MAX_NUM_TONES];

    for (int i = 0; i<tone_amount; i++)
    {
    	period[i] = freqToPeriod(freq[i]);
    	amount[i] = (uint8_t) (MAX_DMA_LENGTH/period[i]);
    }

    uint16_t buf_length = amount[0]*period[0];

    for (int i = 1; i<tone_amount; i++)
    {
    	if (amount[i]*period[i]<buf_length)
    		buf_length = amount[i]*period[i];
    }


    uint8_t temp_buffer[MAX_DMA_LENGTH];
    uint8_t return_buffer[MAX_DMA_LENGTH];

    for (int i = 0; i<MAX_DMA_LENGTH; i++)
    {
    	return_buffer[i] = 0;
    }

    for (int i = 0; i<tone_amount; i++)
    {
    	switch (tone[i])
    		{
    		    case NONE:  generateNone(temp_buffer, amount[i], period[i]);break;
    		    case SAW:  generateSaw(temp_buffer, amount[i], period[i]);break;
    		    case SINE:  generateSine(temp_buffer, amount[i], period[i]);break;
    		    case TRIANGLE: generateTriangle(temp_buffer, amount[i], period[i]);break;
    		    case SQUARE: generateSquare(temp_buffer, amount[i], period[i]);break;
    		    case NOISE: generateNoise(temp_buffer, amount[i], period[i]);break;
    		}


    	 for (int j = 0; j<MAX_DMA_LENGTH; j++)
    	    {
    	    	return_buffer[j] = return_buffer[j] + temp_buffer[j]/tone_amount;
    	    }

    }

    HAL_DAC_Stop_DMA (&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)return_buffer, buf_length, DAC_ALIGN_8B_R);


}


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
	    case SQUARE: generateSquare(temp_buffer, 1, period);break;
	    case NOISE: generateNoise(temp_buffer, 1, period);break;
	}

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


  double slope_increment;

  slope_increment = 254.0 / (double)(period-1);


  for (int k = 0; k<amount*period;k=k+period)
  {
	  buffer[k] = 1;

      for (int i = k+period-1, j = 0; i>k; i--,j++)
      {
          buffer[i] = 255 - (uint8_t) (j*slope_increment);
      }
  }
}


///@brief Generate noise signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
/// AMOUNT NOT YET IMPLEMENTED
///@param period - amount of dma_time_periods in one signal period, amount of samples per period
void generateNoise(uint8_t *buffer, uint16_t amount, uint16_t period)
{


  for (int i = 0; i<period; i++)
  {
    buffer[i] =  (uint8_t) ((AWGN_generator()*127.0)+128);
  }

}

///@brief Generate triangle signal into buffer
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

  slope_increment = 254.0 / (double)mid;



  for (int i = mid-1, j = 1; i>=1; i--,j++)
  {
    buffer[i] = 255 - (uint8_t) (j*slope_increment);
  }

  for (int i = mid+1, j = 1; i<period; i++,j++)
  {
    buffer[i] = 255 - (uint8_t) (j*slope_increment);
  }

  if (offset ==1)
  buffer[period-1] =  1;

}


///@brief Generate square signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
/// AMOUNT NOT YET IMPLEMENTED
///@param period - amount of dma_time_periods in one signal period, amount of samples per period
void generateSquare(uint8_t *buffer, uint16_t amount, uint16_t period)
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
	 mid = (period)/2+offset;
  }


  for (int i = 0; i<mid; i++ )
   {
       buffer[i] = 255;
   }

   for (int i = mid; i<period; i++ )
   {
       buffer[i] = 1;
   }

  if (offset ==1)
  buffer[mid-1] =  128;

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

  slope_increment = 6.28 / (double)(period-1);


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

//credit: Cagri Tanriover
double AWGN_generator()
{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

  double temp1;
  double temp2;
  double result;
  int p;
  p = 1;
  while( p > 0 )
  {
	temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
                                                       integer between 0 and  RAND_MAX,
                                                       which is defined in stdlib.h.
                                                   */
    if ( temp2 == 0 )
    {// temp2 is >= (RAND_MAX / 2)
      p = 1;
    }// end if
    else
    {// temp2 is < (RAND_MAX / 2)
       p = -1;
    }// end else

  }// end while()

  temp1 = cos( ( 2.0 * (double)3.14 ) * rand() / ( (double)RAND_MAX ) );
  result = sqrt( -2.0 * log( temp2 ) ) * temp1;

  return result;	// return the generated random sample to the caller
}

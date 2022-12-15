/*
 * tone.c
 *
 *  Created on: 13. 12. 2022
 *      Author: michal
 */
#include "tone.h"

///@brief run this before playing any tones
/// launching DAC
void initToneSystem()
{
    HAL_TIM_Base_Start(&htim3);
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
}

///@brief gets frequency in hertz, returns signal period in amount of dma_time_period
uint16_t freqToPeriod(double freq)
{
    double t = 1/freq;
    return ((uint16_t) (t/DMA_TIME_PERIOD));
}

///@brief mixes up to four signals and plays the resulting tone
/// Multi tone API
///@param tone[] Array of TONE with the length of 4,
/// Can have from 0 to 4 tones.
/// Array starts with tones that are other than NONE, the rest (unused channels) are NONE
/// If all are NONE, then the function call single tone API with NONE parameter
///@param freq[] Array of tone frequencies corresponding to tone[].
/// Must have the same length as tone[]
/// values corresponding to NONE tones are ignored
void updateMultipleTone(enum TONE tone[], double freq[])
{

    uint8_t tone_amount = 0;

    //determine amount of non-NONE tones
    for (int i = 0; i<MAX_NUM_TONES; i++)
    {
    	if (tone[i]!=NONE)
    	    	tone_amount++;
    }

    //in case all are NONE
    if (tone_amount == 0)
    {
    	updateSingleTone(NONE,440);
        return;
    }


    uint8_t amount[MAX_NUM_TONES];
    uint16_t period[MAX_NUM_TONES];

    for (int i = 0; i<tone_amount; i++)
    {
    	period[i] = freqToPeriod(freq[i]);
    	amount[i] = (uint8_t) (MAX_DMA_LENGTH/period[i]);
    }

    //in case we find LCM then everything is fine
    uint16_t buf_length;
    buf_length= LCM_generator(period,tone_amount);

    if (buf_length==0)
    {
    	//no LCM, some signals are going to be trimmed/have deaf areas
		buf_length = amount[0]*period[0];

		for (int i = 1; i<tone_amount; i++)
		{
			if (amount[i]*period[i]>buf_length) //slightly better sound when it's '>' (deaf areas option) ('<' is trim option)
				buf_length = amount[i]*period[i];
		}
    }
    else
    {
    	//LCM found
        for (int i = 0; i<tone_amount; i++)
        {
        	amount[i] = (uint8_t) (buf_length/period[i]);
        }

    }

    //now we have parameters to generate tones - types, their periods and amount of eriods generated

    uint8_t temp_buffer[MAX_DMA_LENGTH];
    uint8_t return_buffer[MAX_DMA_LENGTH];


    for (int i = 0; i<MAX_DMA_LENGTH; i++)
    {
    	return_buffer[i] = 0;//this line is the important one, don't delete it
    	temp_buffer[i] = 0;// in case of uncommenting that for loop with temp_buffer[i], comment out this
    }

    //generating tones
    for (int i = 0; i<tone_amount; i++)
    {
       /*this is commented out for a reason
        * for (int i = 0; i<MAX_DMA_LENGTH; i++)
        {
        	temp_buffer[i] = 0;

        }*/
    	switch (tone[i])
    		{
    		    case NONE:  generateNone(temp_buffer, amount[i], period[i]);break;
    		    case SAW:  generateSaw(temp_buffer, amount[i], period[i]);break;
    		    case SINE:  generateSine(temp_buffer, amount[i], period[i]);break;
    		    case TRIANGLE: generateTriangle(temp_buffer, amount[i], period[i]);break;
    		    case SQUARE: generateSquare(temp_buffer, amount[i], period[i]);break;
    		    case NOISE: generateNoise(temp_buffer, amount[i], period[i]);break;
    		}

         //summing signals
    	 for (int j = 0; j<buf_length; j++)
    	    {
    	    	return_buffer[j] = return_buffer[j] + temp_buffer[j]/tone_amount;
    	    }

    }

    HAL_DAC_Stop_DMA (&hdac1, DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)return_buffer, buf_length, DAC_ALIGN_8B_R);


}

///@brief Plays tone composed of a single signal
///Single tone API
///@param tone1 type of signal to play
///@param freq frequency of tone in Hz
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
///@param period - amount of dma_time_periods in one signal period, amount of samples per period
///period and amount only determine the length of samples generated
void generateNoise(uint8_t *buffer, uint16_t amount, uint16_t period)
{


  for (int i = 0; i<(period*amount); i++)
  {
    buffer[i] =  (uint8_t) ((AWGN_generator()*100)+100);
  }

}

///@brief Generate triangle signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
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


  double slope_increment;
  slope_increment = 254.0 / (double)mid;


  for (int k = 0; k<amount*period;k=k+period)
  {

	buffer[k] = 1;

	buffer[k+mid] = 255;

	for (int i = k+mid-1, j = 1; i>=k+1; i--,j++)
	{
	buffer[i] = 255 - (uint8_t) (j*slope_increment);
	}

	for (int i = k+mid+1, j = 1; i<k+period; i++,j++)
	{
	buffer[i] = 255 - (uint8_t) (j*slope_increment);
	}

	if (offset ==1)
	buffer[k+period-1] =  1;

  }
}


///@brief Generate square signal into buffer
///@param buffer - return value from function, generated signal
///@param amount - by default should  be one, the function returns amount periods of signal
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

  for (int k = 0; k<amount*period;k=k+period)
  {
	  for (int i = k; i<(k+mid); i++ )
	   {
		   buffer[i] = 255;
	   }

	   for (int i = k+mid; i<(k+period); i++ )
	   {
		   buffer[i] = 1;
	   }

		if (offset ==1)
		buffer[k+mid-1] =  128;
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



  double slope_increment;

  slope_increment = 6.28 / (double)(period-1);

  for (int k = 0; k<amount*period;k=k+period)
  {
	  buffer[k] = median;
  for (int i = 1; i<period; i++)
  {
	buffer[k+i] = median + (int16_t) (amplitude *  sin (i*slope_increment));
  }
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

///@brief generates Least Common Multiple of nums
///@return LCM in case of success, 0 in case of failure
///credit: https://www.programiz.com/c-programming/examples/lcm
uint16_t LCM_generator(uint16_t nums[], uint8_t length)
{
	uint16_t max = nums[0];
  for (int i = 1; i<length;i++)
  {
	  if (nums[i]>max)
		  max = nums[i];
  }

  while(max<MAX_DMA_LENGTH)
  {
	  uint8_t flag = 1;
	  for (int i = 0; i<length;i++)
	  {
		  if (!(max % nums[i]==0))
		  {
		    flag = 0;
		    break;
		  }
	  }

	  if (flag == 0)
	  {
		  max ++;
	     continue;
	  }
	  else
	  {
		 return max;
	  }
  }
  return (0);
}


///@brief Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1.
///credit: Cagri Tanriover, https://www.embeddedrelated.com/showcode/311.php
double AWGN_generator()
{
  double temp1;
  double temp2;
  double result;
  int p;
  p = 1;
  while( p > 0 )
  {
	temp2 = ( rand() / ( (double)RAND_MAX ) );
    if ( temp2 == 0 )
    {
      p = 1;
    }
    else
    {
       p = -1;
    }
  }

  temp1 = cos( ( 2.0 * (double)3.14 ) * rand() / ( (double)RAND_MAX ) );
  result = sqrt( -2.0 * log( temp2 ) ) * temp1;

  return result;
}

#include "ledlight.h"

#include <math.h>
#include "tim.h"
#include "dma.h"
#include "colors.h"
#include "musiclightdriver.h"

// Header implementation
uint16_t pwm[(24 * NUMBER_OF_LEDS) + 50]; //24*8 + 50 is a reset sequence 2/3 LOW 800 kHz 1 perioda => 1/800000 => 1.25 ms datasheet
uint8_t flagDataSend = 0;
uint8_t brightness = 45; // toto je z internetov tan(45) = 1 => 100%

// DMA Handler
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	flagDataSend = 1;
}

//
void setBrightness(uint8_t value)
{
	if (value > 45) brightness = 45;
	else brightness = value;
}

void sendLedData(Color *ledLight)
{
	uint8_t index = 0;
	uint32_t color = 0;
	for (uint8_t iLed = 0; iLed < NUMBER_OF_LEDS; iLed++)
	{
		color = ((ledLight + iLed)->green << 16)
				| ((ledLight + iLed)->red << 8)
				| ((ledLight + iLed)->blue);

		for (int8_t iBit = 23; iBit >= 0; iBit--)
		{
			if (color & (1 << iBit)) pwm[index] = 53; // 2/3 of 80 80-> je cislo do ktoreho rata counter Period register log 1
			else pwm[index] = 27; // 1/3 of 80 -> log 0
			index++;
		}
	}
	// Reset signal
	for (uint8_t iBit = 0; iBit < 50; iBit++)
	{
		pwm[index] = 0;
		index++;
	}
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t *)pwm, index);
	while(!flagDataSend){};
	flagDataSend = 0;
}

void setLed(uint8_t lightIndex, Color *ledLight, Color color)
{
	float angle = 90 - brightness; // Angle in degrees
	angle *= (M_PI / 180); // Angle in radians
	(ledLight + lightIndex)->red = color.red / tan(angle);
	(ledLight + lightIndex)->green = color.green / tan(angle);
	(ledLight + lightIndex)->blue = color.blue / tan(angle);
}

void initialLedReset(Color *ledLight){
	  setMainToneColor(ledLight, OFF);
	  setBeatColor(ledLight, OFF);
	  setBackingTrackColor(ledLight, OFF);

	  for (int i = 0; i < 100; i++) {
		 sendLedData(ledLight);
	  }
}

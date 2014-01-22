/*
 * brakelight.c
 *
 *  Created on: Jan 22, 2014
 *      Author: jeremybutler
 */

#include "brakelight.h"

void initializeBrakelight(Brakelight * brakeLight, PWMled * led1, PWMled * led2,
		PWMled * led3, PWMled * led4, PWMled * led5, PWMled * led6,
		Accelerometer * accelerometer)
{
	brakeLight->led1 = led1;
	brakeLight->led2 = led2;
	brakeLight->led3 = led3;
	brakeLight->led4 = led4;
	brakeLight->led5 = led5;
	brakeLight->led6 = led6;
	brakeLight->accelerometer = accelerometer;
}

void initializeAccelerometer(Accelerometer * accelerometer)
{
	//TODO implement method
	return;
}

void initializeLED(PWMled * led, volatile uint8_t * dutyCycleReg)
{
	led->dutyCycleReg = dutyCycleReg;
}

// Convenience function for turning off LED
void ledOff(PWMled * led)
{
	*(led->dutyCycleReg) = 0;
}

// Convenience function for turning on LED to maximum brightness
void ledOn(PWMled * led)
{
	*(led->dutyCycleReg) = MAX_DUTY_CYCLE;
}

void setLedBrightness(PWMled * led, const uint8_t brightness)
{
	*(led->dutyCycleReg) = brightness;
}

/*
 * pwmled.c
 *
 *  Created on: Jan 25, 2014
 *      Author: jeremybutler
 */

#include "pwmled.h"

void pwmled_initialize(PWMled_t * led, volatile uint8_t * dutyCycleReg)
{
	led->dutyCycleReg = dutyCycleReg;
}

// Convenience function for turning off LED
void pwmled_off(PWMled_t * led)
{
	*(led->dutyCycleReg) = 0;
}

// Convenience function for turning on LED to maximum brightness
void pwmled_on(PWMled_t * led)
{
	*(led->dutyCycleReg) = MAX_DUTY_CYCLE;
}

void pwmled_setBrightness(PWMled_t * led, const uint8_t brightness)
{
	*(led->dutyCycleReg) = brightness;
}

/*
 * brakelight.h
 *
 *  Created on: Jan 22, 2014
 *      Author: jeremybutler
 */

#ifndef BRAKELIGHT_H_
#define BRAKELIGHT_H_

#include <inttypes.h>

#define MAX_DUTY_CYCLE 255

typedef struct Accelerometer
{
	//TODO figure out what to really put in here
	uint8_t placeholder;
} Accelerometer;

//This typedef is for an LED that has been put onto a PWM line
typedef struct PWMled
{
	volatile uint8_t *dutyCycleReg;
} PWMled;

typedef struct Brakelight
{
	PWMled * led1;
	PWMled * led2;
	PWMled * led3;
	PWMled * led4;
	PWMled * led5;
	PWMled * led6;
	Accelerometer * accelerometer;
} Brakelight;

void initializeBrakelight(Brakelight * brakeLight, PWMled * led1, PWMled * led2,
		PWMled * led3, PWMled * led4, PWMled * led5, PWMled * led6,
		Accelerometer * accelerometer);

void initializeLED(PWMled * led, volatile uint8_t * dutyCycleReg);

void initializeAccelerometer(Accelerometer * accelerometer);

void ledOff(PWMled * led);

void ledOn(PWMled * led);

void setLedBrightness(PWMled * led, const uint8_t brightness);

#endif /* BRAKELIGHT_H_ */

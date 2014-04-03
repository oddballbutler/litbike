/*
 * brakelight.h
 *
 *  Created on: Jan 22, 2014
 *      Author: jeremybutler
 *
 * This is code for a bicycle brake light. It is intended to be used with AVR
 * devices that have at least 2 hardware PWM channels and a hardware I2C(TWI)
 * module.
 */

#ifndef BRAKELIGHT_H_
#define BRAKELIGHT_H_

#include "adxl345.h"
#include "pwmled.h"

#include <avr/io.h>
#include <inttypes.h>
#include <stdbool.h>

#define BRAKE_BRIGHTNESS 255 // LED Brightness for braking
#define IDLE_BRIGHTNESS 40 // LED Brightness when not braking

// Registers to set the duty cycle of each PWM pin
#define LED1_DUTY_CYCLE_REG OCR0A
#define LED2_DUTY_CYCLE_REG OCR0B

typedef enum
{
	BRAKELIGHT_SLEEP,
	BRAKELIGHT_IDLE,
	BRAKELIGHT_BRAKE
}Brakelight_state_t;

/**
 * The brake light object.
 * This should only be instantiated once.
 */
typedef struct Brakelight
{
	PWMled_t led1;
	PWMled_t led2;
	ADXL345_t accelerometer;
	Brakelight_state_t state;
	double xAccel;
	double yAccel;
	double zAccel;
	double xAccelPast;
	double yAccelPast;
	double zAccelPast;

//	int16_t xAccelBuf[BRAKELIGHT_BUFSIZE];
//	int16_t yAccelBuf[BRAKELIGHT_BUFSIZE];
//	int16_t zAccelBuf[BRAKELIGHT_BUFSIZE];
//	uint8_t accelBufIndex;
//	int16_t xAccelSum;
//	int16_t yAccelSum;
//	int16_t zAccelSum;

} Brakelight_t;

void brakelight_initialize(Brakelight_t * brakelight);
void brakelight_run(Brakelight_t * brakelight);
bool brakelight_updateAcceleration(Brakelight_t * brakelight);
void brakelight_updateLights(Brakelight_t * brakelight);

#endif /* BRAKELIGHT_H_ */

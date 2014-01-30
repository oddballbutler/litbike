/*
 * brakelight.h
 *
 *  Created on: Jan 22, 2014
 *      Author: jeremybutler
 */

#ifndef BRAKELIGHT_H_
#define BRAKELIGHT_H_

#include <avr/io.h>
#include <inttypes.h>

#include "adxl345.h"
#include "pwmled.h"

// Registers to set the duty cycle of each PWM pin
#define LED1_DUTY_CYCLE_REG OCR0A
#define LED2_DUTY_CYCLE_REG OCR0B
#define LED3_DUTY_CYCLE_REG OCR1AL
#define LED4_DUTY_CYCLE_REG OCR1BL
#define LED5_DUTY_CYCLE_REG OCR2A
#define LED6_DUTY_CYCLE_REG OCR2B

typedef struct Brakelight
{
	PWMled_t * led1;
	PWMled_t * led2;
	PWMled_t * led3;
	PWMled_t * led4;
	PWMled_t * led5;
	PWMled_t * led6;
	ADXL345_t * accelerometer;
} Brakelight_t;

void brakelight_initialize(Brakelight_t * brakeLight, PWMled_t * led1, PWMled_t * led2,
		PWMled_t * led3, PWMled_t * led4, PWMled_t * led5, PWMled_t * led6,
		ADXL345_t * accelerometer);



#endif /* BRAKELIGHT_H_ */

/*
 * pwmled.h
 *
 *  Created on: Jan 25, 2014
 *      Author: jeremybutler
 */

#ifndef PWMLED_H_
#define PWMLED_H_

#include <inttypes.h>

#define MAX_DUTY_CYCLE 255

typedef struct PWMled
{
	volatile uint8_t *dutyCycleReg;
} PWMled_t;

void pwmled_initialize(PWMled_t * led, volatile uint8_t * dutyCycleReg);

void pwmled_off(PWMled_t * led);

void pwmled_on(PWMled_t * led);

void pwmled_setBrightness(PWMled_t * led, const uint8_t brightness);

#endif /* PWMLED_H_ */

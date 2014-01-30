/*
 * brakelight.c
 *
 *  Created on: Jan 22, 2014
 *      Author: jeremybutler
 */

#include "brakelight.h"

void brakelight_initialize(Brakelight_t * brakeLight, PWMled_t * led1, PWMled_t * led2,
		PWMled_t * led3, PWMled_t * led4, PWMled_t * led5, PWMled_t * led6,
		ADXL345_t * accelerometer)
{
	brakeLight->led1 = led1;
	brakeLight->led2 = led2;
	brakeLight->led3 = led3;
	brakeLight->led4 = led4;
	brakeLight->led5 = led5;
	brakeLight->led6 = led6;
	brakeLight->accelerometer = accelerometer;

	pwmled_initialize(led1, &LED1_DUTY_CYCLE_REG);
	pwmled_initialize(led2, &LED2_DUTY_CYCLE_REG);
	pwmled_initialize(led3, &LED3_DUTY_CYCLE_REG);
	pwmled_initialize(led4, &LED4_DUTY_CYCLE_REG);
	pwmled_initialize(led5, &LED5_DUTY_CYCLE_REG);
	pwmled_initialize(led6, &LED6_DUTY_CYCLE_REG);
	adxl345_initialize(accelerometer, 0);
}


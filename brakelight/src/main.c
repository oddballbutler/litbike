/*
 * main.c
 *
 *  Created on: Jan 17, 2014
 *      Author: jeremybutler
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "TWI_Master.h"

#include "brakelight.h"

void setup(void)
{
	// SETUP PORT DIRECTIONS
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;

	// SETUP PWM
	// Set to Fast PWM
	// Set compare output to non-inverting mode
	// Set timer clock selection to be the I/O clock without prescaling
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS10);
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	TCCR2B = _BV(CS20);

	// SETUP I2C
	TWI_Master_Initialise();

}

int main(void)
{
	setup();

	PWMled_t Led1;
	PWMled_t Led2;
	PWMled_t Led3;
	PWMled_t Led4;
	PWMled_t Led5;
	PWMled_t Led6;
	ADXL345_t Accelerometer1;
	Brakelight_t Brakelight1;

	brakelight_initialize(&Brakelight1, &Led1, &Led2, &Led3, &Led4, &Led5, &Led6,
			&Accelerometer1);

	uint8_t i = 0;
	uint8_t up = 1;
	sei();
	while (1)
	{
		//Pulse all LEDs
		pwmled_setBrightness(&Led1, i);
		pwmled_setBrightness(&Led2, i);
		pwmled_setBrightness(&Led3, i);
		pwmled_setBrightness(&Led4, i);
		pwmled_setBrightness(&Led5, i);
		pwmled_setBrightness(&Led6, i);

		up ? i++ : i--;
		if (i == 255)
		{
			up = 0;
		}
		else if (i == 0)
		{
			up = 1;
		}
		_delay_ms(10);
	}

	return (0);
}

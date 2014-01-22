/*
 * main.c
 *
 *  Created on: Jan 17, 2014
 *      Author: jeremybutler
 */

#include <avr/io.h>
//#include <avr/interrupt.h>

#include "brakelight.h"

#define LED1_DUTY_CYCLE_REG OCR0A
#define LED2_DUTY_CYCLE_REG OCR0B
#define LED3_DUTY_CYCLE_REG OCR1AL
#define LED4_DUTY_CYCLE_REG OCR1BL
#define LED5_DUTY_CYCLE_REG OCR2AL
#define LED6_DUTY_CYCLE_REG OCR2BL

PWMled Led1;
PWMled Led2;
PWMled Led3;
PWMled Led4;
PWMled Led5;
PWMled Led6;
Accelerometer Accelerometer1;
Brakelight Brakelight1;

void setup(void)
{
	// SETUP PORT DIRECTIONS
	DDRA = 0xFF;
	DDRB = 0xFF;

	// SETUP PWM
	// Set to Fast PWM
	// Set compare output to non-inverting mode
	// Set timer clock selection to be the I/O clock without prescaling
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS10);
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
	TCCR2B = _BV(WGM22) | _BV(CS20);

	// SETUP INTERRUPTS

}

int main(void)
{
	setup();

	initializeLED(&Led1, &LED1_DUTY_CYCLE_REG);
	initializeLED(&Led2, &LED2_DUTY_CYCLE_REG);
	initializeLED(&Led3, &LED3_DUTY_CYCLE_REG);
	initializeLED(&Led4, &LED4_DUTY_CYCLE_REG);
	initializeLED(&Led5, &LED5_DUTY_CYCLE_REG);
	initializeLED(&Led6, &LED6_DUTY_CYCLE_REG);
	initializeBrakelight(&Brakelight1, &Led1, &Led2, &Led3, &Led4, &Led5, &Led6,
			&Accelerometer1);

	uint8_t i = 0;
	uint8_t up = 1;
	while (1)
	{
		//Pulse all LEDs
		setLedBrightness(&Led1, i);
		setLedBrightness(&Led2, i);
		setLedBrightness(&Led3, i);
		setLedBrightness(&Led4, i);
		setLedBrightness(&Led5, i);
		setLedBrightness(&Led6, i);

		up ? i++ : i--;
		if (i == 255)
		{
			up = 0;
		}
		else if (i == 0)
		{
			up = 1;
		}
	}

	return 0;
}

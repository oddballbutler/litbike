/*
 * main.c
 *
 *  Created on: Jan 17, 2014
 *      Author: jeremybutler
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void setup(void)
{
	// SETUP PORT DIRECTIONS
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;

	// SETUP PWM
	// Set to Fast PWM
	// Set compare output to non-inverting mode
	// Set timer clock selection to be the I/O clock without prescaling
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS10);
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
	TCCR2B = _BV(CS20);


	// SETUP INTERRUPTS

}

int main(void)
{
	setup();

	//Set PWMs to %50 duty cycle
	OCR0A = 0x7F;
	OCR0B = 0x7F;
	OCR1AL = 0x7F;
	OCR1BL = 0x7F;
	OCR2A = 0x7F;
	OCR2B = 0x7F;

	while(1);

	return 0;
}

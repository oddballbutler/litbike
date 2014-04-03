/*
 * blink.c
 *
 *  Created on: Feb 5, 2014
 *      Author: jeremybutler
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0xFF;
	PORTD = _BV(PORTD6);
	while(1)
	{
		PORTD ^= _BV(PORTD6)|_BV(PORTD5);
		_delay_ms(11);
	}
	return(0);
}

/*
 * leds.c
 *
 * Created: 9/22/2025 4:34:29 PM
 *  Author: josel
 */ 
#include <avr/io.h>
#include <util/delay.h>

void led_on_off()
{
	PORTB ^= 1 << PB5;
	_delay_ms(500);
}
void led_on_off_fast()
{
	for(int x = 0; x < 20; x++)
	{
		PORTB ^= 1 << PB5;
		_delay_ms(100);	
	}
	
}
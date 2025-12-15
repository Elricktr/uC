
#include <avr/io.h>
void init_ports(void)
{
	
	// bomba de agua
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);  //apagada
	
	//runninfg
	DDRB |= (1 << PB5);
	
	
	//  LDR
	DDRC &= ~(1 << PC0);
	PORTC &= ~(1 << PC0);  
	
	
	// INT0
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);  
	
	
}
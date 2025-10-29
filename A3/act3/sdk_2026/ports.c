/*
 * ports.c
 *
 * Created: 9/22/2025 4:32:22 PM
 *  Author: josel
 */
#include <avr/io.h>
void init_ports(void)
{
	//configura PB5 como salida
	//DDRB |= 1 << PB5;
	//configura PB1 como salida para OC1A
	//DDRB |= 1 << PB1;
	//configura PB3 como salida para OC2A
	//DDRB |= 1 << PB3;
	
	//configura PD2 como entrada
	//DDRD &= ~(1 << PD2);
	//PORTD |= (1 << PD2);//activa Rp
	//uso temporal del puerto B para LCD
	//DDRB = 0xff; //PB4 a PB7 para datos
	
	//configura PB1 como salida para OC1A 
	DDRB |= (1 << PB1);
	//configura PB2 como salida para OC1B 
	DDRB |= (1 << PB2);
	
	
}
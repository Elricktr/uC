/*
 * Timers.c
 *
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "control.h"
//timer0
void timer0_normalmode_noprescaler(void)
{
	//configure timer
	TCCR0B |= (1 << CS00); //no prescaler
	TCNT0 = 0; // start count from 0
	int contador = 0;
	while(1)
	{
		if (TCNT0 >= 200)
		{
			contador ++;
			if (contador >= 160)
			{
				PORTB ^= 1<<PB5; // PB5 --> output
				contador = 0;
			}
			TCNT0 = 0;
		}
	}
}
void timer0_normalmode_prescaler(void)
{	
	//configure timer
	TCCR0B |= (1 << CS00) | (1 << CS02); //prescaler = 1024
	TCNT0 = 0; // start count from 0
	/*
	fosc = 1/16,000,000 = 0.0625 us
	with prescaler:
	clock_cycle =(0.0625 us/1024)= 0.06103515625 ms
	counter_value =(delay/pulse_width)-1
	example: counter_value = (2ms/0.06103515625 ms)-1 = 31.768 (~32)                                                             
	*/
	while(1)
	{
		if (TCNT0 >= 32)
		{
				PORTB ^= 1<<PB5; // PB5 --> output 	
				TCNT0 = 0;
		}
	}
}
	
//start Interrupt Service Routine
volatile uint8_t tot_overflow; // global variable to store count (must be volatile)
ISR(TIMER0_OVF_vect) // goes in when timer0 overflows (TCNT0=255)
{
	
	tot_overflow++;
	if (tot_overflow >=125)
	{
		PORTB ^= (1<<PB5); //output on PB5
		tot_overflow = 0;
	}
}
void timer0_overflow_interrupt(void)
{
	TCCR0B |= (1 << CS00); //start timer without pre-scaler
	TCNT0 =0; //start count=0
	TIMSK0 |= 1<<TOIE0; //overflow interrupt enable
	//sei();				//Enable global interrupts done in main
}

ISR(TIMER1_COMPA_vect) {
	
	app_timer_tick();
}

void timer1_10seg(void){
	
	TCCR1B |= (1 << WGM12);	
	OCR1A = 62499;
	TCCR1B |= (1 << CS12);  	
	// habilita interrupcion 
	TIMSK1 |= (1 << OCIE1A);	
}

// inicia timer 
void timer1_start(void) {
	TCNT1 = 0;  
	TCCR1B |= (1 << CS12);  
}

// detiene el timer
void timer1_stop(void) {
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));  
	TCNT1 = 0;  
}

//timer1 (16 bits)
void timer1_normalmode_noprescaler(void) 
{
	
	TCCR1B |= 1<<CS20; 
	TCNT1 = 0; 

}

void timer1_CTCmode_interrupt(void)
{
	
	TCCR1B |= 1<<WGM12; 
	OCR1A |= 7999;
	TCCR1B |= 1<<CS10;		 	
	TCNT1 = 0;				
	TIMSK1 |= 1<<OCIE1A;	
					
}
void timer1_CTCmode_nonPWM(void)
{
	TCCR1B |= 1<<WGM12; 
	OCR1A |= 24023;
	TCCR1B |= 1<<CS10;		
	TCCR1A |= 1 << COM1A0;	
	TCNT1 = 0;				
}
void timer1_fastPWM_inverting(void)

{
	
	TCCR1A |= 1 << COM1A0 | 1 << COM1A1;
	TCCR1A |= 1 << WGM11;
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	TCCR1B |= 1 << CS11;
	ICR1 = 39999;
	OCR1A = ICR1 - 4000;
	
	
}

//timer2
void timer2_CTCmode(void){
	TCCR2A |= 1 << WGM21; 
	OCR2A = 249;								
	TCCR2B |= (1 << CS20) | (1 << CS22); //set prescaler = 128
	
}



void Timer2_PhaseCorrectPWMMode_Inverting(void){
	TCCR2A |= 1 << WGM20;	
	TCCR2A |= 1 << COM2A1;
	TCCR2B |= (1 << CS21)|(1 << CS20);
	OCR2A = 51;	
}

/*
 * Timers.c
 *
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define STATE_COUNTING		0
#define INT1_ALERT			2
#define INT2_ALERT			3

extern volatile uint32_t	g_counter;
extern volatile uint8_t		g_state;
extern volatile uint8_t		g_update_display;
volatile		uint16_t	g_timer = 0;

static uint16_t prescaler = 0;


void timer1_ctc_1ms_init(void){
	//modo CTC 
	TCCR1B |= (1 << WGM12);
	
	
	//f_Timer = 16MHz / 64 = 250,000 Hz
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	OCR1A = 249;
	
	//habilitar interrupcion 
	TIMSK1 |= (1 << OCIE1A);
}

//ISR  timer1 
ISR(TIMER1_COMPA_vect){
	//5 segundos 
	if (g_state == INT1_ALERT || g_state == INT2_ALERT){
		g_timer++;
		if (g_timer >= 5000){//5 segundos
			g_timer = 0;
			g_state = STATE_COUNTING; //regesa al conteo
		}
	}
	prescaler++;
	
	if (prescaler >= 1000){ // 1s
		prescaler = 0;
		
		// modo conteo 
		
		if (g_state != INT1_ALERT && g_state != INT2_ALERT){
			//límite
			if (g_counter <= 99999){
				g_counter++;
				
				//actualizar
				if (g_state == STATE_COUNTING){
					g_update_display = 1;
				}
			}
			//reset
			else if (g_counter == 100000){
				g_counter = 0 ;
			}
		}
	}
}

void timer0_normalmode_noprescaler(void)
{
	//configure timer
	TCCR0B |= (1 << CS00); //no prescaler
	TCNT0 = 0; // start count from 0
	/*
	fosc = 1/16,000,000 = 0.0625 us								
	counter_value = (delay/pulse_width)-1							
	example: count = (2ms/0.0625us)-1 = 31,999					
	because TCNT0 is 8 bits, divide again:31,999/200 = 159.995(~160)
	that means we get 2ms pulse with counter=200x160 times.
	*Note: 160 is the counter for T=TH+TL.
	*/
	int contador = 0; //counting variable
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
	/*
	T = 1/16,000,000 Hz = 0.0625 us,
	then it takes = 255x0.0625us = 15.9375us
	to reach its maximum count.													 
	Example: for pulse width 2ms:
	tot_ovf_time = 2ms / 15.9375 us = 125.49(~125)
	timer must count 125 times                                                     
	*/
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

//timer1 (16 bits)
void timer1_normalmode_noprescaler(void) 
{
	//config timer
	TCCR1B |= 1<<CS20; //timer1 no prescaler
	TCNT1 = 0; //start count from 0
	/*
	fosc = 1/16,000,000 = 0.0625 us
	counter_value =(delay/pulse_width)-1
	example: counter_value = (2ms/0.0625 ms)-1 = 31,999
	*/
	while (1)
	{
		if (TCNT1 >= 31999)	
		{
				PORTB ^= 1<<PB5; //PB5 --> output
				TCNT1 = 0;
		}
	}
}
//start Interrupt Service Routine
//ISR(TIMER1_COMPA_vect)
//{
	//PORTB ^= 1<<PB5; //PB5 --> output
//}
void timer1_CTCmode_interrupt(void)
{
	//configure timer
	TCCR1B |= 1<<WGM12; // timer1 in CTC mode (pag.141,table 16.4)
	/*
	example: fosc = 16 Mhz, (required)TH = 0.5ms -> T= 1ms f = 1/(2x0.5ms) = 1 KHz						
	(prescaler factor) N=(fosc/(1/T)x65,536) = 
	(16,000,000)/(1000x65,536) = 0.244144 (~1)
	OCR1A = (fosc/(2xNxfocr))-1								
	OCR1A = (16,000,000/2x(1)x(1000))-1 = 7999										
	with this value pulse witdth = 0.5 ms												
	*/
	OCR1A |= 7999;
	TCCR1B |= 1<<CS10;		//set prescaler = 1 	
	TCNT1 = 0;				//Set counting to 0
	TIMSK1 |= 1<<OCIE1A;	//Enable interrupts on compare
	//sei();				//Enable global interrupts done in main
}
void timer1_CTCmode_nonPWM(void)
{
	TCCR1B |= 1<<WGM12; // timer1 in CTC mode (table 16-4)
	/*
	example: fosc=16 Mhz, TH=1.5ms -> T=3ms f = 1/(2x1.5ms) = 333 Hz					
	(precaler factor) N=(fosc/(1/T)x65,536) =
	(16,000,000)/(333x65,536) = 0.733 (~1)	
	OCR1A = (fosc/(2xNxfocr))-1								
	OCR1A = (16,000,000/2x(1)x(333))-1 = 24,023										
	with this value pulse width = 1.5 ms,
	***output on OC1A PB1****/	
	OCR1A |= 24023;
	TCCR1B |= 1<<CS10;		//No prescaling (table 16-5)
	TCCR1A |= 1 << COM1A0;	//OC1A toggle (table 16-1)
	TCNT1 = 0;				//Set counting to 0
}
void timer1_fastPWM_inverting(void)
/*well situated for power regulation,
rectification and DAC applications*/
{
	//inverting mode
	TCCR1A |= 1 << COM1A0 | 1 << COM1A1;
	//FAST PWM (table 16.4) TOP=ICR1 
	TCCR1A |= 1 << WGM11;
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	//Prescaling fclk/8
	//f=16,000,000/8= 2,000,000
	//T= 0.5 us
	TCCR1B |= 1 << CS11;
	
	//Define frecuencia
	//Ejemplo, se requiere un pulso de 2ms cada 50 hz (20 ms)
	//Cuenta ICR1= 20ms/0.5us = 40,000
	//Por lo tanto ICR1= 39,999
	ICR1 = 39999;
	
	//Define ancho de pulso fijo
	//Cuenta OCR1= 2ms/0.5us= 4000
	OCR1A = ICR1 - 4000;
	
	//TCCR1A &= 0 << CS10; //Detiene el reloj
	//Ejemplo para variar el ancho de pulso
	/*
	while (1)
	{
		OCR1A = ICR1 - 4200;
		_delay_ms(200);
		OCR1A = ICR1 - 2200;
		_delay_ms(200);
	}
	*/
}

//timer2
void timer2_CTCmode(void)
{
	TCCR2A |= 1 << WGM21; // timer2 in CTC mode (table 18.8)
	/************************************************************************
	* example: fosc = 16 Mhz, T = 2ms -> focr = 1/(2x2ms) = 250 Hz			
	* (precaler) N=(fosc/(1/T)x256) = (16,000,000)/(500x256) = 125 ~128		
	* from (pag.156) formula: OCR2A = (fosc/(2xNxfocr))-1					
	* OCR2A = (16,000,000/2x(128)x(250))-1 = 249							
	* with this value pulse witdth = 2 ms									
	************************************************************************/
	OCR2A = 249;								
	TCCR2B |= (1 << CS20) | (1 << CS22); //set prescaler = 128
	/*
	while (1)
	{
		if (TIFR2 & (1 << OCF2A)) //if OCF2A = 1 (timercounter2 = OCR2A)
		{
			PORTB ^= 1 << PB5; //PB5 --> output (ports.c)
			TIFR2 =  (1 << OCF2A); //TIFR2 flag back to 1
		}
	}*/
}



void Timer2_PhaseCorrectPWMMode_Inverting(void)
{
	//Usa PB3 (OC2A) como salida (definir PB3--> salida)
	//Define modo PHASE CORRECT PWM (pag.164)
	TCCR2A |= 1 << WGM20;
	//Clear OC2A when upcounting(table 18-4)
	TCCR2A |= 1 << COM2A1;
	//fout=fosc/(prescalerx510)
	//if prescaling fclk/32
	//fout=16,000,000/(32x510)=980.39 Hz ~T= 1 ms
	TCCR2B |= (1 << CS21)|(1 << CS20);
	//example: duty cycle= 20%
	//OCR value=(duty cycle in %)x256=(0.2x256)=51.2 ~ (51)
	OCR2A = 51;
	/*
	uint8_t brillo = 0;
	uint8_t fadein = 1;
	while(1) {	
		OCR2A = brillo;	
		if(fadein)
			brillo += 5;
		else
			brillo -= 5;
		if(brillo == 0  || brillo ==  255){
			fadein = !fadein;
			_delay_ms(2000);
			{
			_delay_ms(50);
			}
		}
	}
	*/
}



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "ports.h"
//#include "leds.h"
#include "ext_int.h"
#include "timers.h"
#include "comp.h"
#include "lcd_i2c.h"

#define STATE_COUNTING		0
#define TEMP_ALERT	1
#define INT1_ALERT	2
#define INT2_ALERT	3

volatile uint32_t g_counter = 0;			//contador principal 
volatile uint8_t  g_state = STATE_COUNTING;	//estado actual del LCD
volatile uint8_t  g_flag = 0;				//1 = alta, 0 =baJA
volatile uint8_t  g_update_display = 1;		//forzar refresco de LCD

int main(void)
{
    
	init_ports();
	//init_i2c();			// Ya se llama dentro de lcd_i2c_init()
	lcd_i2c_init();
	init_ext_int12(); //inicializa INT1 e INT2
	init_comp();			//comparador analogico
	timer1_ctc_1ms_init();	//inicializa 1imer1 
	
	sei();
	
	uint8_t last_app_state = 0xFF; 
	
	char buffer[10]; //convertir el numero a string
	
	while (1)
	{
		if (ACSR & (1 << ACO))
		{
			// ACO = 1 significa AIN0 > AIN1 (Temp > 15°C)
			g_flag = 1;
		}
		else
		{
			// ACO = 0 significa AIN0 <= AIN1 (Temp <= 15°C)
			g_flag = 0;
		}
		
		//maquina de estados 
		if (g_state == INT1_ALERT || g_state == INT2_ALERT){
			//no hacer nada aqui
		}
		//alerta de temperatura
		else if (g_flag == 1){
			g_state = TEMP_ALERT;
		}
		//conteo
		else{
			g_state = STATE_COUNTING;
		}

		
		//LCD 
		if (g_state != last_app_state){
			lcd_i2c_clr(); 
			
			switch (g_state){
				case STATE_COUNTING:
				lcd_i2c_col_row(1, 1);
				lcd_i2c_write_string("CONTADOR:");
				g_update_display = 1; 
				break;
				
				case TEMP_ALERT:
				lcd_i2c_col_row(1, 1);
				lcd_i2c_write_string("ALERTA:");
				lcd_i2c_col_row(1, 2);
				lcd_i2c_write_string("TEMP > 25C");
				break;
				
				case INT1_ALERT:
				lcd_i2c_col_row(1, 1);
				lcd_i2c_write_string("EVENTO:");
				lcd_i2c_col_row(1, 2);
				lcd_i2c_write_string("INTERRUPCION 1");
				break;
				
				case INT2_ALERT:
				lcd_i2c_col_row(1, 1);
				lcd_i2c_write_string("EVENTO:");
				lcd_i2c_col_row(1, 2);
				lcd_i2c_write_string("INTERRUPCION 2");
				break;
			}
			last_app_state = g_state; 
		}
		
		
		
		if (g_state == STATE_COUNTING && g_update_display){
			
			ltoa(g_counter, buffer, 10); //convierte a string
			
			lcd_i2c_col_row(1, 2);
			lcd_i2c_write_string("                "); 
			lcd_i2c_col_row(1, 2);
			lcd_i2c_write_string(buffer);
			
			g_update_display = 0; //limpia flag 
		}
	}
}
 


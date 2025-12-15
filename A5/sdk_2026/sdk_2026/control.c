#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "control.h"
#include "adc.h"
#include "lcd_i2c.h"
#include "usart.h"

// variables globales{

static system_state_t current_state = STATE_WAITING;
static volatile uint8_t seconds_remaining = 0;
static uint8_t watering_done_today = 0;  // Flag para evitar que se repita

void app_init(void){
	
    current_state = STATE_WAITING;
    seconds_remaining = 0;
    watering_done_today = 0;
    
    // bomba
    PORTB &= ~(1 << PB0);
}

void app_start_watering(void){
	
    if (current_state == STATE_WAITING && !watering_done_today){
		
        current_state = STATE_WATERING;
        seconds_remaining = WATERING_TIME;
        
        // Activa bomba 
        PORTB |= (1 << PB0);
        
        //BLE
        usart_transmit_string("INICIO RIEGO\r\n");
        

        lcd_i2c_clr();
        lcd_i2c_col_row(1, 1);
        lcd_i2c_write_string("REGANDO...");
        
        // Inicia Timer1
        timer1_start();
    }
}

void app_stop_watering(void){
	
    // Detiene bomba
    PORTB &= ~(1 << PB0);
    
    
    timer1_stop();
    
    if (current_state == STATE_WATERING){
		
        
        usart_transmit_string("FIN RIEGO\r\n");
        
        lcd_i2c_clr();
        lcd_i2c_col_row(1, 1);
        lcd_i2c_write_string("Riego");
        lcd_i2c_col_row(1, 2);
        lcd_i2c_write_string("Completado!");
        _delay_ms(2000);
        
        // cambia flafg
        watering_done_today = 1;
        current_state = STATE_COMPLETED;
        
        
        lcd_i2c_clr();
        lcd_i2c_col_row(1, 1);
        lcd_i2c_write_string("Esperando");
        lcd_i2c_col_row(1, 2);
        lcd_i2c_write_string("nuevo dia...");
    }
    else if (current_state == STATE_STOPPED){
		
        // Parada de emergencia
        usart_transmit_string("EMERGENCIA!\r\n");
        
        lcd_i2c_clr();
        lcd_i2c_col_row(1, 1);
        lcd_i2c_write_string("DETENIDO");
        lcd_i2c_col_row(1, 2);
        lcd_i2c_write_string("EMERGENCIA!");
        _delay_ms(3000);
        
        
        current_state = STATE_WAITING;
        
        lcd_i2c_clr();
        lcd_i2c_col_row(1, 1);
        lcd_i2c_write_string("Esperando");
        lcd_i2c_col_row(1, 2);
        lcd_i2c_write_string("noche");
    }
    
    seconds_remaining = 0;
}

void app_process(void){
	
    uint16_t light_level = app_get_light_level();
    
    switch (current_state){
		
        case STATE_WAITING:
            //logica
            if (light_level < LIGHT_THRESHOLD_DARK && !watering_done_today){
				
                app_start_watering();
            }
            break;
            
        case STATE_WATERING:
           
            if (seconds_remaining > 0){
				
                lcd_i2c_col_row(1, 2);
                lcd_i2c_write_string("Tiempo: ");
                
                // Muestra segundos restantes
                char time_str[4];
                sprintf(time_str, "%2d", seconds_remaining);
                lcd_i2c_write_string(time_str);
                lcd_i2c_write_string("s ");
            }
            else{
				
                
                app_stop_watering();
            }
            break;
            
        case STATE_COMPLETED:
            
            if (light_level > LIGHT_THRESHOLD_BRIGHT){
				
                
                watering_done_today = 0;
                current_state = STATE_WAITING;
                
                usart_transmit_string("NUEVO DIA\r\n");
                
                lcd_i2c_clr();
                lcd_i2c_col_row(1, 1);
                lcd_i2c_write_string("Nuevo dia!");
                lcd_i2c_col_row(1, 2);
                lcd_i2c_write_string("Sistema listo");
                _delay_ms(2000);
                
                lcd_i2c_clr();
                lcd_i2c_col_row(1, 1);
                lcd_i2c_write_string("Esperando");
                lcd_i2c_col_row(1, 2);
                lcd_i2c_write_string("noche");
            }
            break;
            
        case STATE_STOPPED:
            // Ya fue detenido por INT0
            app_stop_watering();
            break;
    }
}

uint16_t app_get_light_level(void){
	
    return read_adc();
}

uint8_t app_get_remaining_time(void){
	
    return seconds_remaining;
}

system_state_t app_get_state(void){
	
    return current_state;
}

//ISR de Timer1 
void app_timer_tick(void){
	
    if (current_state == STATE_WATERING && seconds_remaining > 0){
		
        seconds_remaining--;
    }
}

// ISR 
void app_emergency_stop(void){
	
    if (current_state == STATE_WATERING){
		
        current_state = STATE_STOPPED;
    }
}
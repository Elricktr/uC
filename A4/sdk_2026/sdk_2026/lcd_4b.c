/*
 * lcd_4b.c
 *
 * Created: 6/24/2021 7:47:30 AM
 *  Author: jlb
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "LCD_4b.h"

char FirstColumn [2] = {0x00 , 0x40};	//Direcciones DDRAM (first column)

void lcd_reset(void)
{
	lcd_port = 0xFF; 		//wait at least 15 ms
	_delay_ms(20);
	
	lcd_port = 0x30+LCD_EN; //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	lcd_port = 0x30;		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(10);			//wait at least 4.1 ms
	
	lcd_port = 0x30+LCD_EN; //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	lcd_port = 0x30;		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(1);			//wait at least 100 us
	
	lcd_port = 0x30+LCD_EN; //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	lcd_port = 0x30;		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(1);			//wait at least 100 us
	
	lcd_port = 0x20+LCD_EN; //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 & E=1
	lcd_port = 0x20;		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 & E=0
	_delay_ms(1);			//wait at least 100 us
}

void lcd_cmd (char cmd)
{
	lcd_port = (cmd & 0xF0)|LCD_EN;
	lcd_port = (cmd & 0xF0);
	lcd_port = ((cmd << 4) & 0xF0)|LCD_EN;
	lcd_port = ((cmd << 4) & 0xF0);
	_delay_ms(2);
	_delay_ms(2);
}

void lcd_init (void)
{
	lcd_reset();        // Reset
	
	lcd_cmd(0x2C);		// Set: 4 bits, 2 lines, resolution 5×10
	lcd_cmd(0x0C);      // Display control: LCD on, cursor on, blinking
	lcd_cmd(0x06);      // Entry mode: Increment
	lcd_cmd(0x80);      // point to DDRAM
}

void lcd_data (unsigned char dat)
{
	lcd_port = ((dat & 0xF0)|LCD_EN|LCD_RS);
	lcd_port = ((dat & 0xF0)|LCD_RS);
	lcd_port = (((dat << 4) & 0xF0)|LCD_EN|LCD_RS);
	lcd_port = (((dat << 4) & 0xF0)|LCD_RS);
	_delay_ms(2);
	_delay_ms(2);
}


void lcd_col_row(uint8_t x, uint8_t y)
{
	lcd_cmd(0x80 + FirstColumn[y-1]+(x-1));
}


void lcd_clr(void)
{
	lcd_cmd(0x01); //clear screen (0000 0001)
	_delay_ms(2);
}


void lcd_write_string(char *a)
{
	unsigned int i;
	for(i=0;a[i]!=0;i++)
	lcd_data(a[i]);
}

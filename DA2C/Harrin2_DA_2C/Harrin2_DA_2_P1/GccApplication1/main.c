/*
 * GccApplication1.c
 *
 * Created: 10/11/2019 3:14:47 PM
 * Author : Nick
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
 DDRB = (1<<DDB3); 
 TCCR0A = 0; 
 TCNT0 = 0x00;
 TCCR0B |= (1<<CS02) | (1<<CS00);  
    while (1) 
    {
		while ((TIFR0 & 0x01) == 0);
		PORTB ^= (1<<3);
		TCNT0 = 0x00;
		TIFR0 = 0x01;
    }
}


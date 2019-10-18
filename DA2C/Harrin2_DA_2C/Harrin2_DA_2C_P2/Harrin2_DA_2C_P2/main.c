/*
 * Harrin2_DA_2C_P2.c
 *
 * Created: 10/11/2019 10:54:51 PM
 * Author : Nick
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
   DDRB = 1<<3; //Normal operation
   TCCR0A = 0;
   TCNT0 = 124;
   TIMSK0 |= (1<<TOIE0);
   TCCR0B |= (1<<CS02) | (1<<CS00); //set prescalar 
   
   sei();
    while (1) 
    {
		while((TIFR0 & 0x01) == 0);
		TCNT0 = 0x00;
		TIFR0 = 0x01;
		PORTB ^= (0<<3);
    }
}
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 124;
	PORTB ^= (1<<3); //toggle PB.3
}

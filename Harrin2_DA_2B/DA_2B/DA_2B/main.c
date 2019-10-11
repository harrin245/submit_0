/*
 * DA_2B.c
 *
 * Created: 10/6/2019 1:19:13 AM
 * Author : Nick
 *///DA_2B 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL


int main(void)
{
	DDRB = (1<<3); //pb3 set as output
	PORTC = (1<<3); //pull-up activated
	EICRA = 0x2; //falling edge triggered
	EIMSK = (1<<INT0); //enable ext interrupt 0
	sei(); //enable interrupts
	
    while (1) 
    {
		ISR(INT0_vect)
		{
		PORTB ^= (1<<3); //toggle portb.3	
		}
    }
}

  
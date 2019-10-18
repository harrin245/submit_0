/*
 * GccApplication1.c
 *
 * Created: 10/16/2019 12:30:39 PM
 * Author : Nick
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/interrupt.h>

int main(void)
{
  DDRB = 1<<3; //set PB6 as output
  TCCR0A |= (1<<WGM01) | (1<<COM1A0); //Set timer to CTC
  OCR0A = 0xF0; //count to this value
  TCNT0 = 0; //initialize counter
  TCCR0B |= (1<<CS02) | (1<<CS00); //set pre-scaler
  sei(); //enable interrupts
    while (1) 
    {
		PORTB ^= (0<<3); //untoggle pb3
    }
}
ISR (TIMER0_COMPA_vect)
{
PORTB ^= (1<<3); //toggle pb3	
}
}

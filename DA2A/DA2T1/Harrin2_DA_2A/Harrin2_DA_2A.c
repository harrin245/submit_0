//Nick Harris CPE301 Assignment 2A

#include <iosstream>
#include <avr/io.h>
#include <util/delay.h>
using namespace std;

int main()
{
	DDRB = 0xFF; //set port b as output port
	DDRC = 0x00; //set port c as input port
	PORTC |= (1<<2); //turn on port c pin 3 pull up
	
	if (PINC & (1<<2)) //if pin c port 3 enabled 
	{
	while(1)
	{
		PORTB |= (1<<1); //set port b pin 2
		delay(); //delay for ~ 1.3 secs (each delay ~250ms)
		delay();
		delay();
		delay();
		PORTB &= ~(1<<1); //clear port b pin 2
		if (PINC & ~(1<<2)) //check if pin c port 3 is enabled
		{
			break; //break if not enabled
		}
	}
	}
}

void delay()
{
_delay_ms(250); //delay for 250 ms
}
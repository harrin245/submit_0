/*
 * Harrin2_Midterm.c
 *ThingSpeak Channel ID: 896249
 * Created: 10/26/2019 2:32:33 PM
 * Author : Nick Harris
 */ 
#define F_CPU 8000000UL						//Sets the clock speed of the MCU
#define BAUD 9600							//Sets the baud rate for the UART to transmit
#define BAUDRATE ((F_CPU) / (BAUD * 8UL)-1) // Set Baud Rate Value for UBRR

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char string_num[]; //Holds a number after string conversion
char string_float[]; //holds a decimal value after string conversion
float temp; //Holds the measurement of the temp sensor
char string_deg[] = "°F"; //temperature will be measured in Fahrenheit
char string[] = "The Temperature is: "; //string to announce temp 
char linebreak[] = "/r/n"; //end of line character
volatile uint8_t time_ovf; //holds number of overflows of timer
char cipmux[] = "at+cipmux = 1"; //string for esp to set up single or multi line connection
char cwjap[] = "at+cwjap =\""; //string for esp to look for access points
char passwrd[] = "\"Your Password""; //holds password for access point
char ssid[] = "\Your ssid\","; //holds id/name of access point
char api_key[]="C3ULAMG0GEL8PX8E&field1="; //holds thinkspeak channel key
char ts_command[]="GET /update?api_key="; //update key
char cipstart[]="AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80"; //string to strart communication with thinkspeak
char cipclose[]="AT+CIPCLOSE"; //string to close communication with thinkspeak
char cipsend[]="AT+CIPSEND=4,77"; //string command to sends bits to thinkspeak								 

//Initializes timer1					     
void init_TIMER1()
{
	//Set up timer with a prescale of 64
	TCCR1A |= (0<<COM1A1) | (0<<COM1A0);
	TCCR1B |= (1<<CS11)|(1 << CS10);
	
	//Initialize counter
	TCNT1 = 0;
	
	//Enable overflow interrupt
	TIMSK1 |= (1 << TOIE1);
	
	//Initialize overflow counter variable
	time_ovf = 0;
}

//Used to initialize the UART with a baud rate of 9600 and enables global interrupts.
void init_UART()
{
	UBRR0H = (uint8_t)(BAUDRATE>>8);
	UBRR0L = (uint8_t)(BAUDRATE);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
	sei();
}

//Function used to send one character at a time through USART
void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

//Function used to receive data from USART
unsigned char USART_receive(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

//sends converted string numbers through usart
void usart_send_num( unsigned int data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

//function sends string through usart  
void usart_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}

//converts a string integer and sends through usart
void usart_putnumber(char* String_num, unsigned int temperature){
	
	String_num =  (utoa(temperature,String_num,10)); //Converts an int to a string
	while(*String_num != 0x00){
		USART_send_num(*String_num);
	String_num++;}
	
}

//Function used to send an array of characters of converted floating point value into a string through USART
void USART_putflt(char* String_flt, float temperature){
	
	String_flt =  dtostrf(temperature,0,2,String_flt); //Converts a double to a string
	while(*String_flt != 0x00){
		USART_send_num(*String_flt);
	String_flt++;}
	
}

//Function used to initialize the ADC with an adc-prescaler of 64 and using a reference voltage of 5V
void init_adc(){
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0));    //Prescaler at 64 so we have an 125Khz clock source
	ADMUX |= (0<<REFS1)|( 1 << REFS0 );

}

//function starts convertion of adc and stores it to temp 
void read_adc( uint8_t channel )
{
	// Select the ADC channel to be read.
	ADMUX |= channel;
	// Turn on the ADC.
	ADCSRA |= ( 1 << ADEN );

	// Start the conversion.
	ADCSRA |= ( 1 << ADSC );

	
	while( ADCSRA & ( 1 << ADIF ) );
	
	temperature= ADC; //read upper 8bits
	
	temperature = ((temperature/2)*1.8)+32; //Convert converted value to Fahrenheit.

}

//Interrupt Service Routine used for Timer1 Overflow.
ISR(TIMER1_OVF_vect)
{
	//Keep track of number of overflows
	time_ovf++;
}

ISR(ADC_vect)
{
	PORTD = ADCL;
	PORTB = ADCH;
	
	ADCSRA|=(1<<ADSC);	//start conversion

	while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
	
	ADCSRA |= (1<<ADIF);
	int a = ADCL;
	a = a | (ADCH<<8);
	a -= 289;
	itoa(a, buffer, 10);
	USART_putstring(buffer);
	USART_putstring("\n");
	_delay_ms(1000);
}

int main(void)
{
    init_TIMER1();
	init_UART();
	init_adc();
	read_adc();
	
	USART_putstring(cipmux);
	USART_putstring(linebreak);
	_delay_ms(1000);
	
	USART_putstring(cwjap);
	USART_putstring(ssid);
	USART_putstring(passwrd);
	USART_putstring(linebreak);
	
	/*ADCSRA= 0x86;			//make ADC enable and select ck/64
	ADMUX= 0xC8;			//1.1V Vref, temp, right-justified, internal temp. sensor

	DDRB = (1<<5);
	DDRD = 0x00;

	ADCSRA |= (1<<ADSC);
	sei();*/

	while (1)
	{
		if(time_ovf >= 130){ //Uses increments of 4 for a timer1 overflow interrupt of roughly 1 second. (130 is roughly 33 seconds)
			
			ADC_Read(0); //Using ADC 0
			USART_putstring(string);
			USART_putflt(string_num, temp);
			USART_putstring(string_deg);
			USART_putstring(linebreak);
			USART_putstring(linebreak);
			_delay_ms(2000);
			
			USART_putstring(cipstart);
			USART_putstring(linebreak);
			
			_delay_ms(20000);
			
			USART_putstring(cipsend);
			USART_putstring(linebreak);
			
			_delay_ms(3000);
			
			USART_putstring(ts_command);
			USART_putstring(api_key);
			USART_putflt(string_num,temp);
			USART_putstring(linebreak);

			_delay_ms(30000);
			
			USART_putstring(ts_command);
			USART_putstring(api_key);
			USART_putflt(string_num,temp);
			USART_putstring(linebreak);
			
			_delay_ms(3000);
			
			USART_putstring(cipsend);
			USART_putstring(linebreak);
			
			_delay_ms(3000);
			
			
			TCNT1 = 0;
			time_ovf = 0;
			
		}
		
		else;
	}
	
	return 0;

}


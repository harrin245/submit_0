/*
 * Harrin2_Midterm_2_mpu.c
 *
 * Created: 12/13/2019 7:12:52 PM
 * Author : harrin2
 */ 

 #ifndef F_CPU
 #define F_CPU 16000000UL
 #endif

 #include <avr/io.h>
 #include <util/delay.h>
 #include <math.h>
 #include <stdlib.h>										//Include standard library file 
 #include <stdio.h>										//Include standard library file 
 #include "MPU6050_def.h"							//Include MPU6050 register define file
 #include "i2c_master.h"							//Include I2C Master header file 
 #include "uart.h"							//Include USART header file 

 #define MPU6050_WRITE 0xD0
 #define MPU6050_READ 0xD1

 float Acc_x;

 void init_uart(uint16_t baudrate){ //intialize uart and buad rate

	 uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	 UBRR0H = UBRR_val >> 8;
	 UBRR0L = UBRR_val;

	 UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX
	 UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchronous 8N1
 }

 void uart_putc(unsigned char c){ //function helps send chars through uart

	 while(!(UCSR0A & (1<<UDRE0))); // wait for sending
	 UDR0 = c; // output character saved
 }

 void uart_puts(char *s){ //function helps send strings through uart
	 while(*s){
		 uart_putc(*s);
		 s++;
	 }
 }

 void init_MPU6050(void){ //initialize MPU
	 _delay_ms(150);										//Power up time greater than 100ms
	 i2c_start(MPU6050_WRITE); // Set Gyroscope Sample Rate = 1 KHz, Accelerometer Sample Rate = 1 KHz (default)
	 i2c_write(SMPLRT_DIV); // Sample Rate is generated by dividing the gyroscope output rate by SMPLRT_DIV
	 i2c_write(0x07); // Gyroscope Output Rate = 8kHz, Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
	 i2c_stop();

	 i2c_start(MPU6050_WRITE);
	 i2c_write(PWR_MGMT_1);
	 i2c_write(0x01); // PLL with X axis gyroscope reference
	 i2c_stop();

	 i2c_start(MPU6050_WRITE);
	 i2c_write(CONFIG); //Frame Synchronization & Digital Low Pass Filter setting
	 i2c_write(0x00);
	 i2c_stop();
	 
	 i2c_start(MPU6050_WRITE);
	 i2c_write(GYRO_CONFIG); //gyroscopes’ scale range = FS_SEL selects = 11 = +/- 2000 °/s
	 i2c_write(0x18);		// default accelerometer range = +/- 2g
	 i2c_stop();

	 i2c_start(MPU6050_WRITE);
	 i2c_write(INT_ENABLE); // DATA_RDY_EN = 1
	 i2c_write(0x01);
	 i2c_stop();
 }

 void getreading(void){ //read values from i2c

	 i2c_start(MPU6050_WRITE);
	 i2c_write(ACCEL_XOUT_H); // set pointer
	 i2c_stop();

	 i2c_start(MPU6050_READ);
	 Acc_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	 i2c_stop();
 }

 int main(void){                                             //Main function
	 char buffer[20], float_[10];
	 float Xa;
	 init_uart(9600);
	 i2c_init();
	 init_MPU6050();
	 
	 while(1){
		 getreading();
		 Xa = Acc_x/16384.0;								//Divide raw value by sensitivity scale factor to get real values

		 dtostrf( Xa, 3, 2, float_ );					//Take values in buffer to send all parameters over USART
		 sprintf(buffer,"%s, ",float_);
		 USART_SendString(buffer);

		 _delay_ms(1000);
	 }
	 
	 return 0;
 }

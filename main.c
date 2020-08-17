
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

#include "BME280_SPI.h"
#include "BME280_SPI.cpp"
#include "BME280_SPI1.h"
#include "BME280_SPI1.cpp"
#include "BME280_SPI2.h"
#include "BME280_SPI2.cpp"
#include "BME280_SPI3.h"
#include "BME280_SPI3.cpp"
#include "spi.h"
#include "ADC.h"




void USART_Transmit( int x )
{
	unsigned char data=x;
	while ( !( UCSR3A & (1<<UDRE3)) );
	
	UDR3 = data;
}
void USART_Transmit_String(char* stringToSend)
{
	
	while (*stringToSend)
	USART_Transmit(*stringToSend++);
	
	
}

unsigned char USART_Receive( void )
{
	
	while ( !(UCSR3A & (1<<RXC3)) );
	
	return UDR3;
}

/////// Zmienne dla odczytów BME/////////////////////////////////////////////
double temp, humid, pressure;
double temp1, humid1, pressure1;
double temp2, humid2, pressure2;
double temp3, humid3, pressure3;

char charValt[10];
char charValp[10];
char charValh[10];

char charValt1[10];
char charValp1[10];
char charValh1[10];

char charValt2[10];
char charValp2[10];
char charValh2[10];

char charValt3[10];
char charValp3[10];
char charValh3[10];
//////////////////////zmienne do odczytów wilgotnoœci gleby//////////////////////////

int adc_value;
int adc_value1;
int adc_value2;
int adc_value3;

float moisture;
float moisture1;
float moisture2;
float moisture3;

char charValg[10];
char charValg1[10];
char charValg2[10];
char charValg3[10];

int main(void)
{

	
	DDRC |= (1<<PC0);
	DDRC |= (1<<PC1);
	DDRC |= (1<<PC2);
	DDRC |= (1<<PC3);
	DDRH |= (1<<PH3);
	
	/////////////// USART INIT///////////////////////////////////////////////////
	
	UCSR3A |= (1 << U2X3);
	UBRR3 = 103;
	UCSR3C |= (1 << UCSZ31) | (1 << UCSZ30);
	UCSR3B |= (1 << TXEN3) | (1 << RXEN3) | (1 << RXCIE3);
	
	/////////////////////////// BME0-3 INIT/////////////////////////////////////////////
	bme280_calib_data _bme280_calib;
	//begin(BME_CS);
	
	uint8_t osrs_t = 1;             //Temperature oversampling x 1
	uint8_t osrs_p = 1;             //Pressure oversampling x 1
	uint8_t osrs_h = 1;             //Humidity oversampling x 1
	uint8_t mode = 3;               //Normal mode
	uint8_t t_sb = 5;               //Tstandby 1000ms
	uint8_t filter = 0;             //Filter off
	uint8_t spi3w_en = 0;           //3-wire SPI Disable
	
	uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
	uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
	uint8_t ctrl_hum_reg  = osrs_h;
	
	SPI_MasterInit();
	_delay_ms(1000);
	
	writeReg(0xF2,ctrl_hum_reg);
	writeReg(0xF4,ctrl_meas_reg);
	writeReg(0xF5,config_reg);
	readTrim();
	_delay_ms(500);
	
	writeReg1(0xF2,ctrl_hum_reg);
	writeReg1(0xF4,ctrl_meas_reg);
	writeReg1(0xF5,config_reg);
	readTrim1();
	_delay_ms(500);
	
	writeReg2(0xF2,ctrl_hum_reg);
	writeReg2(0xF4,ctrl_meas_reg);
	writeReg2(0xF5,config_reg);
	readTrim2();
	_delay_ms(500);
	
	writeReg3(0xF2,ctrl_hum_reg);
	writeReg3(0xF4,ctrl_meas_reg);
	writeReg3(0xF5,config_reg);
	readTrim3();
	_delay_ms(500);
	//////////////////////////////////////////////////////////////////////////////////////
	///////////LCD INIT////////////////////////////////////
	DDRE |= (1<<PE3);
	PORTE |= (1<<PE3);
	DDRF|=(1<<PF6);
	PORTF &= ~(1<<PF6);
	
	//////////////////////////////////////////////////////////////////////////
	
	
	DIDR2 |= (1<<ADC8D) | (1<<ADC9D) | (1<<ADC10D);
	ADC_Init();
	USART_Receive();
    sei();
    while (1) 
    {
		//odczyt adc//////////////////////////////////////
		
		
		adc_value = ADC_Read();	
		moisture = 100-(adc_value*100.00)/1023.00; 
		dtostrf(moisture,4,2,charValg);
		_delay_ms(100);
		
		//odczyt adc1//////////////////////////////////////
	
		
		adc_value1 = ADC_Read1();
		moisture1 = 100-(adc_value1*100.00)/1023.00;
		dtostrf(moisture1,4,2,charValg1);
		_delay_ms(100);
		
		//odczyt adc2//////////////////////////////////////
		
		
		adc_value2 = ADC_Read2();
		moisture2 = 100-(adc_value2*100.00)/1023.00;
		dtostrf(moisture2,4,2,charValg2);
		_delay_ms(100);
		
		////odczyt adc3//////////////////////////////////////
		
		
		adc_value3 = ADC_Read3();
		moisture3 = 100-(adc_value3*100.00)/1023.00;
		dtostrf(moisture3,4,2,charValg3);
		_delay_ms(100);
		
		//odczyt bme///////////////////////////////////////
		temp = readTemperature();
		dtostrf(temp,4,2,charValt);
		humid = readHumidity();
		dtostrf(humid,4,2,charValh);
		pressure = readPressure();
		dtostrf(pressure,4,2,charValp);
		
		//odczyt bme1//////////////////////////////////////
		temp1 = readTemperature1();
		dtostrf(temp1,4,2,charValt1);
		humid1 = readHumidity1();
		dtostrf(humid1,4,2,charValh1);
		pressure1 = readPressure1();
		dtostrf(pressure1,4,2,charValp1);
		
		//odczyt bme2/////////////////////////////////////
		temp2 = readTemperature2();
		dtostrf(temp2,4,2,charValt2);
		humid2 = readHumidity2();
		dtostrf(humid2,4,2,charValh2);
		pressure2 = readPressure2();
		dtostrf(pressure2,4,2,charValp2);
		
		//odczyt bme3/////////////////////////////////////
		temp3 = readTemperature3();
		dtostrf(temp3,4,2,charValt3);
		humid3 = readHumidity3();
		dtostrf(humid3,4,2,charValh3);
		pressure3 = readPressure3();
		dtostrf(pressure3,4,2,charValp3);
		_delay_ms(4000);
    }
}

ISR(USART3_RX_vect)
{
	char com = UDR3;
	
	switch(com)
	{
		case 'a':
		USART_Transmit_String("Temperature: ");
		USART_Transmit_String(charValt);
		USART_Transmit_String("DegC  ");	
			
		USART_Transmit_String("Humidity: ");
		USART_Transmit_String(charValh);
		USART_Transmit_String("%  ");
		
		USART_Transmit_String("Pressure: ");
		USART_Transmit_String(charValp);
		USART_Transmit_String("hPa  ");	
		
		USART_Transmit_String("W. gleby: ");
		USART_Transmit_String(charValg);	
		USART_Transmit_String("%  ");	
		
		
		break;
		case 'b':
		
		
		USART_Transmit_String("Temperature: ");
		USART_Transmit_String(charValt1);
		USART_Transmit_String("DegC  ");
		
		USART_Transmit_String("Humidity: ");
		USART_Transmit_String(charValh1);
		USART_Transmit_String("%  ");
		
		USART_Transmit_String("Pressure: ");
		USART_Transmit_String(charValp1);
		USART_Transmit_String("hPa  ");
		
		USART_Transmit_String("W. gleby: ");
		USART_Transmit_String(charValg1);
		USART_Transmit_String("%  ");
		
		break;
		case 'c':
		
		
		USART_Transmit_String("Temperature: ");
		USART_Transmit_String(charValt2);
		USART_Transmit_String("DegC  ");
		
		USART_Transmit_String("Humidity: ");
		USART_Transmit_String(charValh2);
		USART_Transmit_String("%  ");
		
		USART_Transmit_String("Pressure: ");
		USART_Transmit_String(charValp2);
		USART_Transmit_String("hPa  ");
		
		USART_Transmit_String("W. gleby: ");
		USART_Transmit_String(charValg2);
		USART_Transmit_String("%  ");
		
		break;
		case 'd':
		
		USART_Transmit_String("Temperature: ");
		USART_Transmit_String(charValt3);
		USART_Transmit_String("DegC  ");
		
		USART_Transmit_String("Humidity: ");
		USART_Transmit_String(charValh3);
		USART_Transmit_String("%  ");
		
		USART_Transmit_String("Pressure: ");
		USART_Transmit_String(charValp3);
		USART_Transmit_String("hPa  ");
		
		USART_Transmit_String("W. gleby: ");
		USART_Transmit_String(charValg3);
		USART_Transmit_String("%  ");
		
		
		break;
		case 'e':
		DDRJ |= (1<<PJ3);
		DDRH |=(1<<PH4);
		PORTH &=~(1<<PH4);
		break;
		case 'E':
		DDRJ &= ~(1<<PJ3);
		DDRH |=(1<<PH4);
		PORTH |=(1<<PH4);
		break;
		case 'f':
		DDRJ |= (1<<PJ4);
		DDRH |=(1<<PH6);
		PORTH &=~(1<<PH6);
		break;
		case 'F':
		DDRJ &= ~(1<<PJ4);
		DDRH |=(1<<PH5);
		PORTH |=(1<<PH5);
		break;
		case 'g':
		DDRJ |= (1<<PJ5);
		DDRH |=(1<<PH6);
		PORTH &=~(1<<PH6);
		break;
		case 'G':
		DDRJ &= ~(1<<PJ5);
		DDRH |=(1<<PH6);
		PORTH |=(1<<PH6);
		break;
		case 'h':
		DDRJ |= (1<<PJ6);
		DDRB |=(1<<PB0);
		PORTB &= ~(1<<PB0);
		break;
		case 'H':
		DDRJ &= ~(1<<PJ6);
		DDRB |=(1<<PB0);
		PORTB |=(1<<PB0);
		break;
		case 'i':
		DDRG |= (1<<PG2);
		PORTH |= (1<<PH3);
		
		break;
		case 'I':
		DDRG &= ~(1<<PG2);
		PORTH &= ~(1<<PH3);
		break;
		default:
		break;
	}
	UDR3=com;
}

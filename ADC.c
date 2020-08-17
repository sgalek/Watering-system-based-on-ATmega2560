#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"


void ADC_Init()
{
	
	DDRA = (0<<PF0);
	DDRA = (0<<PK0);
	DDRA = (0<<PK1);
	DDRA = (0<<PK2);
	ADCSRA |=(1<<ADPS0);
	ADCSRA |=(1<<ADPS1);
	ADCSRA |=(1<<ADPS2);
	ADCSRA |=(1<<ADEN);
}


int ADC_Read()
{
	
	ADCSRB	&= ~(1<< MUX5);
	ADMUX = 0x01000000; //choose channel adc
	
	ADCSRA |=(1<<ADSC);
	_delay_us(20);
	//ADCSRA |=(1<<ADSC);
	while (ADCSRA & (1 << ADSC));
// 	while((ADCSRA &(1<<ADIF))==0);
// 	ADCSRA |=(1<<ADIF);
	return (ADCW);
	
	
}

int ADC_Read1()
{
	
	ADCSRB	|= (1<< MUX5); // choose channel adc8
	ADMUX	= 0b01000000; 
	
	ADCSRA |=(1<<ADSC);
	_delay_us(20);
	//ADCSRA |=(1<<ADSC);
	while (ADCSRA & (1 << ADSC));
// 	while((ADCSRA &(1<<ADIF))==0);
// 	ADCSRA |=(1<<ADIF);
	return (ADCW);
}

int ADC_Read2()
{
	
	ADCSRB	|= (1<< MUX5); //channel adc9
	ADMUX	= 0b01000001; //
	
	ADCSRA |=(1<<ADSC);
	_delay_us(20);
	//ADCSRA |=(1<<ADSC);
	while (ADCSRA & (1 << ADSC));
// 	while((ADCSRA &(1<<ADIF))==0);
// 	ADCSRA |=(1<<ADIF);
	return (ADCW);
}

int ADC_Read3()
{
	
	ADCSRB	|= (1<< MUX5); //channel adc10
	ADMUX	= 0b01000010;//
	
	ADCSRA |=(1<<ADSC);
	_delay_us(20);
	//ADCSRA |=(1<<ADSC);
	while (ADCSRA & (1 << ADSC));
// 	while((ADCSRA &(1<<ADIF))==0);
// 	ADCSRA |=(1<<ADIF);
	return (ADCW);
}
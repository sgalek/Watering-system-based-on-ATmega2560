#include <avr/io.h>
#include "spi.h"


void SPI_MasterInit(void)
{
	
	DDRB = (1<<2)|(1<<1)|(1<<0);
	
	SPCR = (1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(1<<CPHA)|(0<<DORD);
	SPCR = SPCR | (1<<SPE);
	

	
};

void SPI_write(uint8_t byteword)
{
	SPDR = byteword;
	while(!(SPSR & (1<<SPIF)));
};

uint8_t SPI_read( uint8_t byteword)
{
	SPDR = byteword;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
};

uint8_t SPI_transmit( uint8_t byteword)
{
	SPDR = byteword;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
};
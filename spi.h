#ifndef SPI_H
#define SPI_H


void SPI_MasterInit(void);


void SPI_write(uint8_t byteword);


uint8_t SPI_read( uint8_t byteword);


uint8_t SPI_transmit( uint8_t byteword);

#endif
/*
 * Modified from Switchscience BME280 sample code.
 * https://github.com/SWITCHSCIENCE/BME280/blob/master/Arduino/BME280_I2C/BME280_I2C.ino
 */
//#include <ESP8266WiFi.h>

#include "BME280_SPI.h"
#include "spi.h"

void writeReg3(uint8_t reg_address, uint8_t data) {
    PORTC &= ~(1<<PC3);
	SPI_transmit(reg_address & 0x7F);
	SPI_transmit(data);
    PORTC |= (1<<PC3);
   
   
   
   /* digitalWrite(bme280_cs, LOW);
    SPI.transfer(reg_address & B01111111);
    SPI.transfer(data);
    digitalWrite(bme280_cs, HIGH);*/
}

void readReg3(uint8_t reg_address, int numBytes, uint8_t * data) {
	uint8_t addr = reg_address | 0x80 ;
    PORTC &= ~(1<<PC3);
    SPI_transmit(addr);
	for (int i = 0; i < numBytes; i++) {
		data[i] = SPI_transmit(0x00);
	}
    PORTC |= (1<<PC3);
  /*  uint8_t addr = reg_address | B10000000; 0x80
    digitalWrite(bme280_cs, LOW);
    SPI.transfer(addr);
    for (int i = 0; i < numBytes; i++) {
        data[i] = SPI.transfer(0x00);
    }
    digitalWrite(bme280_cs, HIGH);*/
}

void readTrim3() {
    uint8_t data[32];//,i=0;

    readReg3(0x88, 24, data);
    readReg3(0xA1,  1, &data[24]);
    readReg3(0xE1,  7, &data[25]);

    _bme280_calib.dig_T1 = (data[1] << 8) | data[0];
    _bme280_calib.dig_T2 = (data[3] << 8) | data[2];
    _bme280_calib.dig_T3 = (data[5] << 8) | data[4];
    _bme280_calib.dig_P1 = (data[7] << 8) | data[6];
    _bme280_calib.dig_P2 = (data[9] << 8) | data[8];
    _bme280_calib.dig_P3 = (data[11]<< 8) | data[10];
    _bme280_calib.dig_P4 = (data[13]<< 8) | data[12];
    _bme280_calib.dig_P5 = (data[15]<< 8) | data[14];
    _bme280_calib.dig_P6 = (data[17]<< 8) | data[16];
    _bme280_calib.dig_P7 = (data[19]<< 8) | data[18];
    _bme280_calib.dig_P8 = (data[21]<< 8) | data[20];
    _bme280_calib.dig_P9 = (data[23]<< 8) | data[22];
    _bme280_calib.dig_H1 = data[24];
    _bme280_calib.dig_H2 = (data[26]<< 8) | data[25];
    _bme280_calib.dig_H3 = data[27];
    _bme280_calib.dig_H4 = (data[28]<< 4) | (0x0F & data[29]);
    _bme280_calib.dig_H5 = (data[30] << 4) | ((data[29] >> 4) & 0x0F);
    _bme280_calib.dig_H6 = data[31];   

#if 0
    printf("T1:");printf(_bme280_calib.dig_T1);
    printf(", T2:");printf(_bme280_calib.dig_T2);
    printf(", T3:");printf(_bme280_calib.dig_T3);
    printf("\r\n");
    printf("P1:");printf(_bme280_calib.dig_P1);
    printf(", P2:");printf(_bme280_calib.dig_P2);
    printf(", P3:");printf(_bme280_calib.dig_P3);
    printf(", P4:");printf(_bme280_calib.dig_P4);
    printf(", P5:");printf(_bme280_calib.dig_P5);
    printf(", P6:");printf(_bme280_calib.dig_P6);
    printf(", P7:");printf(_bme280_calib.dig_P7);
    printf(", P8:");printf(_bme280_calib.dig_P8);
    printf(", P9:");printf(_bme280_calib.dig_P9);
    printf("\r\n");
    printf("H1:");printf(_bme280_calib.dig_H1);
    printf(", H2:");printf(_bme280_calib.dig_H2);
    printf(", H3:");printf(_bme280_calib.dig_H3);
    printf(", H4:");printf(_bme280_calib.dig_H4);
    printf(", H5:");printf(_bme280_calib.dig_H5);
    printf(", H6:");printf(_bme280_calib.dig_H6);
    printf("\r\n");
#endif
}

void begin3(int cs) {
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

    writeReg3(0xF2,ctrl_hum_reg);
    writeReg3(0xF4,ctrl_meas_reg);
    writeReg3(0xF5,config_reg);
    readTrim3();    
}


double readTemperature3(void) {
    signed long int temp_raw;
    signed long int var1, var2;
  
    uint8_t data[8];

    readReg3(0xFA, 3, data);

    temp_raw = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4);
  
    var1 = ((((temp_raw >> 3) - ((signed long int)_bme280_calib.dig_T1<<1))) * ((signed long int)_bme280_calib.dig_T2)) >> 11;
    var2 = (((((temp_raw >> 4) - ((signed long int)_bme280_calib.dig_T1)) * ((temp_raw>>4) - ((signed long int)_bme280_calib.dig_T1))) >> 12) * ((signed long int)_bme280_calib.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
  
    return ((t_fine * 5 + 128) >> 8) / 100.0;
}

double readPressure3(void) {
    unsigned long int pres_raw;
    signed long int var1, var2;
    unsigned long int P;
  
    uint8_t data[8];

    readReg3(0xF7, 3, data);

    pres_raw = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | ((uint32_t)data[2] >> 4);
  
    var1 = (((signed long int)t_fine)>>1) - (signed long int)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int)_bme280_calib.dig_P6);
    var2 = var2 + ((var1*((signed long int)_bme280_calib.dig_P5))<<1);
    var2 = (var2>>2)+(((signed long int)_bme280_calib.dig_P4)<<16);
    var1 = (((_bme280_calib.dig_P3 * (((var1>>2)*(var1>>2)) >> 13)) >>3) + ((((signed long int)_bme280_calib.dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int)_bme280_calib.dig_P1))>>15);
    if (var1 == 0) {
        return 0;
    }    
    P = (((unsigned long int)(((signed long int)1048576)-pres_raw)-(var2>>12)))*3125;
    if(P<0x80000000) {
       P = (P << 1) / ((unsigned long int) var1);   
    } else {
      P = (P / (unsigned long int)var1) * 2;    
    }
    var1 = (((signed long int)_bme280_calib.dig_P9) * ((signed long int)(((P>>3) * (P>>3))>>13)))>>12;
    var2 = (((signed long int)(P>>2)) * ((signed long int)_bme280_calib.dig_P8))>>13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + _bme280_calib.dig_P7) >> 4));
    return P / 100.0;
}

double readHumidity3(void) {
    unsigned long int hum_raw;
    signed long int v_x1;
  
    uint8_t data[8];

    readReg3(0xFD, 3, data);

    hum_raw  = ((uint32_t)data[0] << 8) | (uint32_t)data[1];
  
    v_x1 = (t_fine - ((signed long int)76800));
    v_x1 = (((((hum_raw << 14) -(((signed long int)_bme280_calib.dig_H4) << 20) - (((signed long int)_bme280_calib.dig_H5) * v_x1)) + 
              ((signed long int)16384)) >> 15) * (((((((v_x1 * ((signed long int)_bme280_calib.dig_H6)) >> 10) * 
              (((v_x1 * ((signed long int)_bme280_calib.dig_H3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) * 
              ((signed long int) _bme280_calib.dig_H2) + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)_bme280_calib.dig_H1)) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
    return (unsigned long int)(v_x1 >> 12) / 1024.0;   
}

// void writeReg3(uint8_t reg_address, uint8_t data) {
//     PORTC &= ~(1<<PC3);
// 	SPI_transmit(reg_address & 0x7F);
// 	SPI_transmit(data);
//     PORTC |= (1<<PC3);
//    
//    
//    
//    /* digitalWrite(bme280_cs, LOW);
//     SPI.transfer(reg_address & B01111111);
//     SPI.transfer(data);
//     digitalWrite(bme280_cs, HIGH);*/
// }
// 
// void readReg3(uint8_t reg_address, int numBytes, uint8_t * data) {
// 	uint8_t addr = reg_address | 0x80 ;
//     PORTC &= ~(1<<PC3);
//     SPI_transmit(addr);
// 	for (int i = 0; i < numBytes; i++) {
// 		data[i] = SPI_transmit(0x00);
// 	}
//     PORTC |= (1<<PC3);
//   /*  uint8_t addr = reg_address | B10000000; 0x80
//     digitalWrite(bme280_cs, LOW);
//     SPI.transfer(addr);
//     for (int i = 0; i < numBytes; i++) {
//         data[i] = SPI.transfer(0x00);
//     }
//     digitalWrite(bme280_cs, HIGH);*/
// }
// 
// void readTrim3() {
//     uint8_t data[32];//,i=0;
// 
//     readReg3(0x88, 24, data);
//     readReg3(0xA1,  1, &data[24]);
//     readReg3(0xE1,  7, &data[25]);
// 
//     _bme280_calib.dig_T1 = (data[1] << 8) | data[0];
//     _bme280_calib.dig_T2 = (data[3] << 8) | data[2];
//     _bme280_calib.dig_T3 = (data[5] << 8) | data[4];
//     _bme280_calib.dig_P1 = (data[7] << 8) | data[6];
//     _bme280_calib.dig_P2 = (data[9] << 8) | data[8];
//     _bme280_calib.dig_P3 = (data[11]<< 8) | data[10];
//     _bme280_calib.dig_P4 = (data[13]<< 8) | data[12];
//     _bme280_calib.dig_P5 = (data[15]<< 8) | data[14];
//     _bme280_calib.dig_P6 = (data[17]<< 8) | data[16];
//     _bme280_calib.dig_P7 = (data[19]<< 8) | data[18];
//     _bme280_calib.dig_P8 = (data[21]<< 8) | data[20];
//     _bme280_calib.dig_P9 = (data[23]<< 8) | data[22];
//     _bme280_calib.dig_H1 = data[24];
//     _bme280_calib.dig_H2 = (data[26]<< 8) | data[25];
//     _bme280_calib.dig_H3 = data[27];
//     _bme280_calib.dig_H4 = (data[28]<< 4) | (0x0F & data[29]);
//     _bme280_calib.dig_H5 = (data[30] << 4) | ((data[29] >> 4) & 0x0F);
//     _bme280_calib.dig_H6 = data[31];   
// 
// #if 0
//     printf("T1:");printf(_bme280_calib.dig_T1);
//     printf(", T2:");printf(_bme280_calib.dig_T2);
//     printf(", T3:");printf(_bme280_calib.dig_T3);
//     printf("\r\n");
//     printf("P1:");printf(_bme280_calib.dig_P1);
//     printf(", P2:");printf(_bme280_calib.dig_P2);
//     printf(", P3:");printf(_bme280_calib.dig_P3);
//     printf(", P4:");printf(_bme280_calib.dig_P4);
//     printf(", P5:");printf(_bme280_calib.dig_P5);
//     printf(", P6:");printf(_bme280_calib.dig_P6);
//     printf(", P7:");printf(_bme280_calib.dig_P7);
//     printf(", P8:");printf(_bme280_calib.dig_P8);
//     printf(", P9:");printf(_bme280_calib.dig_P9);
//     printf("\r\n");
//     printf("H1:");printf(_bme280_calib.dig_H1);
//     printf(", H2:");printf(_bme280_calib.dig_H2);
//     printf(", H3:");printf(_bme280_calib.dig_H3);
//     printf(", H4:");printf(_bme280_calib.dig_H4);
//     printf(", H5:");printf(_bme280_calib.dig_H5);
//     printf(", H6:");printf(_bme280_calib.dig_H6);
//     printf("\r\n");
// #endif
// }


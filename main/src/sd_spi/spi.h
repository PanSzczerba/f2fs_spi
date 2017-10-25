#ifndef SPI_WPI_H
#define SPI_WPI_H 

//for error return codes in wPi setup function
#ifndef WIRINGPI_CODES
#define WIRINGPI_CODES
#endif

#include <stddef.h>
#include <wiringPi.h>

#define PWR 7
#define SCLK 5 
#define CS 1
#define MOSI 4
#define MISO 6

extern int spi_initialized; // to know if pins were initialized - don't change manually
int setup_spi();
int spi_read_write(unsigned char* buff, size_t buff_size);

#endif

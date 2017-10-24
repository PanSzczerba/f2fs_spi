#ifndef SPI_WPI_H
#define SPI_WPI_H 

//for error return codes in wPi setup function
#ifndef WIRINGPI_CODES
#define WIRINGPI_CODES
#endif

#include <stddef.h>
#include <wiringPi.h>

#define SCLK 1
#define CS 4
#define MOSI 5
#define MISO 6

extern int spi_initialized; // to know if pins were initialized - don't change manually
int setup_spi();
int spi_read_write(char* buff, size_t buff_size);

#endif

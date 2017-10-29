#ifndef SPI_WPI_H
#define SPI_WPI_H 

//for error return codes in wPi setup function
#ifndef WIRINGPI_CODES
#define WIRINGPI_CODES
#endif

#include <stddef.h>
#include <stdint.h>
#include <wiringPi.h>

#define PWR 7
#define SCLK 5 
#define CS 1
#define MOSI 4
#define MISO 6

extern int initialized;

int setup_spi();
void power_off();
int spi_read_write(uint8_t* buff, size_t buff_size);

#endif

#ifndef SPI_WPI_H
#define SPI_WPI_H 

//for error return codes in wPi setup function
#ifndef WIRINGPI_CODES
#define WIRINGPI_CODES
#endif

#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>

#define PWR 7
#define SCLK 5 
#define CS 1
#define MOSI 4
#define MISO 6

#define N_CYCLES_TIMEOUT 512

extern int configured_pins;
extern useconds_t udelay;
int configure_pins();
int spi_read_write(uint8_t* buff, size_t buff_size);

#endif

#ifndef SPI_WPI_H
#define SPI_WPI_H 

//for error return codes in wPi setup function
#ifndef WIRINGPI_CODES
#define WIRINGPI_CODES
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <wiringPi.h>

#define PWR 7
#define SCLK 5 
#define CS 1
#define MOSI 4
#define MISO 6

#define N_CYCLES_TIMEOUT 512

/********POSSIBLE R1 VALUES*************/
#define R1_IN_IDLE_STATE (uint8_t)0x1
#define R1_ILLEGAL_COMMAND (uint8_t)0x4
/***************************************/


/********SD VERSION MACROS**************/
#define SD_V1X 1 
#define SD_V2X 2
#define SD_V2XHCXC 3
/***************************************/


/**********POSSIBLE STATES**************/
#define INITIALIZE_SPI 0
/***************************************/

#define BIN_DEBUG(x)    for(uint8_t shift = 0x80; shift > 0; shift >>= 1) \
                        { \
                             if(shift & (x)) \
                                 printf("1"); \
                             else \
                                 printf("0"); \
                        } 

extern int initialized;

int setup_spi();
void power_off();
int spi_read_write(uint8_t* buff, size_t buff_size);

#endif

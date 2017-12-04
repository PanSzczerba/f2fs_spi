#ifndef SD_SPI_H
#define SD_SPI_H

#include <stdint.h>

/********POSSIBLE R1 VALUES*************/
#define R1_IN_IDLE_STATE (uint8_t)0x1
#define R1_ILLEGAL_COMMAND (uint8_t)0x4
/***************************************/

/********SD VERSION MACROS**************/
#define SD_NOT_DETERMINED 0 
#define SD_V1X 1 
#define SD_V2X 2
#define SD_V2XHCXC 3
/***************************************/

#define BIN_DEBUG(x)    for(uint8_t shift = 0x80; shift > 0; shift >>= 1) \
                        { \
                             if(shift & (x)) \
                                 printf("1"); \
                             else \
                                 printf("0"); \
                        } 

extern int spi_initialized;
extern int sd_version;
extern uint8_t csd_register[16];


typedef struct block512
{
    uint8_t data[512];
} block512;


int setup_spi();
void power_off();
int read_single_block(uint32_t index, block512*); //block index should be 32-bit unsigned integer
int write_single_block(uint32_t index, block512); //block index should be 32-bit unsigned integer
int read_multiple_block(uint32_t index, block512* buff, size_t size);
int write_multiple_block(uint32_t index, block512* buff, size_t size);

#endif

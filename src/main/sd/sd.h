#ifndef SD_SPI_H
#define SD_SPI_H

#include <stdint.h>

#define SECTOR_SIZE 512

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

/*
int read_single_block(uint32_t index, block512*); //block index should be 32-bit unsigned integer
int write_single_block(uint32_t index, block512); //block index should be 32-bit unsigned integer
int read_multiple_block(uint32_t index, block512* buff, size_t size);
int write_multiple_block(uint32_t index, block512* buff, size_t size);
*/

int read_blocks(uint32_t index, block512* buff, size_t size);
int write_blocks(uint32_t index, block512* buff, size_t size);


/////////////////////////////////// mbr ///////////////////////////////////////

#define __packed __attribute__((packed))

struct partition_entry_t
{
    uint8_t status;
    uint8_t first_sector_CHS[3];
    uint8_t partition_type;
    uint8_t last_sector_CHS[3];
    uint32_t first_sector_LBA;
    uint32_t sector_no;
} __packed;


struct mbr_t
{
    uint8_t bootstrap_code_area[446];
    struct partition_entry_t partition_entry[4];
    uint16_t boot_signature;
} __packed;

int read_mbr(struct mbr_t*);

#endif

#ifndef RW_BLOCK_H
#define RW_BLOCK_H

#include <string.h>

typedef struct block512
{
    uint8_t data[512];
} block512;


int read_single_block(uint32_t index, block512*); //block index should be 32-bit unsigned integer
int write_single_block(uint32_t index, block512); //block index should be 32-bit unsigned integer
#endif

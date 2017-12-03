#include "spi.h"
#include "rw_block.h"
#include <string.h>
#include <stdio.h>

void display_buffer(size_t block_address, block512 buff)
{
    for(size_t i = 0; i < 32; i++)
    {
        printf("%03x: ", block_address + i*16);
        for(size_t j = 0; j < 16; j++)
            printf("%02x ", buff.data[i * 16 + j]);
        printf("\n");

    } 
}
int main()
{
    setup_spi();
    block512 buff;
    
    read_single_block(0, &buff);

    printf("Single block read\n\n");
    display_buffer(0, buff);
    printf("\n");

    memset(buff.data, 0xf2, 512);
    write_single_block(256000, buff);
    read_single_block(256000, &buff);

    printf("Single block write and read\n");
    display_buffer(256000, buff);
    printf("\n");

    block512 buff_array[10];
    read_multiple_block(255488, buff_array, 10);
   
    printf("Multiple block read\n");
    for(size_t j = 0; j < 10; j++)
    {
       display_buffer(255488 + j*512, buff_array[j]);
    }
    printf("\n");

    for(int i = 0; i < 10; i++)
        memset(buff_array[i].data, 0xa7, 512);
    write_multiple_block(255488, buff_array, 10);

    for(int i = 0; i < 10; i++)
        memset(buff_array[i].data, 0x0, 512);
    
    read_multiple_block(255488, buff_array, 10);
    
    printf("Multiple block write and read\n");
    for(size_t j = 0; j < 10; j++)
    {
        display_buffer(255488 + j*512, buff_array[j]);
    }

    power_off();
}

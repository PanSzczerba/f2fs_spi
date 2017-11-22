#include "spi.h"
#include "rw_block.h"
#include <stdio.h>

int read_single_block(uint32_t index, block512* buff)
{
    uint8_t command[] = {0x51, (uint8_t)(index >> 24), (uint8_t)(index >> 16), (uint8_t)(index >> 8), (uint8_t)index, 0x1 }; // CMD17 
    uint8_t response = 0xff;
    memset(buff->data, 0xff, 512);
    size_t i;

    spi_read_write(&response, 1);

    spi_read_write(command, 6);

    for(i = 0; i < N_CYCLES_TIMEOUT && response == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        response = 0xff;
        spi_read_write(&response, 1);
    }

    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        return 1;
    }
/*
    printf("CMD17 - R1 response:\n");
    BIN_DEBUG(response);
*/
    if(response != 0) // response isn't what it was expected (R1 with idle state bit on)
    {
        fprintf(stderr, "Error: can't read block\n"); 
        return 1;
    }
    
    response = 0xff;
    for(i = 0; i < N_CYCLES_TIMEOUT && response == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        response = 0xff;
        spi_read_write(&response, 1);
    }
    
    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        return 1;
    }

//  BIN_DEBUG(response)
    if(response != 0xfe)
    {
        //DEGBUG
        fprintf(stderr, "Error: sth wrong :c\n"); 
    }

    spi_read_write(buff->data, 512);

    response = 0xff; //read CRC, thou it's not needed
    spi_read_write(&response, 1);
//  BIN_DEBUG(response)

    response = 0xff;
    spi_read_write(&response, 1);
//  BIN_DEBUG(response)

    response = 0xff;
    spi_read_write(&response, 1);
//  BIN_DEBUG(response)

    return 1;
}

int write_single_block(uint32_t index, block512 buff)
{
    uint8_t command[] = {0x58, (uint8_t)(index >> 24), (uint8_t)(index >> 16), (uint8_t)(index >> 8), (uint8_t)index, 0x1 }; // CMD17 
    uint8_t response = 0xff;
    size_t i;

    spi_read_write(&response, 1);

    spi_read_write(command, 6);

    for(i = 0; i < N_CYCLES_TIMEOUT && response == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        response = 0xff;
        spi_read_write(&response, 1);
    }

    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        return 1;
    }

    printf("CMD24 - R1 response:\n");
//  BIN_DEBUG(response);

    if(response != 0) // response isn't what it was expected (R1 with idle state bit on)
    {
        fprintf(stderr, "Error: can't write block\n"); 
        return 1;
    }

    response = 0xff;
    spi_read_write(&response, 1);

    response = 0xfe;
    spi_read_write(&response, 1); //send data token
    
    spi_read_write(buff.data, 512);

    response = 0xff; 
    spi_read_write(&response, 1);
    for(i = 0; i < N_CYCLES_TIMEOUT && response == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        response = 0xff;
        spi_read_write(&response, 1);
    }

    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        return 1;
    }

    response = 0;

    while(response != 0xff)// wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        response = 0xff;
        spi_read_write(&response, 1);
    }

    return 0;
}

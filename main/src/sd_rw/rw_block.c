#include "rw_block.h"
#include <unistd.h>
#include <stdio.h>

int sd_init()
{
    if(!spi_initialized)
        setup_spi();

    usleep(1000);    

    digitalWrite(MOSI, HIGH);
    for(size_t i = 0; i < 74; i++)
    {
        digitalWrite(SCLK, HIGH);
        digitalWrite(SCLK, LOW);
    }
    
    char command[] = "\x40\x0\x0\x0\x0\x95";
    char response;
    spi_read_write(command, 6);

    for(size_t i = 0; i < 10; i++)
    {
        response = '\xff';
        spi_read_write(&response, 1);
        for(unsigned char shift = 0x80; shift > 0; shift >>= 1)
        {
            if(shift & response)
                printf("1");
            else
                printf("0");
        }
        printf("\n");

    }

}

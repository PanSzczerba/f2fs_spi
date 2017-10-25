#include "rw_block.h"
#include <unistd.h>
#include <stdio.h>

#define BIN_DEBUG(x)    for(unsigned char shift = 0x80; shift > 0; shift >>= 1) \
                        { \
                             if(shift & x) \
                                 printf("1"); \
                             else \
                                 printf("0"); \
                        } \
                        printf("\n"); 

int sd_init()
{
    if(setup_spi())
    {
        //debugging
    }

    usleep(1000);    

    digitalWrite(MOSI, HIGH);
    for(size_t i = 0; i < 74; i++)
    {
        digitalWrite(SCLK, HIGH);
        digitalWrite(SCLK, LOW);
    }
    
    char command[] = "\x40\x0\x0\x0\x0\x95";
    char response = '\xff';
    spi_read_write(command, 6);

    size_t i;
    for(i = 0; i < 10 && response == '\xff'; i++)
    {
        response = '\xff';
        spi_read_write(&response, 1);
        BIN_DEBUG(response);
    }

    if(i == 10 || response != '\x1')
    {
        digitalWrite(PWR, 0);
        return 1;
    }

    response = '\xff';
    spi_read_write(&response, 1);


    digitalWrite(PWR, 0);
}

#include "spi.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int configured_pins = 0;
useconds_t udelay = 0;

int configure_pins()
{
///////////////////// PIN INICIALIZATION ////////////////////////
    if(configured_pins)
    {
        fprintf(stderr, "Error: pins have been already configured\n");
        return 0;
    }

    if(!wiringPiSetup()) //returns 0 if everything is ok
    {
        usleep(1000);

        pinMode(PWR, OUTPUT);
        pinMode(SCLK, OUTPUT); //SCLK
        pinMode(CS, OUTPUT); //CS
        pinMode(MOSI, OUTPUT); //MOSI
        pinMode(MISO, INPUT);  //MISO
        //add some PU-PD resistors control
        pullUpDnControl(MISO, PUD_UP);

        digitalWrite(PWR, HIGH);
        digitalWrite(SCLK, LOW);
        digitalWrite(CS, HIGH); // if it is low slave would start counting clock ticks
        digitalWrite(MOSI, LOW);
        configured_pins = 1;
    }
    else
    {
        fprintf(stderr, "Error: couldn't initialize witingPi setup\n");
        return 1;
    }

////////////////// END OF PIN INICIALIZATION ///////////////////////
    return 0;
}

int spi_read_write(uint8_t* buff, size_t buff_size)
{

    if(!configured_pins)
    {
        fprintf(stderr, "Error: can't read - pins have not been configured\n");
        return 1;
    }
    digitalWrite(CS, LOW); // start write sequence 
    for(size_t i = 0; i < buff_size; i++)
    {
        for(uint8_t shift = 0x80; shift > 0; shift >>= 1) // 0x80 = 0b10000000
        {
            digitalWrite(MOSI, (shift & buff[i]) ? HIGH : LOW); //set MOSI to corresponding value in buffer[0]
            if(digitalRead(MISO)) // read from MISO, set correspoding bit to 0 or 1
                buff[i] |= shift; 
            else
                buff[i] &= ~shift;

            digitalWrite(SCLK, HIGH); // latch MOSI value
//          usleep(udelay);
            digitalWrite(SCLK, LOW); // shift values in internal register
        }
    }
    digitalWrite(CS, HIGH); // end write sequence

    return 0;
}

#include "spi.h"
#include <stdlib.h>

static int spi_initialized = 0;

int setup_spi()
{
    if(spi_initialized) // if it was already initialized
        return 0;

   if(!wiringPiSetup()) //returns 0 if everything is ok
   {
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

       spi_initialized = 1;
       return 0;
   }
   else
   {
       //add some debugging here
       return 1;
   }
}

int spi_read_write(unsigned char* buff, size_t buff_size)
{
    if(!spi_initialized)
        setup_spi();

    digitalWrite(CS, LOW); // start write sequence 
    for(size_t i = 0; i < buff_size; i++)
    {
        for(unsigned char shift = 0x80; shift > 0; shift >>= 1) // 0x80 = 0b10000000
        {
            digitalWrite(MOSI, (shift & buff[i]) ? HIGH : LOW); //set MOSI to corresponding value in buffer
            if(digitalRead(MISO)) // read from MISO, set correspoding bit to 0 or 1
                buff[i] |= shift; 
            else
                buff[i] &= ~shift;

            digitalWrite(SCLK, HIGH); // latch MOSI value
            //maybe some delay?
            digitalWrite(SCLK, LOW); // shift values in internal register
        }
    }
    digitalWrite(CS, HIGH); // end write sequence

    return 0;
}

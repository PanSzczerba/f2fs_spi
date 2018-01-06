#include "spi.h"
#include "sd.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int spi_initialized = 0;
int sd_version = SD_NOT_DETERMINED;
uint8_t csd_register[16];

int setup_spi()
{
    
    if(spi_initialized) // if it was already initialized
        return 0;

    if(configure_pins())
    {
        fprintf(stderr, "Error: pin initialization failed\n");
        return 1;
    }

    usleep(1000); // wait 1ms 

    digitalWrite(MOSI, HIGH);
    for(size_t i = 0; i < 74; i++) // 74 dummy clock cycles
    {
        digitalWrite(SCLK, HIGH);
        digitalWrite(SCLK, LOW);
    }

/////////////// SENDING CMD0 AND PARSING RESPONSE //////////////////

    uint8_t command[] = { 0x40, 0x0, 0x0, 0x0, 0x0, 0x95 }; // CMD0 command with precalculated CRC7
    spi_read_write(command, 6); // send CMD0

    uint8_t buffer[] = { 0xff, 0xff, 0xff, 0xff, 0xff };

    size_t i;
    for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        buffer[0] = 0xff;
        spi_read_write(buffer, 1);
    }

    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        reset_pins();
        return 1;
    }
/*
    printf("CMD0 - R1 response:\n");
    BIN_DEBUG(buffer[0]);
*/
    if(buffer[0] != R1_IN_IDLE_STATE) // response isn't what it was expected (R1 with idle state bit on)
    {
        fprintf(stderr, "Error: not an SD card\n"); 
        reset_pins();
        return 1;
    }

    buffer[0] = 0xff;
    spi_read_write(buffer, 1); // after R1 response wait for 8 clock cycles (for safety)

/////////////// END OF PARSING CMD0 RESPONSE ///////////////////////


/////////////// SENDING CMD8 AND PARSING RESPONSE //////////////////

    command[0] = 0x48; command[1] = 0x0;  command[2] = 0x0;
    command[3] = 0x1;  command[4] = 0xaa; command[5] = 0x87; // CMD8 with precalculated CRC7

    spi_read_write(command, 6);


    for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R7 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        buffer[0] = 0xff;
        spi_read_write(buffer, 1);
    }
   
    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        reset_pins();
        return 1;
    }

    if(buffer[0] & R1_ILLEGAL_COMMAND) //if SDv1.x
    {
        sd_version = SD_V1X; // doesn't recognize CMD8 - it's either SD version 1.x or not SD card at all
/*
        printf("\nCMD8 - R1 response:\n");
        BIN_DEBUG(buffer[0]);
*/
    }
    else // SDv2.x | SDHC/SDXCv2.x (determined in later steps)
    {
        spi_read_write(buffer + 1, 4);
/*
        printf("\nCMD8 - R7 response:\n");
        for(size_t j = 0; j < 5; j++)
            BIN_DEBUG(buffer[j]);
*/
    }

    buffer[0] = 0xff;
    spi_read_write(buffer, 1); // after R7/R1 response wait for 8 clock cycles (for safety)

////////////////// END OF PARSING CMD8 RESPONSE///////////////////////


/////////////// SENDING CMD58 AND PARSING RESPONSE //////////////////

    command[0] = 0x7a; command[1] = 0x0;  command[2] = 0x0;
    command[3] = 0x0;  command[4] = 0x0; command[5] = 0xfd; // CMD58 with precalculated CRC7
    
    
    spi_read_write(command, 6);
    buffer[0] = 0xff;
    for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R3 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        buffer[0] = 0xff;
        spi_read_write(buffer, 1);
    }

    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        reset_pins();
        return 1;
    }
    
    if(buffer[0] & R1_ILLEGAL_COMMAND) //not SD card 
    {
        fprintf(stderr, "Error: not an SD card\n");
        reset_pins();
        return 1;
    }
    
    for(size_t i = 1; i < 5; i++) buffer[i] = 0xff;

    spi_read_write(buffer + 1, 4);

/*
    printf("\nCMD58 - R3 response:\n");
    for(size_t j = 0; j < 5; j++)
        BIN_DEBUG(buffer[j]);
*/

////////////// END OF PARSING CMD58 RESPONSE ////////////////////////
    

////////////// SENDING ACMD41 AND PARSING RESPONSE //////////////////

    buffer[0] = 0xff;
    while(buffer[0] & R1_IN_IDLE_STATE)
    {
        buffer[0] = 0xff;
        spi_read_write(buffer, 1); // after response wait for 8 clock cycles (for safety)

        command[0] = 0x77; command[1] = 0x0;  command[2] = 0x0;
        command[3] = 0x0;  command[4] = 0x0; command[5] = 0x65; // CMD55 with precalculated CRC7 (APP_COMMAND)

        spi_read_write(command, 6);

        buffer[0] = 0xff;
        for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
        {
            buffer[0] = 0xff;
            spi_read_write(buffer, 1);
        }

        if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles 
        {
            fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
            reset_pins();
            return 1;
        }
/*
        printf("\nCMD55 - R1 response:\n");
        BIN_DEBUG(buffer[0]);
*/
        if(buffer[0] & R1_ILLEGAL_COMMAND) //not SD card 
        {
            fprintf(stderr, "Error: not an SD card\n");
            reset_pins();
            return 1;
        }
        
        buffer[0] = 0xff;
        spi_read_write(buffer, 1); // after R1 response wait for 8 clock cycles (for safety)
        
        command[0] = 0x69; command[1] = 0x40;  command[2] = 0x0;
        command[3] = 0x0;  command[4] = 0x0; command[5] = 0x77; // ACMD41 with precalculated CRC7 

        spi_read_write(command, 6);
        buffer[0] = 0xff;
        for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R1 response for N_CYCLES_TIMEOUT*8  clock cycles
        {
            buffer[0] = 0xff;
            spi_read_write(buffer, 1);
        }

        if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles 
        {
            fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
            reset_pins();
            return 1;
        }
/*
        printf("\nACMD41 - R1 response:\n");
        BIN_DEBUG(buffer[0]);
*/        
    }

////////////////// END OF SENDING ACMD41 /////////////////////////

///////////////////// CHECK SD VERSION ///////////////////////////
// if inserted card isn't SD version 1.X check if it's normal capacity SD or SDHC/SDXC

    if(sd_version != SD_V1X)
    {
        command[0] = 0x7a; command[1] = 0x0;  command[2] = 0x0;
        command[3] = 0x0;  command[4] = 0x0; command[5] = 0xfd; // CMD58 with precalculated CRC7
        
        spi_read_write(command, 6);

        buffer[0] = 0xff;
        for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R3 response for N_CYCLES_TIMEOUT*8  clock cycles
        {
            buffer[0] = 0xff;
            spi_read_write(buffer, 1);
        }

        if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
        {
            fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
            reset_pins();
            return 1;
        }
        
        for(size_t i = 1; i < 5; i++) buffer[i] = 0xff;

        spi_read_write(buffer + 1, 4);
/*
        printf("\nR3 response:\n");
        for(size_t j = 0; j < 5; j++)
            BIN_DEBUG(buffer[j]);
*/
        if(buffer[1] & (uint8_t)0x40) //check OCR register CCS property
            sd_version = SD_V2XHCXC;
        else
            sd_version = SD_V2X;
    }

////////////////////////////////////////////////////////////////////

////////////////////////READ CSD REGISTER///////////////////////////

    buffer[0] = 0xff;
    spi_read_write(buffer, 1);

    command[0] = 0x49; //CMD9
    spi_read_write(command, 6);

    buffer[0] = 0xff;
    for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R3 response for N_CYCLES_TIMEOUT*8  clock cycles
    {
        buffer[0] = 0xff;
        spi_read_write(buffer, 1);
    }

    if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
    {
        fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
        reset_pins();
        return 1;
    }

    if(buffer[0] != 0)
    {
        fprintf(stderr, "Error: reading CSD register failed\n");
    }
    else
    {
        buffer[0] = 0xff;
        for(i = 0; i < N_CYCLES_TIMEOUT && buffer[0] == 0xff; i++) // wait for R3 response for N_CYCLES_TIMEOUT*8  clock cycles
        {
            buffer[0] = 0xff;
            spi_read_write(buffer, 1);
        }

        if(i == N_CYCLES_TIMEOUT) //if it didn't get response for N_CYCLES_TIMEOUT*8 clock cycles    
        {
            fprintf(stderr, "Error: exceeded time limit waiting for response, check your SD card reader device \n");
            reset_pins();
            return 1;
        }

        memset(csd_register, 0xff, 16); // set all bits to 1 (for reading CSD register)

        spi_read_write(csd_register, 16);

        buffer[0] = 0xff;
        buffer[1] = 0xff;
        spi_read_write(buffer, 2); //discard CRC
    }

    buffer[0] = 0xff;
    spi_read_write(buffer, 1);

////////////////////END OF READING CSD REGISTER/////////////////////

    return 0; // if everything went well it will return 0 - 1 otherwise
}

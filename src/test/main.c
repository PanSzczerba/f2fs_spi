#include "spi.h"
#include "rw_block.h"
#include <string.h>
#include <stdio.h>

int main()
{
    setup_spi();
    block512 buff;
    memset(buff.data, 0xf2, 512);
    
    write_single_block(256000, buff);
    read_single_block(256000, &buff);

    for(size_t i = 0; i < 512; i++)
        BIN_DEBUG(buff.data[i])
    printf("\n");

    power_off();
}

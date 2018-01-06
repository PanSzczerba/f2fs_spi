#include "spi.h"
#include "sd.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int read_mbr(struct mbr_t* mbr)
{
    read_blocks(0, (block512 *) &mbr, 1);
}
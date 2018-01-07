#include "f2fs.h"
#include "sd.h"
#include <stdio.h>

void display_nat_entry(struct f2fs_nat_entry* ne) {
    printf("+----------------------------------------------+\n");
    printf("| NAT Entry                                    |\n");
    printf("+----------------------------------------------+\n");
    printf("version                                   [0x%x]\n", ne->version);
    printf("ino                                       [0x%x]\n", ne->ino);
    printf("block_addr                                [0x%x]\n", ne->block_addr);
}


int get_nat_entry(struct f2fs_nat_entry* ne, uint32_t address)
{
    block512_t blk;
    read_blocks(address, &blk, 1);

    void* vp = (void*)&blk;
    *ne = *((struct f2fs_nat_entry*)vp);
    return 0;
}
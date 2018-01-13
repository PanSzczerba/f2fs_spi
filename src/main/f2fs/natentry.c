#include "f2fs.h"
#include "sd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display_nat_entry(struct f2fs_nat_entry* ne) {
    printf("+----------------------------------------------+\n");
    printf("| NAT Entry                                    |\n");
    printf("+----------------------------------------------+\n");
    printf("version                                   [0x%x]\n", ne->version);
    printf("ino                                       [0x%x]\n", ne->ino);
    printf("block_addr                                [0x%x]\n", ne->block_addr);
}

int get_nat_block(struct f2fs_meta_data* md, size_t nat_block_no)
{
    if(md->nat_block_no == nat_block_no)
        return 0;
    else
    {
        block512_t blocks[F2FS_BLOCK_SIZE/BLOCK_SIZE];
        read_blocks(md->partition_block_address*BLOCK_SIZE + (md->sb.nat_blkaddr + nat_block_no)*F2FS_BLOCK_SIZE, blocks, F2FS_BLOCK_SIZE/BLOCK_SIZE);
        void* vp = (void*)blocks;
        md->nat_blk = *((struct f2fs_nat_block*)vp);
        md->nat_block_no = nat_block_no;

        for(size_t i = 0; i < md->chkp_nat_journal.entries_no; i++)
        {
            if(NAT_ENTRY_PER_BLOCK * md->nat_block_no <= md->chkp_nat_journal.entries[i].nat_entry_no && md->chkp_nat_journal.entries[i].nat_entry_no < NAT_ENTRY_PER_BLOCK* (md->nat_block_no + 1))
            {
                md->nat_blk.entries[md->chkp_nat_journal.entries[i].nat_entry_no - NAT_ENTRY_PER_BLOCK * md->nat_block_no] = md->chkp_nat_journal.entries[i].entry;
            }
        }
        return 0;
    }
}

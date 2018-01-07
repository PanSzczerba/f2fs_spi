#include "spi.h"
#include "sd.h"
#include "f2fs.h"
#include <string.h>
#include <stdio.h>

void display_buffer(size_t block_address, block512_t buff)
{
    for(size_t i = 0; i < 32; i++)
    {
        printf("%03x: ", block_address + i*16);
        for(size_t j = 0; j < 16; j++)
            printf("%02x ", buff.data[i * 16 + j]);
        printf("\n");
    } 
}

int main()
{
    setup_spi();

    printf("CSD register:\n");
    for(size_t j = 0; j < 16; j++)
        printf("%02x ", csd_register[j]);
    printf("\n\n");
    
    struct mbr_t mbr;
    read_mbr(&mbr);

    display_mbr(&mbr);
    //display_buffer(0, (block512_t)mbr);
    printf("\n");

/*
    block512_t buff;
    memset(buff.data, 0xf2, 512);
    write_blocks(256000, &buff, 1);
    read_blocks(256000, &buff, 1);

    printf("Single block write and read\n");
    display_buffer(256000, buff);
    printf("\n");

    block512_t buff_array[10];
    read_blocks(255488, buff_array, 10);
   
    printf("Multiple block read\n");
    for(size_t j = 0; j < 10; j++)
    {
       display_buffer(255488 + j*512, buff_array[j]);
    }
    printf("\n");

    for(int i = 0; i < 10; i++)
        memset(buff_array[i].data, 0xa7, 512);
    write_blocks(255488, buff_array, 10);

    for(int i = 0; i < 10; i++)
        memset(buff_array[i].data, 0x01, 512);
    
    write_blocks(255488, buff_array, 10);
    
    printf("Multiple block write and read\n");
    for(size_t j = 0; j < 10; j++)
    {
        display_buffer(255488 + j*512, buff_array[j]);
    }
*/
    struct f2fs_meta_data md;
    md.partition_block_address = mbr.partition_entry[0].first_sector_LBA;
    get_super_block(&md); 
    super_block_display(&(md.sb));

    get_checkpoint(&md);
    checkpoint_display(&(md.chkp));
    printf("\n");

    block512_t buff_array[64];
    read_blocks(mbr.partition_entry[0].first_sector_LBA*BLOCK_SIZE + md.sb.nat_blkaddr*F2FS_BLOCK_SIZE, buff_array, 64);
   
    for(size_t j = 0; j < 64; j++)
       display_buffer(mbr.partition_entry[0].first_sector_LBA*BLOCK_SIZE + md.sb.nat_blkaddr*F2FS_BLOCK_SIZE + j*512, buff_array[j]);

    struct f2fs_nat_entry nat_entries[4];
    get_nat_entries(&md, nat_entries, 0, 4);
    printf("\n%x - %x\n", nat_entries[3].ino, nat_entries[3].block_addr);


///////////////////CLEANUP///////////
    reset_pins();
}

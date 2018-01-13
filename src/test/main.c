#include "spi.h"
#include "sd.h"
#include "f2fs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS_TO_READ 128

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

void display_blocks(size_t block_address, size_t block_no)
{
    block512_t* buff;
    buff = (block512_t*)malloc(sizeof(block512_t)*block_no);
    read_blocks(block_address, buff, block_no);
    for(size_t k = 0; k < block_no; k++)
    {
        for(size_t i = 0; i < 32; i++)
        {
            printf("%03x: ", block_address + k*BLOCK_SIZE + i*16);
            for(size_t j = 0; j < 16; j++)
                printf("%02x ", buff[k].data[i * 16 + j]);
            printf("\n");
        } 
    }
}

void display_segment(size_t block_address)
{
    size_t segment_address = (block_address/F2FS_SEGMENT_SIZE) * F2FS_SEGMENT_SIZE;
    for(size_t i = 0; i < F2FS_SEGMENT_SIZE/(BLOCK_SIZE*MAX_BLOCKS_TO_READ); i++)
        display_blocks(segment_address + i*BLOCK_SIZE*MAX_BLOCKS_TO_READ, MAX_BLOCKS_TO_READ);
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
    display_super_block(&(md.sb));

    get_checkpoint(&md);
    display_checkpoint(&(md.chkp));
    printf("%d\n", md.chkp_nat_journal.entries_no);

    md.nat_block_no = (uint16_t)-1;
    get_nat_block(&md, 0);
//    get_nat_entries(&md, nat_entries, 0, md.chkp.next_free_nid);
    for(size_t j = 0; j < md.chkp.next_free_nid; j++)
        display_nat_entry(&md.nat_blk.entries[j]);

    struct f2fs_inode inode;

    get_inode(&md, &inode, md.sb.root_ino);

    display_inode(&inode);

    printf("\n");
    for(size_t i = 0; i < DEF_ADDRS_PER_INODE; i++)
        if(inode.i_addr[i] != 0)
            printf("0x%x\n", inode.i_addr[i]);
    printf("\n");

    block512_t buff_array[64];
    read_blocks(mbr.partition_entry[0].first_sector_LBA*BLOCK_SIZE + (inode.i_addr[0])*F2FS_BLOCK_SIZE, buff_array, 64);
    struct f2fs_dentry_block db;
    memcpy(&db, buff_array, sizeof(struct f2fs_dentry_block));

//  printf("SIZE_OF_DENTRY_BITMAP %d\n", SIZE_OF_DENTRY_BITMAP);
//  printf("SIZE_OF_RESERVED %d\n", SIZE_OF_RESERVED);
//  printf("NR_DENTRY_IN_BLOCK %d\n", NR_DENTRY_IN_BLOCK);
//  printf("F2FS_SLOT_LEN %d\n", F2FS_SLOT_LEN);

//  printf("%d\n", sizeof(struct f2fs_dir_entry));
//  printf("%d\n", sizeof(struct f2fs_dentry_block));
    printf("0x%x\n", db.dentry_bitmap[0]);
    printf("\n");
    for(size_t i = 0; i < NR_DENTRY_IN_BLOCK; i++)
    {
        if(db.dentry_bitmap[i/BITS_PER_BYTE] & (uint8_t)(1 << (i%BITS_PER_BYTE)))
            if(db.dentry[i].name_len != 0)
                printf("%.*s\n", db.dentry[i].name_len, (char*)db.filename[i]);
    }
    printf("\n");

//  read_blocks(mbr.partition_entry[0].first_sector_LBA*BLOCK_SIZE + (inode.i_addr[0])*F2FS_BLOCK_SIZE, buff_array, 64);
//  printf("\n");
//  for(size_t j = 0; j < 64; j++)
//      display_buffer(mbr.partition_entry[0].first_sector_LBA*BLOCK_SIZE + (inode.i_addr[0])*F2FS_BLOCK_SIZE + j*512, buff_array[j]);
//  printf("\n\n");
//  display_blocks(md.partition_block_address*BLOCK_SIZE + md.sb.nat_blkaddr*F2FS_BLOCK_SIZE, 64);
    display_blocks(md.partition_block_address*BLOCK_SIZE + inode.i_addr[0] * F2FS_BLOCK_SIZE, 64);

///////////////////CLEANUP///////////
    reset_pins();
}

#include "f2fs.h"
#include "sd.h" 
#include <stdio.h>
struct partition_entry_t 
{
    uint8_t status;
    uint8_t first_sector_CHS[3]; 
    uint8_t partition_type;
    uint8_t last_sector_CHS[3];
    uint32_t first_sector_LBA;
    uint32_t sector_no;
} __packed;


struct mbr_t
{
    uint8_t bootstrap_code_area[446];
    struct partition_entry_t partition_entry[4]; 
    uint16_t boot_signature;
} __packed;

void super_block_display(struct f2fs_super_block* sb) 
{
    printf("+----------------------------------------------+\n");
    printf("| Super block                                  |\n");
    printf("+----------------------------------------------+\n");
    printf("magic                                   [0x%x]\n",sb->magic);
    printf("major_ver                               [0x%x]\n",sb->major_ver);
    printf("minor_ver                               [0x%x]\n",sb->minor_ver);
    printf("log_sectorsize                          [0x%x]\n",sb->log_sectorsize);
    printf("log_sectors_per_block                   [0x%x]\n",sb->log_sectors_per_block);
    printf("log_blocksize                           [0x%x]\n",sb->log_blocksize);
    printf("log_blocks_per_seg                      [0x%x]\n",sb->log_blocks_per_seg);
    printf("segs_per_sec                            [0x%x]\n",sb->segs_per_sec);
    printf("secs_per_zone                           [0x%x]\n",sb->secs_per_zone);
    printf("checksum_offset                         [0x%x]\n",sb->checksum_offset);
    printf("block_count                             [0x%llx]\n",sb->block_count);
    printf("section_count                           [0x%x]\n",sb->section_count);
    printf("segment_count                           [0x%x]\n",sb->segment_count);
    printf("segment_count_ckpt                      [0x%x]\n",sb->segment_count_ckpt);
    printf("segment_count_sit                       [0x%x]\n",sb->segment_count_sit);
    printf("segment_count_nat                       [0x%x]\n",sb->segment_count_nat);
    printf("segment_count_ssa                       [0x%x]\n",sb->segment_count_ssa);
    printf("segment_count_main                      [0x%x]\n",sb->segment_count_main);
    printf("segment0_blkaddr                        [0x%x]\n",sb->segment0_blkaddr);
    printf("cp_blkaddr                              [0x%x]\n",sb->cp_blkaddr);
    printf("sit_blkaddr                             [0x%x]\n",sb->sit_blkaddr);
    printf("nat_blkaddr                             [0x%x]\n",sb->nat_blkaddr);
    printf("ssa_blkaddr                             [0x%x]\n",sb->ssa_blkaddr);
    printf("main_blkaddr                            [0x%x]\n",sb->main_blkaddr);
    printf("root_ino                                [0x%x]\n",sb->root_ino);
    printf("node_ino                                [0x%x]\n",sb->node_ino);
    printf("meta_into                               [0x%x]\n",sb->meta_ino);
    printf("cp_payload                              [0x%x]\n",sb->cp_payload);
}


int get_super_block(struct f2fs_super_block* sb ,size_t partition_no)
{
    if(partition_no > 3) 
    {
        fprintf(stderr,"Error: invalid partition number");
        return 1;
    }
    
    struct mbr_t mbr;
    read_blocks(0,(block512*)&mbr,1);
    read_blocks(mbr.partition_entry[partition_no].first_sector_LBA * SECTOR_SIZE+ 2 * SECTOR_SIZE, (block512*)sb, 6);

    return 0;
}





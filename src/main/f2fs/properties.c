#include "f2fs.h"
#include "sd.h" 
#include <stdio.h>

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


int get_super_block(struct f2fs_super_block* sp ,uint32_t index, uint_8 blocks_no)
{
    read_blocks(index,(block512*)sp, blocks_no);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////


void checkpoint_display(struct f2fs_checkpoint* cp)
{
    printf("+----------------------------------------------+\n");
    printf("| Checkpoint                                   |\n");
    printf("+----------------------------------------------+\n");
    printf("checkpoint_ver                          [0x%x]\n",cp->checkpoint_ver);
    printf("user_block_count                        [0x%x]\n",cp->user_block_count);
    printf("valid_block_count                       [0x%x]\n",cp->valid_block_count);
    printf("rsvd_segment_count                      [0x%x]\n",cp->rsvd_segment_count);
    printf("overprov_segment_count                  [0x%x]\n",cp->overprov_segment_count);
    printf("free_segment_count                      [0x%x]\n",cp->free_segment_count);

    printf("alloc_type[CURSEG_HOT_NODE]             [0x%x]\n",cp->alloc_type[0]);
    printf("alloc_type[CURSEG_WARM_NODE]            [0x%x]\n",cp->alloc_type[1]);
    printf("alloc_type[CURSEG_COLD_NODE]            [0x%x]\n",cp->alloc_type[2]);

    printf("cur_node_segno[0]                       [0x%x]\n",cp->cur_node_segno[0]);
    printf("cur_node_segno[1]                       [0x%x]\n",cp->cur_node_segno[1]);
    printf("cur_node_segno[2]                       [0x%x]\n",cp->cur_node_segno[2]);

    printf("cur_node_blkoff[0]                      [0x%x]\n",cp->cur_node_blkoff[0]);
    printf("cur_node_blkoff[1]                      [0x%x]\n",cp->cur_node_blkoff[1]);
    printf("cur_node_blkoff[2]                      [0x%x]\n",cp->cur_node_blkoff[2]);

    printf("alloc_type[CURSED_HOT_DATA]             [0x%x]\n",cp->alloc_type[3]);
    printf("alloc_type[CURSED_WARM_DATA]            [0x%x]\n",cp->alloc_type[4]);
    printf("alloc_type[CURSED_COLD_DATA]            [0x%x]\n",cp->alloc_type[5]);

    printf("cur_data_segno[0]                       [0x%x]\n",cp->cur_data_segno[0]);
    printf("cur_data_segno[1]                       [0x%x]\n",cp->cur_data_segno[1]);
    printf("cur_data_segno[2]                       [0x%x]\n",cp->cur_data_segno[2]);

    printf("cur_data_blkoff[0]                      [0x%x]\n",cp->cur_data_blkoff[0]);
    printf("cur_data_blkoff[1]                      [0x%x]\n",cp->cur_data_blkoff[1]);
    printf("cur_data_blkoff[2]                      [0x%x]\n",cp->cur_data_blkoff[2]);

    printf("ckpt_flags                              [0x%x]\n",cp->ckpt_flags);
    printf("cp_pack_total_block_count               [0x%x]\n",cp->cp_pack_total_block_count);
    printf("cp_pack_start_sum                       [0x%x]\n",cp->cp_pack_start_sum);
    printf("valid_node_count                        [0x%x]\n",cp->valid_node_count);
    printf("valid_inode_count                       [0x%x]\n",cp->valid_inode_count);
    printf("next_free_nid                           [0x%x]\n",cp->next_free_nid);
    printf("sit_ver_bitmap_bytesize                 [0x%x]\n",cp->sit_ver_bitmap_bytesize);
    printf("nat_ver_bitmap_bytesize                 [0x%x]\n",cp->nat_ver_bitmap_bytesize);
    printf("checksum_offset                         [0x%x]\n",cp->checksum_offset);
    printf("elapsed_time                            [0x%x]\n",cp->elapsed_time);
    printf("sit_nat_version_bitmap[0]               [0x%x]\n",cp->sit_nat_version_bitmap[0]);
}

int get_checkpoint(struct f2fs_checkpoint* cp ,uint32_t index, uint_8 blocks_no)
{
    read_blocks(index,(block512*)cp, blocks_no);
    return 0;
}
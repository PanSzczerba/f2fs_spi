#include "f2fs.h"
#include "sd.h"
#include <stdio.h>

void display_checkpoint(struct f2fs_checkpoint* cp)
{
    printf("+----------------------------------------------+\n");
    printf("| Checkpoint                                   |\n");
    printf("+----------------------------------------------+\n");
    printf("checkpoint_ver                          [0x%llx]\n",cp->checkpoint_ver);
    printf("user_block_count                        [0x%llx]\n",cp->user_block_count);
    printf("valid_block_count                       [0x%llx]\n",cp->valid_block_count);
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
    printf("elapsed_time                            [0x%llx]\n",cp->elapsed_time);
    printf("sit_nat_version_bitmap[0]               [0x%x]\n",cp->sit_nat_version_bitmap[0]);
}

int get_checkpoint(struct f2fs_checkpoint* cp, uint32_t address)
{
    block512_t blk;
    read_blocks(address, &blk, 1);

    void* vp = (void*)&blk;
    *cp = *((struct f2fs_checkpoint*)vp);
    return 0;
}
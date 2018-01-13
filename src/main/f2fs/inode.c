#include "f2fs.h"
#include "sd.h"
#include <stdio.h>
#include <string.h>


void display_inode(struct f2fs_inode* in) {
    printf("+----------------------------------------------+\n");
    printf("| INode                                        |\n");
    printf("+----------------------------------------------+\n");
    printf("i_mode                                    [0%o]\n", in->i_mode);
    printf("i_advise                                  [0x%x]\n", in->_advise);
    printf("i_inline                                  [0x%x]\n", in->i_inline);
    printf("i_uid                                     [0x%x]\n", in->i_uid);
    printf("i_gid                                     [0x%x]\n", in->i_gid);
    printf("i_links                                   [0x%x]\n", in->i_links);
    printf("i_size                                  [0x%llx]\n", in->i_size);
    printf("i_blocks                                [0x%llx]\n", in->i_blocks);
    printf("i_atime                                 [0x%llx]\n", in->i_atime);
    printf("i_ctime                                 [0x%llx]\n", in->i_ctime);
    printf("i_mtime                                 [0x%llx]\n", in->i_mtime);
    printf("i_atime_nsec                              [0x%x]\n", in->i_atime_nsec);
    printf("i_ctime_nsec                              [0x%x]\n", in->i_ctime_nsec);
    printf("i_mtime_nsec                              [0x%x]\n", in->i_mtime_nsec);
    printf("i_generation                              [0x%x]\n", in->i_generation);
    printf("i_current_depth                           [0x%x]\n", in->i_current_depth);
    printf("i_xattr_nid                               [0x%x]\n", in->i_xattr_nid);
    printf("i_flags                                   [0x%x]\n", in->i_flags);
    printf("i_pino                                    [0x%x]\n", in->i_pino);
    printf("i_namelen                                 [0x%x]\n", in->i_namelen);
    printf("name                                        [%s]\n", (char *) in->i_name);
    printf("i_dir_level                               [0x%x]\n", in->i_dir_level);
    printf("i_ext:\n");
    printf("fofs                                    [0x%x]\n", in->i_ext.fofs);
    printf("blk                                     [0x%x]\n", in->i_ext.blk);
    printf("len                                     [0x%x]\n", in->i_ext.len);
}

#define BLOCKS_PER_INODE ((sizeof(struct f2fs_inode) + BLOCK_SIZE - 1)/BLOCK_SIZE)
#define INODE_NAT_BLOCK(inode_no) ((inode_no)/NAT_ENTRY_PER_BLOCK)

int get_inode(struct f2fs_meta_data* md, struct f2fs_inode* inode, size_t inode_no)
{
    block512_t inblock[BLOCKS_PER_INODE]; //inode fits on 8 blocks

    get_nat_block(md, INODE_NAT_BLOCK(inode_no));
    

    read_blocks(md->partition_block_address*BLOCK_SIZE + md->nat_blk.entries[inode_no - INODE_NAT_BLOCK(inode_no) * NAT_ENTRY_PER_BLOCK].block_addr * F2FS_BLOCK_SIZE, inblock, BLOCKS_PER_INODE);

    void* vp = (void*)inblock;
    *inode = *((struct f2fs_inode*)vp);
    
    return 0;
}

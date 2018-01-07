#include "f2fs.h"
#include "sd.h"
#include <stdio.h>


void display_checkpoint(struct f2fs_inode* in) {
    printf("+----------------------------------------------+\n");
    printf("| INode                                        |\n");
    printf("+----------------------------------------------+\n");
    printf("i_mode                                    [0x%x]\n", in->i_mode);
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
    printf("i_xattr_nid                               [0x%x]\n", in->i_xattr_nide);
    printf("i_flags                                   [0x%x]\n", in->i_flags);
    printf("i_pino                                    [0x%x]\n", in->i_pino);
    printf("i_namelen                                 [0x%x]\n", in->i_namelen);
    printf("name                                        [%s]\n", (char *) in->i_name);
    printf("i_dir_level                               [0x%x]\n", in->i_dir_level);
    printf("i_ext:\n");
    printf("fofs                                    [0x%llx]\n", in->i_ext.fofs);
    printf("blk                                     [0x%llx]\n", in->i_ext.blk);
    printf("len                                     [0x%llx]\n", in->i_ext.len);
}

int get_inode(struct f2fs_inode* in, uint32_t address)
{
    block512_t blk;
    read_blocks(address, &blk, 1);

    void* vp = (void*)&blk;
    *in = *((struct f2fs_inode*)vp);
    return 0;
}
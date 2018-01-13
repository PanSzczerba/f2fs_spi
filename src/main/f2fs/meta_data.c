#include "f2fs.h"

int init_meta_data(struct f2fs_meta_data* md)
{
    get_super_block(md);
    get_checkpoint(md);
    md->nat_block_no = -1;
    get_nat_block(md, 0);
    return 0;
}

#include "spi.h"
#include "sd.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int read_mbr(struct mbr_t* mbr)
{
    read_blocks(0, (block512_t *) mbr, 1);
    return 0;
}

void display_mbr(struct mbr_t* mbr)
{
    printf("+----------------------------------------------+\n");
    printf("| Master Boot Record                           |\n");
    printf("+----------------------------------------------+\n");
    printf("----------------------Partition 1---------------\n");
    printf("status                                    [0x%x]\n",mbr->partition_entry[0].status);
    
    printf("first_sector_CHS[0]                       [0x%x]\n",mbr->partition_entry[0].first_sector_CHS[0]);
    printf("first_sector_CHS[1]                       [0x%x]\n",mbr->partition_entry[0].first_sector_CHS[1]);
    printf("first_sector_CHS[2]                       [0x%x]\n",mbr->partition_entry[0].first_sector_CHS[2]);
    
    printf("partition_type                            [0x%x]\n",mbr->partition_entry[0].partition_type);
    
    printf("last_sector_CHS[0]                        [0x%x]\n",mbr->partition_entry[0].last_sector_CHS[0]);
    printf("last_sector_CHS[1]                        [0x%x]\n",mbr->partition_entry[0].last_sector_CHS[1]);
    printf("last_sector_CHS[2]                        [0x%x]\n",mbr->partition_entry[0].last_sector_CHS[2]);

    printf("first_sector_LBA                          [0x%x]\n",mbr->partition_entry[0].first_sector_LBA);
    printf("sector_no                                 [0x%x]\n",mbr->partition_entry[0].sector_no);

    printf("----------------------Partition 2---------------\n");
    printf("status                                    [0x%x]\n",mbr->partition_entry[1].status);

    printf("first_sector_CHS[0]                       [0x%x]\n",mbr->partition_entry[1].first_sector_CHS[0]);
    printf("first_sector_CHS[1]                       [0x%x]\n",mbr->partition_entry[1].first_sector_CHS[1]);
    printf("first_sector_CHS[2]                       [0x%x]\n",mbr->partition_entry[1].first_sector_CHS[2]);

    printf("partition_type                            [0x%x]\n",mbr->partition_entry[1].partition_type);

    printf("last_sector_CHS[0]                        [0x%x]\n",mbr->partition_entry[1].last_sector_CHS[0]);
    printf("last_sector_CHS[1]                        [0x%x]\n",mbr->partition_entry[1].last_sector_CHS[1]);
    printf("last_sector_CHS[2]                        [0x%x]\n",mbr->partition_entry[1].last_sector_CHS[2]);

    printf("first_sector_LBA                          [0x%x]\n",mbr->partition_entry[1].first_sector_LBA);
    printf("sector_no                                 [0x%x]\n",mbr->partition_entry[1].sector_no);

    printf("----------------------Partition 3---------------\n");
    printf("status                                    [0x%x]\n",mbr->partition_entry[2].status);

    printf("first_sector_CHS[0]                       [0x%x]\n",mbr->partition_entry[2].first_sector_CHS[0]);
    printf("first_sector_CHS[1]                       [0x%x]\n",mbr->partition_entry[2].first_sector_CHS[1]);
    printf("first_sector_CHS[2]                       [0x%x]\n",mbr->partition_entry[2].first_sector_CHS[2]);

    printf("partition_type                            [0x%x]\n",mbr->partition_entry[2].partition_type);

    printf("last_sector_CHS[0]                        [0x%x]\n",mbr->partition_entry[2].last_sector_CHS[0]);
    printf("last_sector_CHS[1]                        [0x%x]\n",mbr->partition_entry[2].last_sector_CHS[1]);
    printf("last_sector_CHS[2]                        [0x%x]\n",mbr->partition_entry[2].last_sector_CHS[2]);

    printf("first_sector_LBA                          [0x%x]\n",mbr->partition_entry[2].first_sector_LBA);
    printf("sector_no                                 [0x%x]\n",mbr->partition_entry[2].sector_no);

    printf("----------------------Partition 4---------------\n");
    printf("status                                    [0x%x]\n",mbr->partition_entry[3].status);

    printf("first_sector_CHS[0]                       [0x%x]\n",mbr->partition_entry[3].first_sector_CHS[0]);
    printf("first_sector_CHS[1]                       [0x%x]\n",mbr->partition_entry[3].first_sector_CHS[1]);
    printf("first_sector_CHS[2]                       [0x%x]\n",mbr->partition_entry[3].first_sector_CHS[2]);

    printf("partition_type                            [0x%x]\n",mbr->partition_entry[3].partition_type);

    printf("last_sector_CHS[0]                        [0x%x]\n",mbr->partition_entry[3].last_sector_CHS[0]);
    printf("last_sector_CHS[1]                        [0x%x]\n",mbr->partition_entry[3].last_sector_CHS[1]);
    printf("last_sector_CHS[2]                        [0x%x]\n",mbr->partition_entry[3].last_sector_CHS[2]);

    printf("first_sector_LBA                          [0x%x]\n",mbr->partition_entry[3].first_sector_LBA);
    printf("sector_no                                 [0x%x]\n",mbr->partition_entry[3].sector_no);
    
    printf("boot_signature                            [0x%x]\n",mbr->boot_signature);
}

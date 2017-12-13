#ifndef F2FS_H
#define F2FS_H

#include <stdint.h>
#include <stdlib.h>

#define MAX_ACTIVE_LOGS	16
#define MAX_ACTIVE_NODE_LOGS 8
#define MAX_ACTIVE_DATA_LOGS 8

#define VERSION_LEN	256
#define MAX_VOLUME_NAME 512
#define MAX_PATH_LEN 64
#define MAX_DEVICES	8
#define F2FS_MAX_EXTENSION 64

#define __packed __attribute__((packed))

/*
 * For superblock
 */
struct f2fs_device {
	uint8_t path[MAX_PATH_LEN];
	uint32_t total_segments;
} __packed;

struct f2fs_super_block {
	uint32_t magic;			/* Magic Number */
	uint16_t major_ver;		/* Major Version */
	uint16_t minor_ver;		/* Minor Version */
	uint32_t log_sectorsize;		/* log2 sector size in bytes */
	uint32_t log_sectors_per_block;	/* log2 # of sectors per block */
	uint32_t log_blocksize;		/* log2 block size in bytes */
	uint32_t log_blocks_per_seg;	/* log2 # of blocks per segment */
	uint32_t segs_per_sec;		/* # of segments per section */
	uint32_t secs_per_zone;		/* # of sections per zone */
	uint32_t checksum_offset;		/* checksum offset inside super block */
	uint64_t block_count;		/* total # of user blocks */
	uint32_t section_count;		/* total # of sections */
	uint32_t segment_count;		/* total # of segments */
	uint32_t segment_count_ckpt;	/* # of segments for checkpoint */
	uint32_t segment_count_sit;	/* # of segments for SIT */
	uint32_t segment_count_nat;	/* # of segments for NAT */
	uint32_t segment_count_ssa;	/* # of segments for SSA */
	uint32_t segment_count_main;	/* # of segments for main area */
	uint32_t segment0_blkaddr;	/* start block address of segment 0 */
	uint32_t cp_blkaddr;		/* start block address of checkpoint */
	uint32_t sit_blkaddr;		/* start block address of SIT */
	uint32_t nat_blkaddr;		/* start block address of NAT */
	uint32_t ssa_blkaddr;		/* start block address of SSA */
	uint32_t main_blkaddr;		/* start block address of main area */
	uint32_t root_ino;		/* root inode number */
	uint32_t node_ino;		/* node inode number */
	uint32_t meta_ino;		/* meta inode number */
	uint8_t uuid[16];			/* 128-bit uuid for volume */
	uint16_t volume_name[MAX_VOLUME_NAME];	/* volume name */
	uint32_t extension_count;		/* # of extensions below */
	uint8_t extension_list[F2FS_MAX_EXTENSION][8];	/* extension array */
	uint32_t cp_payload;
	uint8_t version[VERSION_LEN];	/* the kernel version */
	uint8_t init_version[VERSION_LEN];	/* the initial kernel version */
	uint32_t feature;			/* defined features */
	uint8_t encryption_level;		/* versioning level for encryption */
	uint8_t encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
	struct f2fs_device devs[MAX_DEVICES];	/* device list */
	uint8_t reserved[327];		/* valid reserved region */
} __packed;

int get_super_block(struct f2fs_super_block*, size_t partition_no);
void super_block_display(struct f2fs_super_block*);


#endif 

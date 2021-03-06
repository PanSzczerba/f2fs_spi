#ifndef F2FS_H
#define F2FS_H

#include <stdint.h>
#include <stdlib.h>

#define F2FS_SEGMENT_SIZE 0x200000
#define F2FS_BLOCK_SIZE 0x1000

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

int get_super_block(struct f2fs_super_block* sp, uint32_t address);
void super_block_display(struct f2fs_super_block*);

/*
 * For checkpoint
 */
#define CP_TRIMMED_FLAG		0x00000100
#define CP_NAT_BITS_FLAG	0x00000080
#define CP_CRC_RECOVERY_FLAG	0x00000040
#define CP_FASTBOOT_FLAG	0x00000020
#define CP_FSCK_FLAG		0x00000010
#define CP_ERROR_FLAG		0x00000008
#define CP_COMPACT_SUM_FLAG	0x00000004
#define CP_ORPHAN_PRESENT_FLAG	0x00000002
#define CP_UMOUNT_FLAG		0x00000001

#define F2FS_CP_PACKS		2	/* # of checkpoint packs */

struct f2fs_checkpoint {
	uint64_t checkpoint_ver;		/* checkpoint block version number */
	uint64_t user_block_count;	/* # of user blocks */
	uint64_t valid_block_count;	/* # of valid blocks in main area */
	uint32_t rsvd_segment_count;	/* # of reserved segments for gc */
	uint32_t overprov_segment_count;	/* # of overprovision segments */
	uint32_t free_segment_count;	/* # of free segments in main area */

	/* information of current node segments */
	uint32_t cur_node_segno[MAX_ACTIVE_NODE_LOGS];
	uint16_t cur_node_blkoff[MAX_ACTIVE_NODE_LOGS];

	/* information of current data segments */
	uint32_t cur_data_segno[MAX_ACTIVE_DATA_LOGS];
	uint16_t cur_data_blkoff[MAX_ACTIVE_DATA_LOGS];
	uint32_t ckpt_flags;		/* Flags : umount and journal_present */
	uint32_t cp_pack_total_block_count;	/* total # of one cp pack */
	uint32_t cp_pack_start_sum;	/* start block number of data summary */
	uint32_t valid_node_count;	/* Total number of valid nodes */
	uint32_t valid_inode_count;	/* Total number of valid inodes */
	uint32_t next_free_nid;		/* Next free node number */
	uint32_t sit_ver_bitmap_bytesize;	/* Default value 64 */
	uint32_t nat_ver_bitmap_bytesize; /* Default value 256 */
	uint32_t checksum_offset;		/* checksum offset inside cp block */
	uint64_t elapsed_time;		/* mounted time */

	/* allocation type of current segment */
	unsigned char alloc_type[MAX_ACTIVE_LOGS];

	/* SIT and NAT version bitmap */
	unsigned char sit_nat_version_bitmap[1];
} __packed;

#define F2FS_NAME_LEN       255
#define DEF_ADDRS_PER_INODE 923 /* Address Pointers in an Inode */
#define DEF_NIDS_PER_INODE  5   /* Node IDs in an Inode */


struct f2fs_inode {
	uint16_t i_mode;			/* file mode */
	uint8_t i_advise;			/* file hints */
	uint8_t i_inline;			/* file inline flags */
	uint32_t i_uid;			/* user ID */
	uint32_t i_gid;			/* group ID */
	uint32_t i_links;			/* links count */
	uint64_t i_size;			/* file size in bytes */
	uint64_t i_blocks;		/* file size in blocks */
	uint64_t i_atime;			/* access time */
	uint64_t i_ctime;			/* change time */
	uint64_t i_mtime;			/* modification time */
	uint32_t i_atime_nsec;		/* access time in nano scale */
	uint32_t i_ctime_nsec;		/* change time in nano scale */
	uint32_t i_mtime_nsec;		/* modification time in nano scale */
	uint32_t i_generation;		/* file version (for NFS) */
	uint32_t i_current_depth;		/* only for directory depth */
	uint32_t i_xattr_nid;		/* nid to save xattr */
	uint32_t i_flags;			/* file attributes */
	uint32_t i_pino;			/* parent inode number */
	uint32_t i_namelen;		/* file name length */
	uint8_t i_name[F2FS_NAME_LEN];	/* file name for SPOR */
	uint8_t i_dir_level;		/* dentry_level for large dir */

	struct f2fs_extent i_ext;	/* caching a largest extent */

	union {
		struct {
			uint16_t i_extra_isize;	/* extra inode attribute size */
			uint16_t i_padding;	/* padding */
			uint32_t i_projid;	/* project id */
			uint32_t i_inode_checksum;/* inode meta checksum */
			uint32_t i_extra_end[0];	/* for attribute size calculation */
		};
		uint32_t i_addr[DEF_ADDRS_PER_INODE];	/* Pointers to data blocks */
	};
	uint32_t i_nid[DEF_NIDS_PER_INODE];	/* direct(2), indirect(2),
						double_indirect(1) node id */
} __packed;

struct f2fs_nat_entry {
	uint8_t version;		/* latest version of cached nat entry */
	uint32_t ino;		/* inode number */
	uint32_t block_addr;	/* block address */
} __packed;


int get_checkpoint(struct f2fs_checkpoint* cp, uint32_t address);
void checkpoint_display(struct f2fs_checkpoint*);

#endif 

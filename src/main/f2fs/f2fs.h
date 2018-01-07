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

////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////
/*
 * For NODE structure
 */
struct f2fs_extent {
    uint32_t fofs;		/* start file offset of the extent */
    uint32_t blk;		/* start block address of the extent */
    uint32_t len;		/* length of the extent */
} __packed;

#define F2FS_NAME_LEN		255
#define F2FS_INLINE_XATTR_ADDRS	50	/* 200 bytes for inline xattrs */
#define DEF_ADDRS_PER_INODE	923	/* Address Pointers in an Inode */
#define CUR_ADDRS_PER_INODE(inode)	(DEF_ADDRS_PER_INODE - \
					get_extra_isize(inode))
#define DEF_NIDS_PER_INODE	5	/* Node IDs in an Inode */
#define ADDRS_PER_INODE(inode)	addrs_per_inode(inode)
#define ADDRS_PER_BLOCK		1018	/* Address Pointers in a Direct Block */
#define NIDS_PER_BLOCK		1018	/* Node IDs in an Indirect Block */

#define ADDRS_PER_PAGE(page, inode)	\
	(IS_INODE(page) ? ADDRS_PER_INODE(inode) : ADDRS_PER_BLOCK)

#define	NODE_DIR1_BLOCK		(DEF_ADDRS_PER_INODE + 1)
#define	NODE_DIR2_BLOCK		(DEF_ADDRS_PER_INODE + 2)
#define	NODE_IND1_BLOCK		(DEF_ADDRS_PER_INODE + 3)
#define	NODE_IND2_BLOCK		(DEF_ADDRS_PER_INODE + 4)
#define	NODE_DIND_BLOCK		(DEF_ADDRS_PER_INODE + 5)

#define F2FS_INLINE_XATTR	0x01	/* file inline xattr flag */
#define F2FS_INLINE_DATA	0x02	/* file inline data flag */
#define F2FS_INLINE_DENTRY	0x04	/* file inline dentry flag */
#define F2FS_DATA_EXIST		0x08	/* file inline data exist flag */
#define F2FS_INLINE_DOTS	0x10	/* file having implicit dot dentries */
#define F2FS_EXTRA_ATTR		0x20	/* file having extra attribute */

struct f2fs_inode {
    uint16_t i_mode;			/* file mode */
    uint8_t _advise;			/* file hints */
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

struct direct_node {
    uint32_t addr[ADDRS_PER_BLOCK];	/* array of data block address */
} __packed;

struct indirect_node {
    uint32_t nid[NIDS_PER_BLOCK];	/* array of data block address */
} __packed;

enum {
    COLD_BIT_SHIFT = 0,
    FSYNC_BIT_SHIFT,
    DENT_BIT_SHIFT,
    OFFSET_BIT_SHIFT
};

#define OFFSET_BIT_MASK		(0x07)	/* (0x01 << OFFSET_BIT_SHIFT) - 1 */

struct node_footer {
    uint32_t nid;		/* node id */
    uint32_t ino;		/* inode nunmber */
    uint32_t flag;		/* include cold/fsync/dentry marks and offset */
    uint64_t cp_ver;		/* checkpoint version */
    uint32_t next_blkaddr;	/* next node page block address */
} __packed;

struct f2fs_node {
    /* can be one of three types: inode, direct, and indirect types */
    union {
        struct f2fs_inode i;
        struct direct_node dn;
        struct indirect_node in;
    };
    struct node_footer footer;
} __packed;

int get_inode(struct f2fs_inode* in, uint32_t address);
void dislpay_inode(struct f2fs_inode*);


////////////////////////////////////////////////////////////////////

/*
 * For NAT entries
 */
#define NAT_ENTRY_PER_BLOCK (PAGE_SIZE / sizeof(struct f2fs_nat_entry))
#define NAT_ENTRY_BITMAP_SIZE	((NAT_ENTRY_PER_BLOCK + 7) / 8)

struct f2fs_nat_entry {
    uint8_t version;		/* latest version of cached nat entry */
    unt32_t ino;		/* inode number */
    uint32_t block_addr;	/* block address */
} __packed;

struct f2fs_nat_block {
    struct f2fs_nat_entry entries[NAT_ENTRY_PER_BLOCK];
} __packed;

struct f2fs_meta_data
{
    uint32_t partition_block_address;
    struct f2fs_super_block sb;
    struct f2fs_checkpoint chkp;
} __packed;

int get_super_block(struct f2fs_meta_data* md);
void super_block_display(struct f2fs_super_block*);
int get_checkpoint(struct f2fs_meta_data* md);
void checkpoint_display(struct f2fs_checkpoint*);
int get_nat_entries(struct f2fs_meta_data* md, struct f2fs_nat_entry* nat_entries, size_t first_nat_entry_no, size_t nat_entry_count);
int get_nat_entry(struct f2fs_nat_entry* ne, uint32_t address);
void display_nat_entry(struct f2fs_nat_entry*);

#endif 

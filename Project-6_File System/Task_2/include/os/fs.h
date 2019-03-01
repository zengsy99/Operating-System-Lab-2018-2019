#ifndef INCLUDE_FS_H_
#define INCLUDE_FS_H_

#include "type.h"

// ---------------------------------------------------------------------
// | Superblock  | Block Bitmap  | Inode Bitmap |   Inode   |   Data   |
// | 1 Block 4KB | 8 Block 32KB  | 1 Block 4KB  | 246 Block | 1023*256 |
// ---------------------------------------------------------------------

#define FS_START_ADDR     0x20000000  // 512MB
#define FS_SIZE           0x40000000  // 1GB
#define MAGIC             0x66666666
#define BLOCK_SIZE        0x1000      // 4KB
#define BLOCK_MAP_NUM     8
#define INODE_MAP_NUM     1
#define INODE_BLOCK_NUM   246
#define DIRECT_BLOCK_NUM  2
#define MAX_FILE_NAME_LEN 28
#define MAX_DEPTH         16
#define MAX_FILE_NUM      32
#define MAX_DEN_SIZE      1024
#define INODE_SIZE        512
#define FD_NUM            8

typedef enum {
    IS_FILE,
    IS_DIRECTORY
}type;

typedef enum {
    O_RDONLY,
    O_WRONLY,
    O_RDWR
} access_t;

typedef struct dentry_t 
{
    char file_name[MAX_FILE_NAME_LEN];
    int  ino;
} dentry_t;      // 32B

typedef struct file_descriptor
{
    uint32_t ino;
    access_t access; // R W RW
    uint32_t w_pos;
    uint32_t r_pos;
    uint32_t size;
    uint32_t used;
} fd_t;

typedef struct superblock_t
{
    uint32_t magic;
    uint32_t fs_size;
    uint32_t used_block_num;
    uint32_t fs_start_addr;
    uint32_t fs_end_addr;

    uint32_t block_map_num;
    uint32_t block_map_size;
    uint32_t block_map_start;

    uint32_t inode_map_num;
    uint32_t inode_map_size;
    uint32_t inode_map_start;

    uint32_t inode_num;
    uint32_t inode_size;
    uint32_t inode_start;

    uint32_t data_start;
    uint32_t block_size;
    uint32_t dentry_size;
    
} superblock_t;

typedef struct inode_t{
    int  mode;                      
    int  file_size;
    int  file_indirect_blocks_num;

    type file_type;                
    int  dentry_num;              
    
    int  blocks_number;
    int  direct_block[DIRECT_BLOCK_NUM];
    int  indirect_block;
    int  links;

    uint64_t create_time;
    uint64_t last_access_time;
    uint64_t last_modified_time;
} inode_t;    // 64B

extern fd_t fdesc[FD_NUM];
extern superblock_t superblock;
extern char current_dir_name[MAX_DEPTH][32];
extern int  current_dir_depth;

void do_mkfs();
void do_statfs(int *);
void do_ls(char *, int *);
int  do_cd(char *);
int  do_mkdir(char *);
int  do_rmdir(char *);

int do_touch(char *);
int do_cat(char *, int *);
int do_open(char *, int);
int do_read(int, char *, int);
int do_write(int, char *, int);
void do_close(int);

#endif

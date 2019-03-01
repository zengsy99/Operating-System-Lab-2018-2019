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

typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef uint32_t *Elf32_Addr;   
typedef int      Elf32_Sword;
typedef uint32_t Elf32_Off;

#define EI_NIDENT (16)
typedef struct
{
  unsigned char e_ident[EI_NIDENT];   /* Magic number and other info */
  Elf32_Half    e_type;               /* Object file type */
  Elf32_Half    e_machine;            /* Architecture */
  Elf32_Word    e_version;            /* Object file version */
  Elf32_Addr    e_entry;              /* Entry point virtual address */
  Elf32_Off     e_phoff;              /* Program header table file offset */
  Elf32_Off     e_shoff;              /* Section header table file offset */
  Elf32_Word    e_flags;              /* Processor-specific flags */
  Elf32_Half    e_ehsize;             /* ELF header size in bytes */
  Elf32_Half    e_phentsize;          /* Program header table entry size */
  Elf32_Half    e_phnum;              /* Program header table entry count */
  Elf32_Half    e_shentsize;          /* Section header table entry size */
  Elf32_Half    e_shnum;              /* Section header table entry count */
  Elf32_Half    e_shstrndx;           /* Section header string table index */
} Elf32_Ehdr;

typedef struct { 
    Elf32_Word p_type; /* segment type */ 
    Elf32_Off  p_offset; /* segment offset */ 
    Elf32_Addr p_vaddr; /* virtual address of segment */ 
    Elf32_Addr p_paddr; /* physical address - ignored? */ 
    Elf32_Word p_filesz; /* number of bytes in file for seg. */ 
    Elf32_Word p_memsz; /* number of bytes in mem. for seg. */ 
    Elf32_Word p_flags; /* flags */ 
    Elf32_Word p_align; /* memory alignment */ 
} Elf32_Phdr;

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
void do_launch(char *);

#endif

#include "fs.h"
#include "time.h"

superblock_t superblock;

char block_bitmap_buffer[32*1024] = {0};   // 32KB
char inode_bitmap_buffer[ 4*1024] = {0};   //  4KB
char block_buf[BLOCK_SIZE] = {0};
int  ino;

inode_t inode_buffer;
inode_t root_inode;

dentry_t dentries_buffer[MAX_FILE_NUM];

char current_dir_name[MAX_DEPTH][32];
int  current_dir_depth = 0;

fd_t fdesc[FD_NUM];

int map_to_i(char * map)
{
    if( (*map & 0x01) == 0x0 ){
        * map = (*map) | 0x01;
        return 0;
    }
    else if((*map & 0x02) == 0x00){
        * map = (*map) | 0x02;
        return 1;
    }
    else if((*map & 0x04) == 0x00){
        * map = (*map) | 0x04;
        return 2;
    }
    else if((*map & 0x08) == 0x00){
        * map = (*map) | 0x08;
        return 3;
    }
    else if((*map & 0x10) == 0x00){
        * map = (*map) | 0x10;
        return 4;
    }
    else if((*map & 0x20) == 0x00){
        * map = (*map) | 0x20;
        return 5;
    }
    else if((*map & 0x40) == 0x00){
        * map = (*map) | 0x40;
        return 6;
    }
    else if((*map & 0x80) == 0x00){
        * map = (*map) | 0x80;
        return 7;
    }

    return -1;
}

void i_to_map(char * map, int res)
{
    if( res == 0 )
        * map = (*map) & 0xfe;
    else if( res == 1 )
        * map = (*map) & 0xfd;
    else if( res == 2 )
        * map = (*map) & 0xfb;
    else if( res == 3 )
        * map = (*map) & 0xf7;
    else if( res == 4 )
        * map = (*map) & 0xef;
    else if( res == 5 )
        * map = (*map) & 0xdf;
    else if( res == 6 )
        * map = (*map) & 0xbf;
    else if( res == 7 )
        * map = (*map) & 0x7f;
}

static void init_superblock()
{
    superblock.magic   = MAGIC;
    superblock.fs_size = FS_SIZE;
    superblock.used_block_num  = 257;
    superblock.fs_start_addr   = FS_START_ADDR;
    superblock.fs_end_addr     = FS_START_ADDR + FS_SIZE;
    
    superblock.block_map_num   = BLOCK_MAP_NUM;
    superblock.block_map_size  = BLOCK_MAP_NUM * BLOCK_SIZE;
    superblock.block_map_start = FS_START_ADDR + 1 * BLOCK_SIZE;

    superblock.inode_map_num   = INODE_MAP_NUM;
    superblock.inode_map_size  = BLOCK_SIZE;
    superblock.inode_map_start = FS_START_ADDR + 9 * BLOCK_SIZE;
    
    superblock.inode_num   = 1;
    superblock.inode_size  = sizeof(inode_t);
    superblock.inode_start = FS_START_ADDR + 10 * BLOCK_SIZE;

    superblock.data_start  = FS_START_ADDR + 256 * BLOCK_SIZE;
    superblock.block_size  = BLOCK_SIZE;
    superblock.dentry_size = sizeof(dentry_t);

    // write SDcard
    sdwrite(&superblock, FS_START_ADDR, sizeof(superblock_t));

    return;
}

static void init_inode_bitmap()
{
    int i;
    inode_bitmap_buffer[0] = 0x1;  // root inode
    
    for (i = 1; i < BLOCK_SIZE; i++)
        inode_bitmap_buffer[i] = 0x00;

    sdwrite(&inode_bitmap_buffer, superblock.inode_map_start, sizeof(inode_bitmap_buffer));
}

static void init_block_bitmap()
{
    int i;

    for(i = 0; i < 32; i++)
    {
        block_bitmap_buffer[i]= 0xff;
    }
    
    for(i = 32; i < 0x8000; i++)
    {
        block_bitmap_buffer[i]= 0x00;
    }

    block_bitmap_buffer[32]= 0x01;

    sdwrite(&block_bitmap_buffer, superblock.block_map_start, sizeof(block_bitmap_buffer));
    return;
}

static void init_root_inode()
{
    root_inode.mode                       = 4;
    root_inode.file_size                  = 2 * sizeof(dentry_t);
    root_inode.file_indirect_blocks_num   = 0;

    root_inode.file_type                  = IS_DIRECTORY;
    root_inode.dentry_num                 = 2;

    root_inode.blocks_number              = 1;
    root_inode.direct_block[0]            = 0;
    root_inode.links                      = 0;

    root_inode.last_access_time           =get_timer();      
    root_inode.last_modified_time         =get_timer();
    root_inode.create_time                =get_timer();

    sdwrite(&root_inode, superblock.inode_start, sizeof(inode_t));
    return;
}

static void init_root_dir()
{
    dentry_t init_root_dentries[2] = { 
                                       {"." , 0},
                                       {"..", 0}
                                     };

    sdwrite(init_root_dentries, superblock.data_start, sizeof(dentry_t) * 2);
}

void do_mkfs()
{
    // init superblock
    init_superblock();
	
    // init inode bitmap
    init_inode_bitmap();

    // init block bitmap
    init_block_bitmap();

    // init root inode
    init_root_inode();
    
    // init root inode
    init_root_dir();
    
    ino = 1;
    current_dir_depth = 0;
    current_dir_name[0][0] = '\0';

    return;
}

void do_statfs(int * location)
{
    int L;
    L = *location;

    vt100_move_cursor(1, L+1);
    printk("Magic: 0x%x\n", superblock.magic);

    vt100_move_cursor(1, L+2);
    printk("Block size: 0x%xB, inode size: 0x%xB, dentry size: 0x%xB\n", superblock.block_size, superblock.inode_size, superblock.dentry_size);

    vt100_move_cursor(1, L+3);
    printk("Used sector: 0x%x/0x40000, ", superblock.used_block_num);
    printk("Start Addr: 0x%x\n", superblock.fs_start_addr);

    vt100_move_cursor(1, L+4);
    printk("block map offset: 1, ");
    printk("block map size: 8, ");
    printk("block map start addr: 0x%x\n", superblock.block_map_start);

    vt100_move_cursor(1, L+5);
    printk("inode map offset: 9, ");
    printk("inode map size: 1, ");
    printk("inode map start addr: 0x%x\n", superblock.inode_map_start);

    vt100_move_cursor(1, L+6);
    printk("inode offset:    10, ");
    printk("inode size:   246, ");
    printk("inode start addr:     0x%x\n", superblock.inode_start);

    vt100_move_cursor(1, L+7);
    printk("data block offset: 256, ");
    printk("data block start addr: 0x%x\n", superblock.data_start);

    *location = L + 6;
    return;
}

int do_rmdir(char *file_name)
{
    int i, j;
    int current_ino = 0;
    int block_num;
    int father_block_num;
    int father_dir_num;

    for (i = 0; i < current_dir_depth; i++)
    {
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    	
    	// Assume directory only use 1 block
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }

        if(j == inode_buffer.dentry_num)
        {
            printk("Path Error in do_mkdir");
            return 1; // Path Error in the middle path
        }
    }
    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);
    father_block_num = inode_buffer.direct_block[0];
    father_dir_num = inode_buffer.dentry_num;

    // to find the dir
    for(j = 0; j < father_dir_num; j++)
        if (!strcmp(file_name, dentries_buffer[j].file_name))
            break;
    if(j == inode_buffer.dentry_num)
    {
        printk("[Error] The directory doesn't exist!");
        return 2;
    }

    // recovery inode and reset inode bitmap
    sdread(&inode_bitmap_buffer, superblock.inode_map_start, 4096);
    ino = dentries_buffer[j].ino;
    int locate;
    int res;
    res = ino % 8;
    locate = ino - res;
    i_to_map(&inode_bitmap_buffer[locate], res); // and reset inode_bitmap_buffer[i]
    sdwrite(&inode_bitmap_buffer, superblock.inode_map_start, 4096);

    // recovery block and reset block bitmap 
    sdread(&block_bitmap_buffer, superblock.block_map_start, 32768);
    sdread(&inode_buffer, superblock.inode_start + ino * INODE_SIZE, sizeof(inode_t));
    block_num = inode_buffer.direct_block[0];
    res = block_num % 8;
    locate = block_num - res;
    i_to_map(&block_bitmap_buffer[locate + 32], res); // and reset inode_bitmap_buffer[i]
    sdwrite(&block_bitmap_buffer, superblock.block_map_start, 32768);

    // updata the father directory and inode
    for(i = j; i < father_dir_num; i++)
        dentries_buffer[i] = dentries_buffer[i + 1];
    sdwrite(dentries_buffer, superblock.data_start + father_block_num * BLOCK_SIZE, MAX_DEN_SIZE);

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    inode_buffer.file_size -= 3 * sizeof(dentry_t);
    inode_buffer.dentry_num --;
    inode_buffer.links --;
    inode_buffer.last_modified_time         = get_timer();
    inode_buffer.create_time                = get_timer();
    sdwrite(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));

    return 1;
}


int do_mkdir(char *file_name)
{
    int i, j;
    int current_ino = 0;
    int block_num;
    int father_block_num;

    for (i = 0; i < current_dir_depth; i++)
    {
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    	
    	// Assume directory only use 1 block
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }

        if(j == inode_buffer.dentry_num)
        {
            printk("Path Error in do_mkdir");
            return 1; // Path Error in the middle path
        }
    }
    
    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);
    father_block_num = inode_buffer.direct_block[0];

    for(j = 0; j < inode_buffer.dentry_num; j++)
        if (!strcmp(file_name, dentries_buffer[j].file_name))
        {
            printk("The directory has already existed!");
            return 2; // The directory has already existed!
        }

    // allocate inode and reset inode bitmap
    sdread(&inode_bitmap_buffer, superblock.inode_map_start, 4096);
    for(i = 0; i < 4096; i ++)
    {
    	if(inode_bitmap_buffer[i] != 0xff)
    	    break;
    }
    ino = i * 8 + map_to_i(&inode_bitmap_buffer[i]); // and reset inode_bitmap_buffer[i]
    sdwrite(&inode_bitmap_buffer, superblock.inode_map_start, 4096);

    // allocate block and reset block bitmap 
    sdread(&block_bitmap_buffer, superblock.block_map_start, 32768);
    for(i = 32; i < 32768; i ++)
    {
    	if(block_bitmap_buffer[i] != 0xff)
    	    break;
    }
    block_num = (i-32) * 8 + map_to_i(&block_bitmap_buffer[i]); // and reset block_bitmap_buffer[i]
    sdwrite(&block_bitmap_buffer, superblock.block_map_start, 32768);

    // init the inode
    inode_buffer.mode                       = 0;
    inode_buffer.file_size                  = 2 * sizeof(dentry_t);
    inode_buffer.file_indirect_blocks_num   = 0;
    inode_buffer.file_type                  = IS_DIRECTORY;
    inode_buffer.dentry_num                 = 2;
    inode_buffer.blocks_number              = 1;
    inode_buffer.direct_block[0]            = block_num;
    inode_buffer.links                      = 0;
    inode_buffer.last_access_time           = get_timer();      
    inode_buffer.last_modified_time         = get_timer();
    inode_buffer.create_time                = get_timer();

    sdwrite(&inode_buffer, superblock.inode_start + ino * INODE_SIZE, sizeof(inode_t));

    // init the data block
    dentry_t dentries[2] = { 
                              {"." , ino},
                              {"..", current_ino}
                           };
    sdwrite(dentries, superblock.data_start + block_num * BLOCK_SIZE, 2 * sizeof(dentry_t));

    // updata the father directory
    memcpy(&(dentries_buffer[j].file_name), file_name, strlen(file_name));
    dentries_buffer[j].ino = ino;
    sdwrite(dentries_buffer, superblock.data_start + father_block_num * BLOCK_SIZE, MAX_DEN_SIZE);

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    inode_buffer.file_size += 3 * sizeof(dentry_t);
    inode_buffer.dentry_num ++;
    inode_buffer.links ++;
    inode_buffer.last_modified_time         = get_timer();
    inode_buffer.create_time                = get_timer();
    sdwrite(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));

    return 0;
}

int do_cd(char *file_name)
{
    int i, j;
    int current_ino = 0;
    int file_depth_num = 0;
    int cnt = 0;
    char input_file_name[5][32];

    for(i = 0, cnt = 0; i < 5 ; i ++, cnt ++)
    {
        for(j = 0; file_name[cnt] != '/' && file_name[cnt] != '\0'; j++, cnt ++)
            input_file_name[i][j] = file_name[cnt];

        input_file_name[i][j] = '\0';

        if(file_name[cnt] == '\0')
            break;
    }

    if( !strcmp(file_name, ".") )
        file_depth_num = 0;
    else if( !strcmp(file_name, "..") )
        file_depth_num = -1;
    else
        file_depth_num = i+1;

    for (i = 0; i < current_dir_depth; i++)
    {
        // Assume directory only use 1 block
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
        {
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }
        }

        if(j == inode_buffer.dentry_num)
        {
            printk("[Error] Path Error in do_cd!");
            return 1; // Path Error in the middle path
        }
    }

    // to find the behind directory
    i = 0;
    for (i = 0; i < file_depth_num - 1; i++)
    {
        // Assume directory only use 1 block
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
        {
            if (!strcmp(input_file_name[i], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }
        }

        if(j == inode_buffer.dentry_num)
        {
            printk("[Error] Path Error in do_cd!");
            return 1; // Path Error in the middle path
        }
    }

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

    for(j = 0; j < inode_buffer.dentry_num; j++)
        if (!strcmp(input_file_name[i], dentries_buffer[j].file_name))
        {
            int k;
            for(k = 0; k < file_depth_num; k++)
                memcpy(current_dir_name[current_dir_depth + k + 1], input_file_name[k], strlen(input_file_name[k]));

            current_dir_depth = current_dir_depth + file_depth_num;
            if(current_dir_depth < 0)
                current_dir_depth = 0;

            return 1; // match and return
        }
    
    printk("[Error] Not Find the directory! ");
    return -1;
}

void do_ls(char *file_name, int * location)
{
    int i, j;
    int current_ino = 0;
    int file_depth_num = 0;
    int cnt = 0;
    char input_file_name[5][32];

    for(i = 0, cnt = 0; i < 5 ; i ++, cnt ++)
    {
        for(j = 0; file_name[cnt] != '/' && file_name[cnt] != '\0'; j++, cnt ++)
            input_file_name[i][j] = file_name[cnt];

        input_file_name[i][j] = '\0';

        if(file_name[cnt] == '\0')
            break;
    }
    if( !strcmp(file_name, ".") || !strcmp(file_name, "\0") )
        file_depth_num = 0;
    else if( !strcmp(file_name, "..") )
        file_depth_num = -1;
    else
        file_depth_num = i+1;


    for (i = 0; i < current_dir_depth; i++)
    {
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE,   sizeof(inode_t));
    	
    	// Assume directory only use 1 block
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }

        if(j == inode_buffer.dentry_num)
            printk("[Error] Path Error in do_ls_1!");
    }

    // to find the behind directory
    i = 0;
    for (i = 0; i < file_depth_num; i++)
    {
        // Assume directory only use 1 block
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
        {
            if (!strcmp(input_file_name[i], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }
        }

        if(j == inode_buffer.dentry_num)
        {
            printk("[Error] Path Error in do_ls_2!");
            return; // Path Error in the middle path
        }
    }

    // Debug !!!
    vt100_move_cursor(1, 12);
    printk("                                                                       ");
    vt100_move_cursor(1, 12);
    printk("[ls] depth = %d,    root",current_dir_depth);
    int m;
    for(m = 1; m <= current_dir_depth; m ++)
        printk("/%s", current_dir_name[m]);
    // Debug !!!

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

    vt100_move_cursor(1, *location + 1);
    for(j = 0; j < inode_buffer.dentry_num; j++)
      printk("%s  ", dentries_buffer[j].file_name);

    return;
}

int do_touch(char *file_name)
{
    int i, j;
    int current_ino = 0;
    int block_num;
    int father_block_num;

    for (i = 0; i < current_dir_depth; i++)
    {
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    	
    	// Assume directory only use 1 block
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }

        if(j == inode_buffer.dentry_num)
        {
            printk("Path Error in do_mkdir");
            return 1; // Path Error in the middle path
        }
    }

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);
    father_block_num = inode_buffer.direct_block[0];

    for(j = 0; j < inode_buffer.dentry_num; j++)
        if (!strcmp(file_name, dentries_buffer[j].file_name))
        {
            printk("The directory has already existed!");
            return 2; // The directory has already existed!
        }

    // allocate inode and reset inode bitmap
    sdread(&inode_bitmap_buffer, superblock.inode_map_start, 4096);
    for(i = 0; i < 4096; i ++)
    {
    	if(inode_bitmap_buffer[i] != 0xff)
    	    break;
    }
    ino = i * 8 + map_to_i(&inode_bitmap_buffer[i]); // and reset inode_bitmap_buffer[i]
    sdwrite(&inode_bitmap_buffer, superblock.inode_map_start, 4096);

    // allocate block and reset block bitmap 
    sdread(&block_bitmap_buffer, superblock.block_map_start, 32768);
    for(i = 32; i < 32768; i ++)
    {
    	if(block_bitmap_buffer[i] != 0xff)
    	    break;
    }
    block_num = (i-32) * 8 + map_to_i(&block_bitmap_buffer[i]); // and reset block_bitmap_buffer[i]
    sdwrite(&block_bitmap_buffer, superblock.block_map_start, 32768);

    // init the inode
    inode_buffer.mode                       = 0;
    inode_buffer.file_size                  = 0;
    inode_buffer.file_indirect_blocks_num   = 0;
    inode_buffer.file_type                  = IS_FILE;
    inode_buffer.dentry_num                 = 2;
    inode_buffer.blocks_number              = 1;
    inode_buffer.direct_block[0]            = block_num;
    inode_buffer.links                      = 0;
    inode_buffer.last_access_time           = get_timer();      
    inode_buffer.last_modified_time         = get_timer();
    inode_buffer.create_time                = get_timer();

    sdwrite(&inode_buffer, superblock.inode_start + ino * INODE_SIZE, sizeof(inode_t));

    // updata the father directory
    memcpy(&(dentries_buffer[j].file_name), file_name, strlen(file_name));
    dentries_buffer[j].ino = ino;
    sdwrite(dentries_buffer, superblock.data_start + father_block_num * BLOCK_SIZE, MAX_DEN_SIZE);

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    inode_buffer.file_size += 3 * sizeof(dentry_t);
    inode_buffer.dentry_num ++;
    inode_buffer.links ++;
    inode_buffer.last_modified_time         = get_timer();
    inode_buffer.create_time                = get_timer();
    sdwrite(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));

    return 0;
}

int do_cat(char *file_name, int *location)
{
    int i, j;
    int current_ino = 0;

    for (i = 0; i < current_dir_depth; i++)
    {
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE,   sizeof(inode_t));
    	
    	// Assume directory only use 1 block
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }

        if(j == inode_buffer.dentry_num)
        {
            printk("[Error] Path Error in do_cat_1!");
            return 1;
        }
    }

    // to find the behind fi
    // Assume directory only use 1 block
    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

    for(j = 0; j < inode_buffer.dentry_num; j++)
    {
        if (!strcmp(file_name, dentries_buffer[j].file_name))
        {
            current_ino = dentries_buffer[j].ino;
            break;
        }
    }

    if(j == inode_buffer.dentry_num)
    {
        printk("[Error] Path Error in do_cat_2!");
        return 2; // Path Error in the middle path
    }

    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(block_buf, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, BLOCK_SIZE);

    vt100_move_cursor(1, *location + 1);
    printk("%s", block_buf);//remember to change !!!

    return 0;
}

int do_open(char *file_name, int access)
{
    int i, j;
    int current_ino = 0;

    for (i = 0; i < current_dir_depth; i++)
    {
    	sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE,   sizeof(inode_t));
    	
    	// Assume directory only use 1 block
    	sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

        for(j = 0; j < inode_buffer.dentry_num; j++)
            if (!strcmp(current_dir_name[i+1], dentries_buffer[j].file_name))
            {
                current_ino = dentries_buffer[j].ino;
                break;
            }

        if(j == inode_buffer.dentry_num)
        {
            printk("[Error] Path Error in do_open_1!");
            return 1;
        }
    }

    // to find the behind fi
    // Assume directory only use 1 block
    sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
    sdread(dentries_buffer, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, MAX_DEN_SIZE);

    for(j = 0; j < inode_buffer.dentry_num; j++)
    {
        if (!strcmp(file_name, dentries_buffer[j].file_name))
        {
            current_ino = dentries_buffer[j].ino;
            break;
        }
    }

    if(j == inode_buffer.dentry_num)
    {
        printk("[Error] Path Error in do_open_2!");
        return 2; // Path Error in the middle path
    }
	  sdread(&inode_buffer, superblock.inode_start + current_ino * INODE_SIZE, sizeof(inode_t));
	  
	  // Allocate fd
    for (i = 0; i < FD_NUM; i++)
    {
        if(fdesc[i].used == 0)
            break;
    }
    if(i == FD_NUM)
    {
        printk("[Error] No Available FD!");
        return -1;	
    }
    
    fdesc[i].ino    = current_ino;
    fdesc[i].access = access;
    fdesc[i].w_pos  = 0;
    fdesc[i].r_pos  = 0;
    fdesc[i].size   = inode_buffer.file_size;
    fdesc[i].used   = 1;
    
    return i;
}

int do_read(int fd, char *buff, int size)
{
    // Assume directory only use 1 block
    sdread(&inode_buffer, superblock.inode_start + fdesc[fd].ino * INODE_SIZE, sizeof(inode_t));
    sdread(block_buf, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, BLOCK_SIZE);
    
    memcpy(buff, block_buf + fdesc[fd].r_pos, size);
    fdesc[fd].r_pos = fdesc[fd].r_pos + size;

    return size;
}

int do_write(int fd, char *buff, int size)
{
    sdread(&inode_buffer, superblock.inode_start + fdesc[fd].ino * INODE_SIZE, sizeof(inode_t));
    sdread(block_buf, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, BLOCK_SIZE);

    memcpy(block_buf + fdesc[fd].w_pos, buff, size);

    sdwrite(block_buf, superblock.data_start + inode_buffer.direct_block[0] * BLOCK_SIZE, BLOCK_SIZE);
    fdesc[fd].w_pos = fdesc[fd].w_pos + size;

    return size;
}

void do_close(int fd)
{
    fdesc[fd].used = 0;
}

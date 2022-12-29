#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include "fs.h"
#include "SiSH/base.h"
#include "hash_set.h"
#include <string.h>
#include <unistd.h>

#define ERR_STREAM  stderr
#define OUT_STREAM  stdout
#define IN_STREAM   stdin

void print_super_block(struct super_block s);
void print_inode(struct inode i, int idx);

typedef struct _directory {
    char** directory;
    int dir_length;
} Directory;

typedef struct super_block SuperBlock;
typedef struct inode Inode;
typedef struct blocks Block;
typedef unsigned char Blockpiece;

typedef struct _mount_env {
    SuperBlock* super_block;
    Inode* inode_table;
    Block* block;
    int block_starting_point;
    int block_finishing_point;
    FILE* disk_img;
    Directory* current_directory;
    BST_set* set;
    DIR_FILES* dir_files;
} MountEnv;

FILE* mount(char* disk_img_name, SuperBlock** super_block, Inode** inode_table);

void* read_block(Inode inode, MountEnv* mount_env);

char* current_dir_to_string(Directory* current_directory, int len);

#endif // !__FILE_SYSTEM_H__

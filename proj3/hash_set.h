#ifndef __HASH_SET_H__
#define __HASH_SET_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 2147483647

typedef struct _pair {
    unsigned int inode;
    int hash;
    union { // name
		unsigned char name[256];
		unsigned char n_pad[16][16];
	};
} Pair;

typedef struct _set_node{
	Pair* pair;
	struct _set_node* left;
    struct _set_node* right;
} SetNode;

typedef struct {
    int root_inode;
	SetNode* root;
	int size;
} BST_set;

typedef struct _directory_file_list {
    char* file_list[1000];
    int size;
} DIR_FILES;

int hash_func(char* str, int len);
void insert_node(BST_set* set, Pair* pair);
int find_inode(BST_set* set, char* name);

#endif // !__HASH_SET_H__
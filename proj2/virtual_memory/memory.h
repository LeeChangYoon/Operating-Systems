#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>


/* 
 * Virtual Address Table structure
 *
 */
typedef struct TABLE {
	int* fn;
	int* tn;
	int state_bit;
	int* valid_bit;
	int* present_bit;
} TABLE;


TABLE* ptbl1;
TABLE* ptbl2;

int* lru;
int* lfu;
int* mfu;
int flag;
int* disk;
int* memory;
int* disk_ffl;
int* memory_ffl;
int disk_access;
int memory_access;
int memory_ffl_size;
int set_replacement;

int search_random();
int search_lru(int* ffl);
int search_lfu(int* ffl);
int search_mfu(int* ffl);

void virtual_memory_alloc();
int search_table(TABLE* table);
int search_frame(int* ffl, int option);
void MMU(int* va_arr, int idx, int time);
void copy_page(int* src, int src_idx, int* src_list, int* dest, int dest_idx, int* dest_list);

#endif //!__MEMORY_H__

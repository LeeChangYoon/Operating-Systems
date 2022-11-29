#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* 
 * Virtual Address Table structure
 *
 */
typedef struct TABLE {
	int* address;
	int* swap_bit;
	int* valid_bit;
	int* table_address;
	int level2_valid_bit;

	int* level1_address;
	int* level2_address;
} TABLE;


TABLE* level1;
TABLE* level2;

int* lru;
int* disk;
int* memory;
int* disk_frame_list;
int* memory_frame_list;
int memory_frame_list_size;


void memory_to_list();
int find_lru_page(int* list);
int find_free_table(TABLE* table);
void MMU(int idx, int* virtual_address, int time);
int find_free_frame(int* free_frame_list, int option);
void copy_page(int* tar1, int tar1_idx, int* tar1_list, int* tar2, int tar2_idx, int* tar2_list);

#endif //!__MEMORY_H__

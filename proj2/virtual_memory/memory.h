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
 * Structure of the virtual address table.
 * It contains the frame and page number.
 * Also, it contains the bits that represents the state, valid, and present of the table.
 */
typedef struct TABLE {
	int* fn; // frame number list
	int* tn; // table number list
	int state_bit; // state bit that distinguishes the usage of pabe table level 2
	int* valid_bit; // valid bit of page table level 2
	int* present_bit; // bit that distinguishes the swap of the page
} TABLE;


TABLE* ptbl1; // page table level 1
TABLE* ptbl2; // page table level 2


int* lru;
int* lfu; 
int* mfu;
int* sca;
int flag; 
int* fifo;
int* esca;
int* disk;
int* memory;
int* disk_ffl; // disk free-frame list
int* memory_ffl; // memory free-frame list
float ptbl1_hit; 
float ptbl2_hit; 
float ptbl1_fault; 
float ptbl2_fault;
int memory_ffl_size;
int set_replacement;
long int disk_access;
long int memory_access;

int search_random();
int search_lru(int* ffl);
int search_lfu(int* ffl);
int search_mfu(int* ffl);
int search_sca(int* ffl);
int search_fifo(int* ffl);
int search_esca(int* ffl);

void virtual_memory_free();
void virtual_memory_alloc();
int search_table(TABLE* table);
void q_cycle(int* ffl, int idx);
int search_frame(int* ffl, int option);
void MMU(int* va_arr, int idx, int time);
void copy_page(int* src, int src_idx, int* src_list, int* dest, int dest_idx, int* dest_list);

#endif //!__MEMORY_H__

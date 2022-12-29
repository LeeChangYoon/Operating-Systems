#ifndef __MEMORY_H__
#define __MEMORY_H__


#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "queue.h"

/* 
 * Translation Look-Aside Buffer structure
 *
 */
typedef struct TLB {
	int key;
	int valid;
	int value;
} TLB;


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


TLB** tlb;
TABLE* ptbl1; // page table level 1
TABLE* ptbl2; // page table level 2

int swap;
int* map;
int* lru;
int* lfu; 
int* mfu;
int flag; 
int* disk;
int* memory;
Queue* fifo;
int tlb_size;
int* disk_ffl; // disk free-frame list
int* memory_ffl; // memory free-frame list
float tlb_hit;
float tlb_miss;
float ptbl1_hit; 
float ptbl2_hit; 
float ptbl1_fault; 
float ptbl2_fault;
int memory_ffl_size;
int set_replacement;
long int tlb_access;
long int disk_access;
long int memory_access;

int search_random();
int search_lru(int* ffl);
int search_lfu(int* ffl);
int search_mfu(int* ffl);
int search_sca(Queue* q);
int search_fifo(Queue* q);
int search_esca(Queue* q);

void memory_init();
void virtual_memory_free();
void virtual_memory_alloc();
int search_table(TABLE* table);
void q_cycle(int* ffl, int idx);
int search_frame(int* ffl, int option);
void MMU(int* va_arr, int idx, int time);
void copy_page(int* src, int src_idx, int* src_list, int* dest, int dest_idx, int* dest_list);

#endif //!__MEMORY_H__

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/* 
 * Heap structure
 *
 * Structure of the heap that are used for ready and wait queues.
 * It contains the head and tail, which presents the front and rear node.
 * Count variable stores the length (or size) of the heap.
 */
typedef struct Heap {
	int count;
	Node** heap;
} Heap;


Heap* createHeap();
void removeHeap(Heap* h);
Node* deleteHeap(Heap* h);
void printHeap(Heap* h, char c);
void heapify(Heap* h, int index);
void fprintHeap(Heap* h, char c, FILE* fp);
void insertHeap(Heap* h, int idx, int cpu_burst, int io_burst);

#endif //!__HEAP_H__
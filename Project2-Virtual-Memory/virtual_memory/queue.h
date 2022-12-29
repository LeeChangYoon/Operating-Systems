#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>


/* 
 * PCB structure
 *
 * Structure of the Process Control Block (PCB).
 * The PCB is controlled by the parent process.
 * It contains the information of that are needed for the scheduling operations.
 */
typedef struct PCB {
	int idx;
	pid_t pid;
	int io_burst;
	int cpu_burst;
} PCB;


/* 
 * Node structure
 *
 * Structure of the node that are used in the queue.
 * It contains the PCB data and the node pointer that points the next node of the single linked list.
 */
typedef struct Node {
	int fn; // frame number
	int sca; // second chance bit
	PCB pcb; // process control block
	int esca; // enhanced second chance bit
	struct Node* next;
} Node;


/* 
 * Queue structure
 *
 * Structure of the queue that are used for ready and wait queues.
 * It contains the head and tail, which presents the front and rear node.
 * Count variable stores the length (or size) of the queue.
 */
typedef struct Queue {
	int count;
	Node* tail;
	Node* head;
} Queue;


Node* createNode();
Queue* createQueue();
int isEmpty(Queue* q);
Node* dequeue(Queue* q);
void removeQueue(Queue* q);
void printQueue(Queue* q, char c);
int searchQueue(Queue* q, int key);
void fprintQueue(Queue* q, char c, FILE* fp);
void enqueue(Queue* q, int idx, int cpu_burst, int io_burst, int fn, int sca, int esca);

#endif //!__QUEUE_H__

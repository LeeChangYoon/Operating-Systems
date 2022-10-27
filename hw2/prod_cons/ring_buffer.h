#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>

typedef struct Node{
	char* line;
	struct Node* next;
}Node ;

typedef struct RingBuffer{
	Node* head;
	Node* tail;
	volatile unsigned int size;
} RingBuffer;

void* dequeue(RingBuffer* rb);
void createQueue(RingBuffer** rb);
int enqueue(RingBuffer* rb, void* line);

void createQueue(RingBuffer** rb) {
	*rb = (RingBuffer*)malloc(sizeof(RingBuffer));
	if (rb == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	(*rb)->head = (Node*)malloc(sizeof(Node));
	(*rb)->tail = (*rb)->head;
	(*rb)->head->next = NULL;
	(*rb)->head->line = NULL;
}

int enqueue(RingBuffer* rb, void* line) {
	Node* temp;
	Node* node = (Node*)malloc(sizeof(Node));
	
	node->next = NULL;
	node->line = line;
	
	while (1) {
		temp = rb->tail;
		if (__sync_bool_compare_and_swap(&(temp->next), NULL, node)) break;
		else __sync_bool_compare_and_swap(&(rb->tail), temp, temp->next);
	}
	
	__sync_bool_compare_and_swap(&(rb->tail), temp, node);
	return 1;
}

void* dequeue(RingBuffer* rb) {
	void* line;
	Node* node;
	
	while (1) {
		node = rb->head;
		if (node->next == NULL) return NULL;
		if (__sync_bool_compare_and_swap(&(rb->head), node, node->next)) break;
	}

	line = (void*)node->next->line;
	free(node);
	
	return line;
}


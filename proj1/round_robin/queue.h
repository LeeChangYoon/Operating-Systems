#pragma once

#include <stdio.h>
#include <stdlib.h>


typedef struct PCB {
	int idx;
	pid_t pid;
	int io_burst;
	int cpu_burst;
} PCB;


typedef struct Node {
	PCB pcb;
	struct Node* next;
} Node;


typedef struct Queue {
	int count;
	Node* tail;
	Node* head;
} Queue;


int isEmpty(Queue* q) {
	return q->count == 0;
}


Node* createNode() {
	Node* node = (Node*)malloc(sizeof(Node));

	node->pcb.idx = -1;
	node->pcb.pid = -1;
	node->pcb.io_burst = -1;
	node->pcb.cpu_burst = -1;
	return node;
}


Queue* createQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));

	q->count = 0;
	q->tail = NULL;
	q->head = NULL;
	return q;
}


void printQueue(Queue* q, char c) {
	Node* node = q->head;
	if (c == 'r') printf("Ready: ");
	else if (c == 'w') printf("Wait: ");
	else {
		perror("char");
		exit(EXIT_FAILURE);
	}

	while (node) {
		printf("%d ", node->pcb.idx);
		node = node->next;
	}
	printf("\n");
}


void enqueue(Queue* q, int idx, int cpu_burst, int io_burst) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		perror("node");
		exit(EXIT_FAILURE);
	}

	node->next = NULL;
	node->pcb.idx = idx;
	node->pcb.io_burst = io_burst;
	node->pcb.cpu_burst = cpu_burst;

	if (q->head == NULL) {
		q->head = node;
		q->tail = node;
	}
	else {
		q->tail->next = node;
		q->tail = node;
	}
	q->count++;
}


Node* dequeue(Queue* q) {
	Node* node = q->head;
	Node* ret = (Node*)malloc(sizeof(Node));

	if (isEmpty(q)) {
		ret->pcb.idx = -1;
		ret->pcb.io_burst = -1;
		ret->pcb.cpu_burst = -1;
		return ret;
	}

	if (q->head->next == NULL) {
		q->head = NULL;
		q->tail = NULL;
	}
	else q->head = q->head->next;
	q->count--;

	*ret = *node;
	free(node);
	return ret;
}


void removeQueue(Queue* q) {
	while (!isEmpty(q)) {
		Node* node = q->head;
		q->head = q->head->next;
		q->count--;
		free(node);
	}
}
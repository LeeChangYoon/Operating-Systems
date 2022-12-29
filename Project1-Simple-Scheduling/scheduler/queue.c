#include "queue.h"

/* 
 * int isEmpty(Queue* q)
 *
 * Returns whether the following queue is empty or not.
 * If the queue is empty, it returns 1.
 * If the queue is not empty, it returns 0.
 */
int isEmpty(Queue* q) {
	return q->count == 0;
}


/* 
 * Node* createNode()
 *
 * Creates the new node and returns it.
 * It is used to create a new node for currently running ready and wait queue node.
 */
Node* createNode() {
	Node* node = (Node*)malloc(sizeof(Node));

	node->pcb.idx = -1;
	node->pcb.pid = -1;
	node->pcb.io_burst = -1;
	node->pcb.cpu_burst = -1;
	return node;
}


/* 
 * Queue* createQueue()
 *
 * Creates the new queue and returns it.
 * It is used to create a new queue for ready and wait queue.
 */
Queue* createQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));

	q->count = 0;
	q->tail = NULL;
	q->head = NULL;
	return q;
}


/* 
 * void printQueue(Queue* q, char c)
 *
 * Prints all of the data for each node in the following queue.
 * The initial print statement are fixed according to the first character of the name of the queue.
 */
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


/* 
 * void fprintQueue(Queue* q, char c)
 *
 * Prints all of the data for each node in the following queue to the text file.
 * The initial print statement are fixed according to the first character of the name of the queue.
 */
void fprintQueue(Queue* q, char c, FILE* fp) {
	Node* node = q->head;
	if (c == 'r') fprintf(fp, "Ready: ");
	else if (c == 'w') fprintf(fp, "Wait: ");
	else {
		perror("char");
		exit(EXIT_FAILURE);
	}

	while (node) {
		fprintf(fp, "%d ", node->pcb.idx);
		node = node->next;
	}
	fprintf(fp, "\n");
}


/* 
 * void enqueue(Queue* q, int idx, int cpu_burst, int io_burst)
 *
 * Enqueus the new node into the rear of the following queue.
 * As the node contains the information of the PCB block, it gets the information of the PCB as arguments.
 */
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


/* 
 * Node* dequeue(Queue* q)
 *
 * Denqueus the node from the front of the following queue.
 * After dequeuing the front node, it frees the former node and returns it.
 */
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


/* 
 * void removeQueue(Queue* q)
 *
 * Removes all of the nodes in the queue.
 * After removing them, it frees the queue.
 */
void removeQueue(Queue* q) {
	while (!isEmpty(q)) {
		Node* node = q->head;
		q->head = q->head->next;
		q->count--;
		free(node);
	}
	free(q);
}
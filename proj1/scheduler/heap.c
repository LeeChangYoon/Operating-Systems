#include "heap.h"

/* 
 * Heap* createHeap()
 *
 * Creates the new heap and returns it.
 * It is used to create a new ready heap..
 */
Heap* createHeap(int max_process) {
	Heap* h = (Heap*)malloc(sizeof(Heap));

	h->count = 0;
	h->heap = (Node**)malloc(sizeof(Node*) * max_process);
	return h;
}


/* 
 * void printHeap(Heap* h, char c)
 *
 * Prints all of the data for each node in the following heap.
 * The initial print statement are fixed according to the first character of the name of the heap.
 */
void printHeap(Heap* h, char c) {
	if (c == 'r') printf("Ready: ");
	else if (c == 'w') printf("Wait: ");
	else {
		perror("char");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < h->count; i++) printf("%d ", h->heap[i]->pcb.idx);
	printf("\n");
}


/* 
 * void fprintHeap(Queue* q, char c)
 *
 * Prints all of the data for each node in the following heap to the text file.
 * The initial print statement are fixed according to the first character of the name of the heap.
 */
void fprintHeap(Heap* h, char c, FILE* fp) {
	if (c == 'r') fprintf(fp, "Ready: ");
	else if (c == 'w') fprintf(fp, "Wait: ");
	else {
		perror("char");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < h->count; i++) fprintf(fp, "%d ", h->heap[i]->pcb.idx);
	fprintf(fp, "\n");
}


/*
 * parent(int i)
 * 
 * Get the index of the parent.
 */
int parent(int i) {
	return (i - 1) / 2;
}


/*
 * left_child(int i)
 *
 * Get the index of the left child.
 */
int left_child(int i) {
	return (2 * i + 1);
}


/* 
 * right_child(int i)
 *
 * Get the index of the right child.
 */
int right_child(int i) {
	return (2 * i + 2);
}


/*
 * get_min(Heap* h)
 *
 * Get the PCB with minumum CPU burst time
 */
Node* get_min(Heap* h) {
	return h->heap[0];
}


/*
 * void insertHeap(Heap* h, Node* node)
 *
 * Insert the new node into the following heap.
 * As the node contains the information of the PCB block, it gets the information of the PCB as arguments.
 */
void insertHeap(Heap* h, int idx, int cpu_burst, int io_burst) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	node->pcb.idx = idx;
	node->pcb.io_burst = io_burst;
	node->pcb.cpu_burst = cpu_burst;

	h->count++;
	h->heap[h->count - 1] = node;

	int cur = h->count - 1;
	while (cur > 0 && h->heap[parent(cur)]->pcb.cpu_burst > h->heap[cur]->pcb.cpu_burst) {
		Node* temp = h->heap[parent(cur)];
		h->heap[parent(cur)] = h->heap[cur];
		h->heap[cur] = temp;
		cur = parent(cur);
	}
}


/*
 * Node* deleteHeap(Heap* h)
 *
 * Deletes the node from the front of the following heap.
 * After deleting the front node, it frees the former node and returns it.
 */
Node* deleteHeap(Heap* h) {
	if (!h || h->count == 0) return NULL;
	Node* node = get_min(h);	


	Node* last = h->heap[h->count - 1];
	h->heap[0] = last;
	h->count--;

	heapify(h, 0);
	return node;
}


/*
 * Heap* heapify(Heap* h, int index)
 *
 * Readjust the following array into the heap.
 * The following operation will iterate until it generates the heap.
 */
void heapify(Heap* h, int index) {
	if (h->count <= 1) return;

	int smallest = index;
	int left = left_child(index);
	int right = right_child(index);

	if (left < h->count && h->heap[left]->pcb.cpu_burst < h->heap[index]->pcb.cpu_burst) smallest = left;
	if (right < h->count && h->heap[right]->pcb.cpu_burst < h->heap[smallest]->pcb.cpu_burst) smallest = right;

	if (smallest != index) {
		Node* temp = h->heap[index];
		h->heap[index] = h->heap[smallest];
		h->heap[smallest] = temp;
		heapify(h, smallest);
	}
}


/* 
 * void removeHeap(Heap* h)
 *
 * Removes all of the nodes in the heap.
 * After removing them, it frees the heap.
 */
void removeHeap(Heap* h) {
	free(h->heap);
}

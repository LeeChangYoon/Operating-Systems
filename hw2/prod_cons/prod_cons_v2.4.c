#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "ring_buffer.h"

FILE* rfile;
RingBuffer *rb;

void* producer(void *arg) {
    	int count = 0;
	size_t len = 0;
	ssize_t read = 0;
	char* line = NULL;
	int* ret = malloc(sizeof(int));
	
	while (1) {
		read = getdelim(&line, &len, '\n', rfile);
		if (read == -1) break;
		enqueue(rb, strdup(line));
		count++;
	}
	
	printf("Prod_%08x: %d lines\n", (unsigned int)pthread_self(), count);
	free(line);
	*ret = count;
	pthread_exit(ret);
}

void* consumer(void *arg) {
	int count = 0;
	char* line = NULL;
	int* ret = malloc(sizeof(int));

	while (1) {
		line = (char*)dequeue(rb);
		if (line == NULL) break;
		// printf("%s", line);
		count++;
     	}

	printf("Cons_%08x: %d lines\n", (unsigned int)pthread_self(), count);	
	*ret = count;
	pthread_exit(ret);
}

int main(int argc, char* argv[]) {
	int* ret;
	int Nprod, Ncons;
	pthread_t prod[100];
	pthread_t cons[100];
	rfile = fopen((char *) argv[1], "r");
	
	// wrong argument
        if (argc == 1) {
                printf("usage: ./prod_cons <readfile> #Producer #Consumer\n");
                exit (0);
        }

        // no file specification
        if (rfile == NULL) {
                perror("rfile");
                exit(0);
        }

	// no producer number specification
        if (argv[2] != NULL) {
                Nprod = atoi(argv[2]);
                if (Nprod > 100) Nprod = 100;
                if (Nprod == 0) Nprod = 1;
        } else Nprod = 1;

        // no consumer number specification
        if (argv[3] != NULL) {
                Ncons = atoi(argv[3]);
                if (Ncons > 100) Ncons = 100;
                if (Ncons == 0) Ncons = 1;
        } else Ncons = 1;

	
    	createQueue(&rb);
	pthread_attr_t attr;
    	pthread_attr_init(&attr);
	
	// thread initialization
	printf("main continuing\n");
    	for (int p = 0; p < Nprod; p++) pthread_create(&prod[p], &attr, producer, NULL);
    	for (int c = 0; c < Ncons; c++) pthread_create(&cons[c], &attr, consumer, NULL);
	
	// thread join
	for (int p = 0; p < Nprod; p++) {
		pthread_join(prod[p], (void**)&ret);
		printf("main: producer_%d joined with %d\n", p, *ret);
    	}
	for (int c = 0; c < Ncons; c++) {
        	pthread_join(cons[c], (void**)&ret);
		printf("main: consumer_%d joined with %d\n", c, *ret);
	}
    	
	pthread_exit(NULL);
	exit(0);
}

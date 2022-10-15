#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define BuffSize 100

typedef struct sharedobject {
	int nextin;
	int nextout;
	FILE* rfile;
	int consumer;
	char* line[BuffSize];
	
	sem_t full;
	sem_t empty;
	pthread_mutex_t lock;
} so_t;

void *producer(void *arg) {
	so_t *so = arg;
	int i = 0;
	int count = 0;
	size_t len = 0;
	ssize_t read = 0;
	char *line = NULL;
	FILE *rfile = so->rfile;
	int *ret = malloc(sizeof(int));
	
	while (1) {
		sem_wait(&so->empty);
		pthread_mutex_lock(&so->lock);		

		read = getdelim(&line, &len, '\n', rfile);
		if (read == -1) {
			so->line[so->nextin] = NULL;
			pthread_mutex_unlock(&so->lock);
			sem_post(&so->full);
			break;
		}

		so->line[so->nextin] = strdup(line); // share the line
		so->nextin = (so->nextin + 1) % BuffSize;
		count++;		

		pthread_mutex_unlock(&so->lock);
		sem_post(&so->full);
		sleep(0.1);
	}
	
	free(line);
	printf("Prod_%x: %d lines\n", (unsigned int)pthread_self(), count);
	*ret = count;
	pthread_exit(ret);
}

void *consumer(void *arg) {
	int len;
	int i = 0;
	char *line;
	int count = 0;
	so_t *so = arg;
	int *ret = malloc(sizeof(int));
	
	while (1) {	
		sem_wait(&so->full);
		pthread_mutex_lock(&so->lock);
			
		line = so->line[so->nextout];
		if (line == NULL) {
			pthread_mutex_unlock(&so->lock);
			sem_post(&so->empty);
			sem_post(&so->full);
			break;
		}
		
		len = strlen(line);
		printf("Cons_%x: [%02d:%02d] %s", (unsigned int)pthread_self(), count, so->nextout, line);
		so->nextout = (so->nextout + 1) % BuffSize;
		count++;		

		pthread_mutex_unlock(&so->lock);
		sem_post(&so->empty);	
		sleep(0.1);
	}

	printf("Cons: %d lines\n", count);
	*ret = count;
	pthread_exit(ret);
}


int main (int argc, char *argv[]) {
	int *ret;
	int i;
	FILE *rfile;
	int Nprod, Ncons;
	int rc;   long t;
	pthread_t prod[100];
	pthread_t cons[100];

	// wrong argument
	if (argc == 1) {
		printf("usage: ./prod_cons <readfile> #Producer #Consumer\n");
		exit (0);
	}

	so_t *share = malloc(sizeof(so_t));
	memset(share, 0, sizeof(so_t));
	rfile = fopen((char *) argv[1], "r");
	
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
	
	share->consumer = 0;
	share->rfile = rfile;
	for (i = 0; i < BuffSize; i++) share->line[i] = NULL;
	
	// mutex initialization
	share->nextin = 0;
	share->nextout = 0;
	sem_init(&share->full, 0, 0);
	sem_init(&share->empty, 0, BuffSize);
	pthread_mutex_init(&share->lock, NULL);
	
	// thred initialization
	printf("main continuing\n");
	for (i = 0 ; i < Nprod ; i++)
		pthread_create(&prod[i], NULL, producer, share);
	for (i = 0 ; i < Ncons ; i++)
		pthread_create(&cons[i], NULL, consumer, share);
	
	// thread join
	for (i = 0 ; i < Ncons ; i++) {
		rc = pthread_join(cons[i], (void **) &ret);
		printf("main: consumer_%d joined with %d\n", i, *ret);
	}
	for (i = 0 ; i < Nprod ; i++) {
		rc = pthread_join(prod[i], (void **) &ret);
		printf("main: producer_%d joined with %d\n", i, *ret);
	}

	pthread_exit(NULL);
	exit(0);
}


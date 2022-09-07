#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define ASCII_SIZE 256
#define BUFFER_SIZE 100
#define MAX_STRING_LENGTH 30

typedef struct sharedobject {
	int nextin;
	int nextout;
	FILE* rfile;
	int consumer;
	int stat2[ASCII_SIZE];
	char* line[BUFFER_SIZE];	
	int stat[MAX_STRING_LENGTH];
	
	sem_t full;
	sem_t empty;
	pthread_mutex_t lock;
} so_t;

void* producer(void* arg) {
	so_t* so = arg;
	int* ret = malloc(sizeof(int));
	FILE* rfile = so->rfile;
	int i = 0;
	int count = 0;
	char* line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	
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

		so->line[so->nextin] = strdup(line);      /* share the line */
		so->nextin = (so->nextin + 1) % BUFFER_SIZE;
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

void* consumer(void* arg) {
	int len;
	int i = 0;
	char* line;
	int count = 0;	
	so_t* so = arg;
	size_t length = 0;
	int* ret = malloc(sizeof(int));

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
			
		so->nextout = (so->nextout + 1) % BUFFER_SIZE;
		count++;		
		
		// gather the char stat for the sinlge line
		char *cptr = NULL;
		char *substr = NULL;
		char *brka = NULL;
		char *sep = "{}()[],;\" \n\t^";
		
		// for each line,
		cptr = line;
		for (substr = strtok_r(cptr, sep, &brka); substr;substr = strtok_r(NULL, sep, &brka)) {
			length = strlen(substr);
			cptr = cptr + length + 1;
			if (length >= 30) length = 30;
			so->stat[length-1]++;
			if (*cptr == '\0') break;
		}

		cptr = line;
		for (int i = 0 ; i < length ; i++) {
			if (*cptr < 256 && *cptr > 1) {
				so->stat2[*cptr]++;
			}
			cptr++;
		}

		pthread_mutex_unlock(&so->lock);
		sem_post(&so->empty);	
		sleep(0.1);	
	}

	printf("Cons: %d lines\n", count);
	*ret = count;
	pthread_exit(ret);
}

int main (int argc, char* argv[]) {
	pthread_t prod[100];
	pthread_t cons[100];
	int Nprod, Ncons;
	int rc;   long t;
	int* ret;
	int i;
	int sum;
	FILE* rfile;

	// wrong argument
	if (argc == 1) {
		printf("usage: ./prod_cons <readfile> #Producer #Consumer\n");
		exit (0);
	}

	so_t *share = malloc(sizeof(so_t));
	memset(share, 0, sizeof(so_t));
	rfile = fopen((char *) argv[1], "rb");
	
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
	memset(share->stat, 0, sizeof(share->stat));
	memset(share->stat2, 0, sizeof(share->stat2));
	for (i = 0; i < BUFFER_SIZE; i++) share->line[i] = NULL;
	
	// mutex initialization
	share->nextin = 0;
	share->nextout = 0;
	sem_init(&share->full, 0, 0);
	pthread_mutex_init(&share->lock, NULL);	
	sem_init(&share->empty, 0, BUFFER_SIZE);

	// thred initialization
	printf("main continuing\n");
	for (i = 0 ; i < Nprod ; i++)
		pthread_create(&prod[i], NULL, producer, share);
	for (i = 0 ; i < Ncons ; i++)
		pthread_create(&cons[i], NULL, consumer, share);
	
	// thread join
	for (i = 0 ; i < Ncons ; i++) {
		rc = pthread_join(cons[i], (void**) &ret);
		printf("main: consumer_%d joined with %d\n", i, *ret);
	}
	for (i = 0 ; i < Nprod ; i++) {
		rc = pthread_join(prod[i], (void**) &ret);
		printf("main: producer_%d joined with %d\n", i, *ret);
	}
	
	// sum
	sum = 0;
	for (i = 0; i < 30; i++) sum += share->stat[i];
	
	// print out the distributions
	printf("\n");
	printf("*** print out distributions *** \n");
	printf("  #ch  freq \n");
	for (i = 0; i < 30; i++) {
		int j = 0;
		int num_star = share->stat[i] * 80 / sum;
		printf("[%3d]: %4d \t", i + 1, share->stat[i]);
		for (j = 0; j < num_star; j++) printf("*");
		printf("\n");
	}
	printf("\n");
		
	pthread_exit(NULL);
	exit(0);
}


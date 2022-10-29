#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFERSIZE 4096

typedef struct threadobject {
	int fd;
	int index;
	long offset;
} to_t;

int terminate;
char** buffer;
long int fsize;
int Nprod, Ncons;
pthread_mutex_t* full;
pthread_mutex_t* empty;

void* producer(void* arg) {
	int result;
	int count = 0;
	to_t* to = arg;
	int offset = 0;
	lseek(to->fd, to->offset, SEEK_SET);
	char* block = (char*)malloc(sizeof(char) * BUFFERSIZE);

	while (1) {
		pthread_mutex_lock(&empty[to->index]);

		if (count >= fsize / Nprod) break;
		if (fsize / Nprod - count >= BUFFERSIZE) offset = BUFFERSIZE;
		else offset =  fsize / Nprod - count;

		result = read(to->fd, block, offset);
		buffer[to->index] = block;
		count += offset;		
		
		pthread_mutex_unlock(&full[to->index]);
	}
	
	// printf("P_%d: exit with %d\n", to->index, count);	
	pthread_exit(0);	
}

void* consumer(void* arg) {
	int index = 0;
	
	while (1) {
		if (pthread_mutex_trylock(&full[index]) == 0) {
			if (buffer[index] != NULL) {
				// printf("C_%d:\n%s\n\n", index, buffer[index]);
				buffer[index] = NULL;
				pthread_mutex_unlock(&empty[index]);
			}
		}
			
		else {
			if (terminate < Nprod) index = (index + 1) % Nprod;
			else break;
		}	
	}
	
	// printf("C_%d: exit\n", index);
	pthread_exit(0);
}


int main(int argc, char* argv[]) {

	// wrong argument
	if (argc == 1) {
		printf("usage: ./prod_cons <readfile> #Producer #Consumer\n");
		exit (0);
	}

	// file specification
	FILE* rfile;
	
	rfile = fopen((char *) argv[1], "r");
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

	to_t to[Nprod];	
	pthread_t prod[Nprod];	
	pthread_t cons[Ncons];

	buffer = (char**)malloc(sizeof(char*) * Nprod);
	if (buffer == NULL) {
		perror("malloc");
		exit(0);
	}
	memset(buffer, 0, sizeof(buffer));
	
	fseek(rfile, 0, SEEK_END);
	fsize = ftell(rfile);
	rewind(rfile);
	
	full = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * Nprod);
	for (int i = 0; i < Nprod; i++) pthread_mutex_init(&full[i], NULL);
	empty = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * Nprod);
	for (int i = 0; i < Nprod; i++) pthread_mutex_init(&empty[i], NULL);

	// thred initialization
	printf("main continuing\n");
	for (int i = 0 ; i < Nprod ; i++) {
		to[i].index = i;
		to[i].fd = open((char*) argv[1], O_RDONLY);		
		to[i].offset = fsize  - i * fsize / Nprod <= 0 ? fsize : i * fsize / Nprod;
			
		pthread_create(&prod[i], NULL, producer, &to[i]);
	}
	
	for (int i = 0 ; i < Ncons ; i++) pthread_create(&cons[i], NULL, consumer, NULL);
	
	// thread join
	int rc;
	terminate = 0;	

	for (int i = 0 ; i < Nprod ; i++) {
		rc = pthread_join(prod[i], (void**) NULL);
		// printf("main: producer_%d joined with \n", i);
		terminate++;
	}
	for (int i = 0 ; i < Ncons ; i++) {
		rc = pthread_join(cons[i], (void**) NULL);
		// printf("main: consumer_%d joined with \n", i);
	}
	
	printf("\n");	
	pthread_exit(NULL);
	free(buffer);
	exit(0);	
}

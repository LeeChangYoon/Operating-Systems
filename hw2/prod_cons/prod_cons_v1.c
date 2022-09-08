#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct sharedobject {
	FILE *rfile;
	int linenum;
	char *line;
	int buffsize;
	pthread_cond_t full;
	pthread_cond_t empty;
	pthread_mutex_t lock;
} so_t;

void *producer(void *arg) {
	so_t *so = arg;
	int *ret = malloc(sizeof(int));
	FILE *rfile = so->rfile;
	int i = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	
	while (1) {
		pthread_mutex_lock(&so->lock);
		while (so->buffsize == 1) pthread_cond_wait(&so->empty, &so->lock);		
			
		read = getdelim(&line, &len, '\n', rfile);
		if (read == -1) {
			so->line = NULL;
			so->buffsize = 1;
			pthread_cond_signal(&so->full);
			pthread_mutex_unlock(&so->lock);
			break;
		}

		so->linenum = i;
		so->line = strdup(line);      /* share the line */
		so->buffsize = 1;		
		i++;

		pthread_cond_signal(&so->full);
		pthread_mutex_unlock(&so->lock);
	}

	free(line);
	printf("Prod_%x: %d lines\n", (unsigned int)pthread_self(), i);
	*ret = i;
	pthread_exit(ret);
}

void *consumer(void *arg) {
	so_t *so = arg;
	int *ret = malloc(sizeof(int));
	int i = 0;
	int len;
	char *line;
	
	while (1) {
		pthread_mutex_lock(&so->lock);
		while (so->buffsize == 0) pthread_cond_wait(&so->full, &so->lock);		
	
		line = so->line;
		if (line == NULL) {
			break;
		}
		
		len = strlen(line);
		printf("Cons_%x: [%02d:%02d] %s", (unsigned int)pthread_self(), i, so->linenum, line);
		so->buffsize = 0;
		free(so->line);
		i++;
		
		pthread_cond_signal(&so->empty);
		pthread_mutex_unlock(&so->lock);	
	}

	printf("Cons: %d lines\n", i);
	*ret = i;
	pthread_exit(ret);
}


int main (int argc, char *argv[])
{
	pthread_t prod[100];
	pthread_t cons[100];
	int Nprod, Ncons;
	int rc;   long t;
	int *ret;
	int i;
	FILE *rfile;

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

	share->rfile = rfile;
	share->line = NULL;

	// mutex initialization
	pthread_cond_init(&share->full, NULL);
	pthread_cond_init(&share->empty, NULL);
	pthread_mutex_init(&share->lock, NULL);
	
	// thred initialization
	for (i = 0 ; i < Nprod ; i++)
		pthread_create(&prod[i], NULL, producer, share);
	for (i = 0 ; i < Ncons ; i++)
		pthread_create(&cons[i], NULL, consumer, share);
	printf("main continuing\n");
	
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


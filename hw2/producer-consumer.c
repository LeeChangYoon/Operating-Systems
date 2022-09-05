#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MaxItems 5
#define BufferSize 5

sem_t full;
sem_t empty;
int in = 0, out = 0;
pthread_mutex_t mutex;
int buffer[BufferSize];

void* producer(void* pno) {
	int item;
	for (int i = 0; i < MaxItems; i++) {
		item = rand();
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		buffer[in] = item;
		printf("Producer %d: Inset Item %d at %d\n", *((int*)pno), buffer[in], in);
		in = (in + 1) % BufferSize;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);	
	
	}
}

void* consumer(void* cno) {
	for (int i = 0; i < MaxItems; i++) {
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		int item = buffer[out];
		printf("Consumer %d: Remove Item %d from %d\n", *((int*)cno), item, out);
		out = (out + 1) % BufferSize;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int main() {
	pthread_t pro[5], con[5];
	pthread_mutex_init(&mutex, NULL);
	
	sem_init(&full, 0, 0);	
	sem_init(&empty, 0, BufferSize);
	
	int i;
	int a[5] = {1, 2, 3, 4, 5};
	
	for (i = 0; i < 5; i++) 
		pthread_create(&pro[i], NULL, (void*)producer, (void*)&a[i]);

	for (i = 0; i < 5; i++)
		pthread_create(&con[i], NULL, (void*)consumer, (void*)&a[i]);

	for (i = 0; i < 5; i++)
		pthread_join(pro[i], NULL);
	
	for (i = 0; i < 5; i++)
		pthread_join(con[i], NULL);
	
	sem_destroy(&full);
	sem_destroy(&empty);
	pthread_mutex_destroy(&mutex);
	
	return 0;
}


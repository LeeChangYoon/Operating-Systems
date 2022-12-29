#ifndef __MSG_H__
#define __MSG_H__

#include "queue.h"

#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* 
 * Message Buffer for Scheduling structure
 *
 */
typedef struct msgbuf_schedule {
	long  mtype;

	// pid will sleep for io_time
	int pid;
	int io_time;
	int cpu_time;
} msgbuf_schedule;


/* 
 * Message Buffer for Virtual Address structure
 *
 */
typedef struct msgbuf_memory {
	long  mtype;

	// pid will sleep for io_time
	int va_arr[10];
} msgbuf_memory;


void cmsgsnd_schedule(int k, int c, int io);
void pmsgrcv_schedule(int idx, Node* node);

void cmsgsnd_memory(int idx, int* va_arr);
void pmsgrcv_memory(int idx, int* va_buffer);


#endif //!__MSG_H__

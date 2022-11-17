#ifndef __MSG_H__
#define __MSG_H__

#include "queue.h"

#include <string.h>

#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>

/* 
 * PCB structure
 *
 * Structure of the message for the message queue.
 * It contains the information of that are needed for the IPC communications.
 */
typedef struct msgbuf {
	long  mtype;

	// pid will sleep for io_time
	int pid;
	int io_time;
	int cpu_time;
} msgbuf;


void cmsgsnd(int k, int c, int io);
void pmsgrcv(int idx, Node* node);


#endif //!__MSG_H__
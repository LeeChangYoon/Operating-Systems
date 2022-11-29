#include "msg.h"

/*
 * void cmsgsnd_schedule(int k, int c, int io)
 *
 * Message send function of the child process.
 * Child process sends the message structure to the parent process through the IPC communication.
 * Used IPC communication method is message queuing.
 */
void cmsgsnd_schedule(int k, int c, int io) {
	int qid = msgget(k, IPC_CREAT | 0666);

	msgbuf_schedule msg;
	memset(&msg, 0, sizeof(msgbuf_schedule));

	msg.mtype = 1;
	msg.io_time = io;
	msg.cpu_time = c;
	msg.pid = getpid();

	if (msgsnd(qid, &msg, sizeof(msgbuf_schedule) - sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
}


/*
 * void pmsgrcv_schedule(int idx, Node* node)
 *
 * Message receive function of the parent process.
 * Parent process receives the message structure from the child process through the IPC communication.
 * Used IPC communication method is message queuing.
 */
void pmsgrcv_schedule(int idx, Node* node) {
	int k = 0x12345 * (idx + 1);
	int qid = msgget(k, IPC_CREAT | 0666);

	msgbuf_schedule msg;
	memset(&msg, 0, sizeof(msgbuf_schedule));

	if (msgrcv(qid, &msg, sizeof(msgbuf_schedule) - sizeof(long), 0, 0) == -1) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	node->pcb.pid = msg.pid;
	node->pcb.io_burst = msg.io_time;
	node->pcb.cpu_burst = msg.cpu_time;
}


/*
 * void cmsgsnd_memory(int idx, int* virtual_address) 
 *
 */
void cmsgsnd_memory(int idx, int* virtual_address) {
	int key = 0x80000 * (idx + 1);
	int qid = msgget(key, IPC_CREAT | 0666);

	struct msgbuf_memory msg;
	memset(&msg, 0, sizeof(msgbuf_memory));

	msg.mtype = 1;
	for (int i = 0; i < 10; i++) {
		msg.virtual_address[i] = virtual_address[i];
	}
	
	if (msgsnd(qid, &msg, sizeof(msgbuf_memory) - sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
}


/*
 * void pmsgrcv_memory(int idx, int* virtual_address_buffer) 
 *
 */
void pmsgrcv_memory(int idx, int* virtual_address_buffer) {
	int key = 0x80000 * (idx + 1);
	int qid = msgget(key, IPC_CREAT | 0666);

	struct msgbuf_memory msg;
	memset(&msg, 0, sizeof(msgbuf_memory));

	if (msgrcv(qid, &msg, sizeof(msgbuf_memory) - sizeof(long), 0, 0) == -1) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 10; i++) {
		virtual_address_buffer[i] = msg.virtual_address[i];
	}
}

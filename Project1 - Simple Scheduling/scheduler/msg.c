#include "msg.h"

/*
 * void cmsgsnd(int k, int c, int io)
 *
 * Message send function of the child process.
 * Child process sends the message structure to the parent process through the IPC communication.
 * Used IPC communication method is message queuing.
 */
void cmsgsnd(int k, int c, int io) {
	int qid = msgget(k, IPC_CREAT | 0666);

	msgbuf msg;
	memset(&msg, 0, sizeof(msgbuf));

	msg.mtype = 1;
	msg.io_time = io;
	msg.cpu_time = c;
	msg.pid = getpid();

	if (msgsnd(qid, &msg, sizeof(msgbuf) - sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
}


/*
 * void pmsgrcv(int idx, Node* node)
 *
 * Message receive function of the parent process.
 * Parent process receives the message structure from the child process through the IPC communication.
 * Used IPC communication method is message queuing.
 */
void pmsgrcv(int idx, Node* node) {
	int k = 0x12345 * (idx + 1);
	int qid = msgget(k, IPC_CREAT | 0666);

	msgbuf msg;
	memset(&msg, 0, sizeof(msgbuf));

	if (msgrcv(qid, &msg, sizeof(msgbuf) - sizeof(long), 0, 0) == -1) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	node->pcb.pid = msg.pid;
	node->pcb.io_burst = msg.io_time;
	node->pcb.cpu_burst = msg.cpu_time;
}
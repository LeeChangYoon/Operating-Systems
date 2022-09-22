#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#include "msg.h"
#include "queue.h"


#define RUN_TIME 10000
#define TIME_TICK 10000
#define MAX_PROCESS 10
#define TIME_QUANTUM 5


int counter;
int run_time;
Queue* waitq;
Queue* readyq;
Node* cur_wait;
Node* cur_ready;
int key[MAX_PROCESS];
pid_t cpid[MAX_PROCESS];
int io_burst[MAX_PROCESS];
int cpu_burst[MAX_PROCESS];


void cmsgsnd(int k, int c, int io) {
	int qid = msgget(k, IPC_CREAT | 0666);

	msgbuf msg;
	memset(&msg, 0, sizeof(msgbuf));

	msg.mtype = 1;
	msg.io_time = io;
	msg.pid = getpid();
	msg.cpu_time = c;

	if (msgsnd(qid, &msg, sizeof(msgbuf) - sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
}



void pmsgrcv(int idx, PCB* pcb) {
	int k = 0x12345 * (idx + 1);
	int qid = msgget(k, IPC_CREAT | 0666);

	msgbuf msg;
	memset(&msg, 0, sizeof(msgbuf));

	if (msgrcv(qid, &msg, sizeof(msgbuf) - sizeof(long), 0, 0) == -1) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	pcb->pid = msg.pid;
	pcb->io_burst = msg.io_time;
	pcb->cpu_burst = msg.cpu_time;
}


void signal_io(int signo) {
	pmsgrcv(cur_ready->pcb.idx, cur_ready);
	if (cur_ready->pcb.io_burst == 0) enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
	else enqueue(waitq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
	cur_ready = dequeue(readyq);
	counter = 0;
}


void signal_rr(int signo) {
	counter++;
	if (counter >= TIME_QUANTUM) {
		enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
		cur_ready = dequeue(readyq);
		counter = 0;
	}
}


void signal_count(int signo) {
	printf("Time: %d\n", RUN_TIME - run_time);
	printf("Process: %d -> %d\n", cur_ready->pcb.idx, cur_ready->pcb.cpu_burst);
	printQueue(readyq, 'r');
	printQueue(waitq, 'w');
	printf("\n");

	int length = waitq->count;
	Node* node = waitq->head;
	for (int i = 0; i < length; i++) {
		cur_wait = dequeue(waitq);
		cur_wait->pcb.io_burst--;

		if (cur_wait->pcb.io_burst == 0) enqueue(readyq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
		else enqueue(waitq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
	}

	if (cur_ready->pcb.idx != -1) kill(cpid[cur_ready->pcb.idx], SIGCONT);
	run_time--;
}


 int main(int argc, char* argv[]) {
	counter = 0;
	pid_t ppid = getpid();
	run_time = RUN_TIME;
	srand((unsigned int)time(NULL));

	struct itimerval new_itimer;
	struct itimerval old_itimer;
	new_itimer.it_interval.tv_sec = 0;
	new_itimer.it_interval.tv_usec = TIME_TICK;
	new_itimer.it_value.tv_sec = 1;
	new_itimer.it_value.tv_usec = 0;

	struct sigaction io;
	struct sigaction rr;
	struct sigaction count;
	memset(&rr, 0, sizeof(rr));
	memset(&io, 0, sizeof(io));
	memset(&count, 0, sizeof(count));
	rr.sa_handler = &signal_rr;
	io.sa_handler = &signal_io;
	count.sa_handler = &signal_count;
	sigaction(SIGALRM, &count, NULL);
	sigaction(SIGUSR1, &rr, NULL);
	sigaction(SIGUSR2, &io, NULL);

	waitq = createQueue();
	readyq = createQueue();
	cur_wait = createNode();
	cur_ready = createNode();
	if (waitq == NULL || readyq == NULL || cur_wait == NULL || cur_ready == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < MAX_PROCESS; i++) {
		key[i] = 0x12345 * (i + 1);
		msgctl(msgget(key[i], IPC_CREAT | 0666), IPC_RMID, NULL);
	}

	for (int i = 0; i < MAX_PROCESS; i++) {
		io_burst[i] = rand() % 20 + 1;
		cpu_burst[i] = rand() % 20 + 1;
	}


	for (int i = 0; i < MAX_PROCESS; i++) {
		pid_t pid;
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		else if (pid == 0) {
			int idx = i;
			int p_io = io_burst[i];
			int p_cpu = cpu_burst[i];

			kill(getpid(), SIGSTOP);

			while (1) {
				p_cpu--;
				if (p_cpu == 0) {
					p_cpu = cpu_burst[idx];
					cmsgsnd(key[idx], p_cpu, p_io);
					p_io = io_burst[idx];

					kill(ppid, SIGUSR2);
				}
				else {
					kill(ppid, SIGUSR1);
				}

				kill(getpid(), SIGSTOP);
			}
		}

		else {
			cpid[i] = pid;
			enqueue(readyq, i, cpu_burst[i], io_burst[i]);
		}
	}

	cur_ready = dequeue(readyq);
	setitimer(ITIMER_REAL, &new_itimer, &old_itimer);

	while (run_time != -1);
	for (int i = 0; i < MAX_PROCESS; i++) {
		msgctl(msgget(key[i], IPC_CREAT | 0666), IPC_RMID, NULL);
		kill(cpid[i], SIGKILL);
	}

	removeQueue(waitq);
	removeQueue(readyq);
	return 0;
}
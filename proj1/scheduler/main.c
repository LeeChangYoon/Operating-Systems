#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#include "msg.h"
#include "heap.h"
#include "queue.h"
#include "u_signal.h"

#define TIME_TICK 1000 // SIGALRM interval time -> 0.001s (10ms)

int max_limit;

int io_burst[MAX_PROCESS];
int cpu_burst[MAX_PROCESS];

int main(int argc, char* argv[]) {
	counter = 0;
	wait_time = 0;
	proc_count = 0;
	turnaround_time = 0;
	run_time = RUN_TIME;
	pid_t ppid = getpid();
	srand((unsigned int)time(NULL));
	
	max_limit = atoi(argv[2]);
	set_scheduler = atoi(argv[1]);

	// initialize the file
	switch (set_scheduler) {
	case 1: fp = fopen("fcfs_dump.txt", "w"); break;
	case 2: fp = fopen("sjf_dump.txt", "w"); break;
	case 3:	fp = fopen("rr_dump.txt", "w"); break;
	default: 
		perror("scheduler");
		exit(EXIT_FAILURE);
	}
	
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	fclose(fp);

	// set the timer for SIGALRM
	struct itimerval new_itimer;
	struct itimerval old_itimer;

	new_itimer.it_interval.tv_sec = 0;
	new_itimer.it_interval.tv_usec = TIME_TICK;
	new_itimer.it_value.tv_sec = 1;
	new_itimer.it_value.tv_usec = 0;

	struct sigaction io;
	struct sigaction count;
	struct sigaction scheduler;

	memset(&io, 0, sizeof(io));
	memset(&count, 0, sizeof(count));
	memset(&scheduler, 0, sizeof(scheduler));

	io.sa_handler = &signal_io;
	count.sa_handler = &signal_count;

	switch (set_scheduler) {
	case 1: scheduler.sa_handler = &signal_fcfs; break;
	case 2: scheduler.sa_handler = &signal_sjf; break;
	case 3: scheduler.sa_handler = &signal_rr; break;
	default: 
		perror("scheduler");
		exit(EXIT_FAILURE);
	}

	sigaction(SIGUSR2, &io, NULL);
	sigaction(SIGALRM, &count, NULL);
	sigaction(SIGUSR1, &scheduler, NULL);

	// create the nodes and the queues for the scheduling operation
	waitq = createQueue();
	cur_wait = createNode();
	cur_ready = createNode();
	if (set_scheduler == 2) readyq = createHeap(MAX_PROCESS); else readyq = createQueue();
	if (waitq == NULL || readyq == NULL || cur_wait == NULL || cur_ready == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// create the message queues for each child process.
	for (int i = 0; i < MAX_PROCESS; i++) {
		key[i] = 0x12345 * (i + 1);
		msgctl(msgget(key[i], IPC_CREAT | 0666), IPC_RMID, NULL);
	}

	// set the random cpu and io bursts.
	switch (set_scheduler) {
	case 1: fp = fopen("fcfs_dump.txt", "a+"); break;
	case 2: fp = fopen("sjf_dump.txt", "a+"); break;
	case 3:	fp = fopen("rr_dump.txt", "a+"); break;
	default: 
		perror("scheduler");
		exit(EXIT_FAILURE);
	}
	
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	printf("-----------------------------------------\n");
	fprintf(fp, "-----------------------------------------\n");
	
	for (int i = 0; i < MAX_PROCESS; i++) {
		io_burst[i] = rand() % max_limit + 1;
		cpu_burst[i] = rand() % max_limit + 1;

		printf("Process %d\t-> CPU:\t%d,\tIO:\t%d\n", i, cpu_burst[i], io_burst[i]);	
		fprintf(fp, "Process %d\t-> CPU:\t%d,\tIO:\t%d\n", i, cpu_burst[i], io_burst[i]); 
	}
	
	printf("-----------------------------------------\n\n");
	fprintf(fp, "-----------------------------------------\n\n");
	fclose(fp);

	// creates the child process and operates the processes.
	for (int i = 0; i < MAX_PROCESS; i++) {
		pid_t pid;

		// error on the fork operation.
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		// child process
		else if (pid == 0) {
			int idx = i;
			int c_io = io_burst[i];
			int c_cpu = cpu_burst[i];

			// child process waits until the interrupt occurs.
			kill(getpid(), SIGSTOP);

			// child operates the cpu burst.
			while (1) {
				c_cpu--; // decreases the cpu burst by 1.i
				if (c_cpu == 0) {
					c_cpu = cpu_burst[i];

					// send the PCB data of the child process to the parent process.
					cmsgsnd(key[idx], c_cpu, c_io);
					c_io = io_burst[i];
					kill(ppid, SIGUSR2);
				}
				else { // cpu burst is over.
					kill(ppid, SIGUSR1);
				}

				// child proces waits for the next interrupt.
				kill(getpid(), SIGSTOP);
			}
		}

		else {
			cpid[i] = pid;
			arrival_time[i] = RUN_TIME - run_time;
			if (set_scheduler == 2) insertHeap(readyq, i, cpu_burst[i], io_burst[i]); 
			else enqueue(readyq, i, cpu_burst[i], io_burst[i]);
		}
	}

	// gets the node from the ready queue.
	if (set_scheduler == 2) cur_ready = deleteHeap(readyq);
	else cur_ready = dequeue(readyq);
	setitimer(ITIMER_REAL, &new_itimer, &old_itimer);

	// parent process operation.
	while (1);
	return 0;
}


/*
 * void dump_data(FILE* fp)
 *
 * Writes the data of ready queue dump and wait queue dump on the text file.
 */
void dump_data(FILE* fp) {
	switch (set_scheduler) {
	case 1: fp = fopen("fcfs_dump.txt", "a+"); break;
	case 2: fp = fopen("sjf_dump.txt", "a+"); break;
	case 3: fp = fopen("rr_dump.txt", "a+"); break;
	default:
		perror("scheduler");
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "-----------------------------------------\n");
	fprintf(fp, "Time: %d\n", RUN_TIME - run_time);
	fprintf(fp, "CPU Process: %d -> %d\n", cur_ready->pcb.idx, cur_ready->pcb.cpu_burst);
	
	for (int i = 0; i < waitq->count; i++) {
		cur_wait = dequeue(waitq);
		fprintf(fp, "I/O Process: %d -> %d\n", cur_wait->pcb.idx, cur_wait->pcb.io_burst);
		enqueue(waitq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
	
	}
	
	if (set_scheduler == 2) fprintHeap(readyq, 'r', fp); 
	else fprintQueue(readyq, 'r', fp);
	fprintQueue(waitq, 'w', fp);
	fprintf(fp, "-----------------------------------------\n");
	fclose(fp);
}


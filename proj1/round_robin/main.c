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
#include "queue.h"


#define RUN_TIME 10000 // limitation of the program run time.
#define MAX_PROCESS 10 // number of the child process. 
#define TIME_QUANTUM 5 // time quantum for the round robin (RR) algorithm
#define TIME_TICK 1000 // SIGALRM interval time -> 0.001s (10ms)


FILE* fp;
int counter;
int run_time;
Queue* waitq;
double wait_time;
Queue* readyq;
int proc_count;
Node* cur_wait;
Node* cur_ready;
double turnaround_time;
int key[MAX_PROCESS];
pid_t cpid[MAX_PROCESS];
int io_burst[MAX_PROCESS];
int end_time[MAX_PROCESS];
int cpu_burst[MAX_PROCESS];
int service_time[MAX_PROCESS];
int arrival_time[MAX_PROCESS];


void signal_io(int signo);
void signal_rr(int signo);
void dump_data(FILE* fp);
void signal_count(int signo);


int main(int argc, char* argv[]) {
	counter = 0;
	wait_time = 0;
	proc_count = 0;
	turnaround_time = 0;
	run_time = RUN_TIME;
	pid_t ppid = getpid();
	srand((unsigned int)time(NULL));

	// initialize the file
	fp = fopen("rr_dump.txt", "w");
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
	struct sigaction rr;
	struct sigaction count;

	memset(&rr, 0, sizeof(rr));
	memset(&io, 0, sizeof(io));
	memset(&count, 0, sizeof(count));

	rr.sa_handler = &signal_rr;
	io.sa_handler = &signal_io;
	count.sa_handler = &signal_count;

	sigaction(SIGUSR1, &rr, NULL);
	sigaction(SIGUSR2, &io, NULL);
	sigaction(SIGALRM, &count, NULL);

	// create the nodes and the queues for the scheduling operation
	waitq = createQueue();
	readyq = createQueue();
	cur_wait = createNode();
	cur_ready = createNode();
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
	for (int i = 0; i < MAX_PROCESS; i++) {
		io_burst[i] = rand() % 20 + 1;
		cpu_burst[i] = rand() % 20 + 1;
	}

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
					c_cpu = cpu_burst[idx];

					// send the PCB data of the child process to the parent process.
					cmsgsnd(key[idx], c_cpu, c_io);
					c_io = io_burst[idx];
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
			enqueue(readyq, i, cpu_burst[i], io_burst[i]);
		}
	}

	// gets the node from the ready queue.
	cur_ready = dequeue(readyq);
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
	fp = fopen("rr_dump.txt", "a+");
	fprintf(fp, "-----------------------------------------\n");
	fprintf(fp, "Time: %d\n", RUN_TIME - run_time);
	fprintf(fp, "CPU Process: %d -> %d\n", cur_ready->pcb.idx, cur_ready->pcb.cpu_burst);
	
	for (int i = 0; i < waitq->count; i++) {
		cur_wait = dequeue(waitq);
		fprintf(fp, "I/O Process: %d -> %d\n", cur_wait->pcb.idx, cur_wait->pcb.io_burst);
		enqueue(waitq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
	
	}
	
	fprintQueue(readyq, 'r', fp);
	fprintQueue(waitq, 'w', fp);
	fprintf(fp, "-----------------------------------------\n");
	fclose(fp);
}


/*
 * void signal_io(int signo)
 *
 * Signal handler that checks whether the child process has the IO burst or not.
 * If it has the remained IO burst, it enqueues the process into the wait queue.
 * If not, it enqueues the process into the ready queue.
 */
void signal_io(int signo) {
	pmsgrcv(cur_ready->pcb.idx, cur_ready);
	if (cur_ready->pcb.io_burst == 0) enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
	else enqueue(waitq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
	cur_ready = dequeue(readyq);
	counter = 0;
}


/*
 * void signal_rr(int signo)
 *
 * Signal handler that enqueues the current cpu process into the end of the ready queue
 * and dequeues the next process from the ready queue which is to be executed.
 */
void signal_rr(int signo) {
	counter++;
	cur_ready->pcb.cpu_burst--;
	if (counter >= TIME_QUANTUM) {
		enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
		cur_ready = dequeue(readyq);
		counter = 0;
	}
}


/*
 * void signal_count(int signo)
 *
 * Signal handler of SIGALRM wihch is called for evey count. 
 */
void signal_count(int signo) {
	printf("-----------------------------------------\n");
	printf("Time: %d\n", RUN_TIME - run_time);
	printf("CPU Process: %d -> %d\n", cur_ready->pcb.idx, cur_ready->pcb.cpu_burst);
	service_time[cur_ready->pcb.idx]++;	

	if (cur_ready->pcb.cpu_burst == 1) {
		proc_count++;
		end_time[cur_ready->pcb.idx] = RUN_TIME - run_time;
		wait_time += end_time[cur_ready->pcb.idx] - arrival_time[cur_ready->pcb.idx];
		turnaround_time += end_time[cur_ready->pcb.idx] - service_time[cur_ready->pcb.idx] - arrival_time[cur_ready->pcb.idx];
	}
	
	int length = waitq->count;
	for (int i = 0; i < length; i++) {
		cur_wait = dequeue(waitq);
		cur_wait->pcb.io_burst--;
		printf("I/O Process: %d -> %d\n", cur_wait->pcb.idx, cur_wait->pcb.io_burst);
		if (cur_wait->pcb.io_burst == 0) {
			enqueue(readyq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
			arrival_time[cur_wait->pcb.idx] = RUN_TIME - run_time;
			service_time[cur_wait->pcb.idx] = 0;
			end_time[cur_wait->pcb.idx] = 0;
		}
		else enqueue(waitq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
	}
	printQueue(readyq, 'r');
	printQueue(waitq, 'w');
	printf("-----------------------------------------\n");
	
	dump_data(fp);
	
	if (cur_ready->pcb.idx != -1) kill(cpid[cur_ready->pcb.idx], SIGCONT);

	if (run_time != 0) {
		run_time--;
	}
	else {
		printf("-----------------------------------------\n");
		printf("Result\n");
		printf("Troughput: %0.3f\n", (float)proc_count / RUN_TIME);
		printf("Average Wait Time: %0.3f\n", wait_time / proc_count);
		printf("Average Turnaround Time: %0.3f\n", turnaround_time / proc_count);
		printf("-----------------------------------------\n\n");
		
		fp = fopen("rr_dump.txt", "a+");	
		fprintf(fp, "-----------------------------------------\n");
		fprintf(fp, "Result\n");
		fprintf(fp, "Troughput: %0.3f\n", (float)proc_count / RUN_TIME);
		fprintf(fp, "Average Wait Time: %0.3f\n", wait_time / proc_count);
		fprintf(fp, "Average Turnaround Time: %0.3f\n", turnaround_time / proc_count);		
		fprintf(fp, "-----------------------------------------\n\n");

		for (int i = 0; i < MAX_PROCESS; i++) {
			msgctl(msgget(key[i], IPC_CREAT | 0666), IPC_RMID, NULL);
			kill(cpid[i], SIGKILL);
		}
		
		fclose(fp);
		free(cur_wait);
		free(cur_ready);
		removeQueue(waitq);
		removeQueue(readyq);
		exit(EXIT_SUCCESS);
	}
}

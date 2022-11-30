#ifndef __U_SIGNAL_H__
#define __U_SIGNAL_H__

#include <stdio.h>
#include "msg.h"
#include "heap.h"
#include "queue.h"
#include "memory.h"
#include <signal.h>

#define MAX_PROCESS 10 // number of the child process. 
#define RUN_TIME 10000 // limitation of the program run time.
#define TIME_QUANTUM 5 // time quantum for the round robin (RR) algorithm

FILE* fp;

Queue* waitq;

Node* cur_wait;
Node* cur_ready;

int counter;
int proc_count;
int run_time;
int set_scheduler;

double wait_time;
double turnaround_time;

void* readyq;

int key[MAX_PROCESS];

int arrival_time[MAX_PROCESS];
int end_time[MAX_PROCESS];
int service_time[MAX_PROCESS];

pid_t cpid[MAX_PROCESS];

void dump_data(FILE* fp);
void signal_io(int signo);
void signal_rr(int signo);
void signal_sjf(int signo);
void signal_fcfs(int signo);
void signal_count(int signo);

#endif //!__SIGNAL_H__

#include "signal.h"


/*
 * void signal_io(int signo)
 *
 * Signal handler that checks whether the child process has the IO burst or not.
 * If it has the remained IO burst, it enqueues the process into the wait queue.
 * If not, it enqueues the process into the ready queue.
 */
void signal_io(int signo) {
	int va_arr[10];
	pmsgrcv_schedule(cur_ready->pcb.idx, cur_ready);
	pmsgrcv_memory(cur_ready->pcb.idx, va_arr);
	MMU(va_arr, cur_ready->pcb.idx, RUN_TIME - run_time);

	if (cur_ready->pcb.io_burst == 0) { 
		if (set_scheduler == 2) insertHeap(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
		else enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst, 0, 0, 0);
	}

	else enqueue(waitq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst, 0, 0, 0);
	
	if (set_scheduler == 2) cur_ready = deleteHeap(readyq);
	else cur_ready = dequeue(readyq);
	counter = 0;
}


/*
 * void signal_fcfs(int signo)
 *
 * Signal handler that enqueues the current cpu process into the end of the ready queue
 * and dequeues the next process from the ready queue which is to be executed.
 */
void signal_fcfs(int signo) {
	int va_arr[10];
	cur_ready->pcb.cpu_burst--;

	pmsgrcv_memory(cur_ready->pcb.idx, va_arr);
	MMU(va_arr, cur_ready->pcb.idx, RUN_TIME - run_time);

	if (cur_ready->pcb.cpu_burst == 0) {
		enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst, 0, 0, 0);
		cur_ready = dequeue(readyq);
	}
}


/*
 * void signal_sjf(int signo)
 *
 * Signal handler that insert the current cpu process into the end of the ready queue
 * and deletes the next process from the ready queue which is to be executed.
 */
void signal_sjf(int signo) {
	counter++;
	int va_arr[10];
	cur_ready->pcb.cpu_burst--;

	pmsgrcv_memory(cur_ready->pcb.idx, va_arr);
	MMU(va_arr, cur_ready->pcb.idx, RUN_TIME - run_time);

	if (cur_ready->pcb.cpu_burst == 0) {
		insertHeap(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst);
		cur_ready = deleteHeap(readyq);
	}
}


/*
 * void signal_rr(int signo)
 *
 * Signal handler that enqueues the current cpu process into the end of the ready queue
 * and dequeues the next process from the ready queue which is to be executed.
 */
void signal_rr(int signo) {
	counter++;
	int va_arr[10];
	cur_ready->pcb.cpu_burst--;

	pmsgrcv_memory(cur_ready->pcb.idx, va_arr);
	MMU(va_arr, cur_ready->pcb.idx, RUN_TIME - run_time);

	if (counter >= TIME_QUANTUM) {
		enqueue(readyq, cur_ready->pcb.idx, cur_ready->pcb.cpu_burst, cur_ready->pcb.io_burst, 0, 0, 0);
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
			if (set_scheduler == 2) insertHeap(readyq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst);
			else enqueue(readyq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst, 0, 0, 0);
			arrival_time[cur_wait->pcb.idx] = RUN_TIME - run_time;
			service_time[cur_wait->pcb.idx] = 0;
			end_time[cur_wait->pcb.idx] = 0;
		}
		else enqueue(waitq, cur_wait->pcb.idx, cur_wait->pcb.cpu_burst, cur_wait->pcb.io_burst, 0, 0, 0);
	}
	if (set_scheduler == 2) printHeap(readyq, 'r');
	else printQueue(readyq, 'r');
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
		
		switch (set_scheduler) {
		case 1: fp = fopen("results/fcfs_dump.txt", "a+"); break;
		case 2: fp = fopen("results/sjf_dump.txt", "a+"); break;
		case 3: fp = fopen("results/rr_dump.txt", "a+"); break;
		default:
			perror("scheduler");
			exit(EXIT_FAILURE);
		}	
		fprintf(fp, "-----------------------------------------\n");
		fprintf(fp, "Result\n");
		fprintf(fp, "Troughput: %0.3f\n", (float)proc_count / RUN_TIME);
		fprintf(fp, "Average Wait Time: %0.3f\n", wait_time / proc_count);
		fprintf(fp, "Average Turnaround Time: %0.3f\n", turnaround_time / proc_count);		
		fprintf(fp, "-----------------------------------------\n\n");

		for (int i = 0; i < MAX_PROCESS; i++) {
			key[i] = 0x12345 * (i + 1);
			msgctl(msgget(key[i], IPC_CREAT | 0666), IPC_RMID, NULL);
		}

		for (int i = 0; i < MAX_PROCESS; i++) {
			key[i] = 0x80000 * (i + 1);
			msgctl(msgget(key[i], IPC_CREAT | 0666), IPC_RMID, NULL);
		}
		
		for (int i = 0; i < MAX_PROCESS; i++) kill(cpid[i], SIGKILL);
		
		fclose(fp);
		free(cur_wait);
		free(cur_ready);
		removeQueue(waitq);
		virtual_memory_free();
		if (set_scheduler == 2) removeHeap(readyq); 
		else removeQueue(readyq);
		exit(EXIT_SUCCESS);
	}
}

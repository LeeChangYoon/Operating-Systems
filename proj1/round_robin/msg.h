#pragma once


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
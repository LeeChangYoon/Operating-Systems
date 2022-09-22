#pragma once

typedef struct msgbuf {
	long  mtype;

	// pid will sleep for io_time
	int pid;
	int io_time;
	int cpu_time;
} msgbuf;
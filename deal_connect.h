#ifndef _DEAL_CONNECT_H
#define _DEAL_CONNECT_H

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>

struct SWS_worker {
	pid_t pid;	
	int pipefd;
	int status;
};

extern SWS_worker *worker;
extern int SWS_avail_process;

extern void SWS_worker_init();

#endif

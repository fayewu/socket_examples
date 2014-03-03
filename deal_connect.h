#ifndef _DEAL_CONNECT_H
#define _DEAL_CONNECT_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/select.h>

#include "config.h"

struct SWS_worker_t {
	pid_t pid;
	int count;
};

//extern pid_t *SWS_pids;
extern struct SWS_worker_t *SWS_worker; 

extern void SWS_web_start(); 

extern void SWS_wait_worker();

extern void SWS_worker_exit();

#endif

#ifndef _DEAL_CONNECT_H
#define _DEAL_CONNECT_H

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>

extern pid_t pids[SWS_process_num];
extern void SWS_worker_init();

#endif

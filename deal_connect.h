#ifndef _DEAL_CONNECT_H
#define _DEAL_CONNECT_H

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>

#include "config.h"

extern pid_t *SWS_pids;

extern void SWS_worker_init();

#endif

#ifndef _SWS_H
#define _SWS_H

#include <stdio.h>
#include <signal.h>

typedef void (*SWS_handler_t)(int signo); 

struct SWS_sig_t {
	int signo;	
	SWS_handler_t handler;
};

#endif

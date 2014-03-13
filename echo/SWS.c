#include "log.h"
#include "SWS.h"
#include "socket.h"
#include "config.h"
#include "deal_connect.h"

static void SWS_sig_handler(int signo);

struct SWS_sig_t SWS_signals[] = {
	{SIGINT, SWS_sig_handler},
	{SIGCHLD, SWS_sig_handler}
};

sig_atomic_t SWS_sig_quit; 
sig_atomic_t SWS_sig_child;

void
SWS_sig_handler(int signo)
{
	switch (signo) {
		
	case SIGINT:
		SWS_sig_quit = 1;
		break;
	case SIGCHLD:
		SWS_sig_child = 1;
		break;
	}	
}

int
main(int argc, char *argv[])
{
	int i;
	struct sigaction sa;
	
	SWS_log_init();
	SWS_get_config();	

	memset(&sa, 0, sizeof(sa));	
	sa.sa_handler = SWS_sig_handler;

	for (i = 0; i < sizeof(SWS_signals)/sizeof(SWS_signals[0]); i++) {
		if (sigaction(SWS_signals[i].signo, &sa, NULL) < 0) {
			SWS_log_fatal("[%s:%d] sigactio error: %s", __FILE__,
					__LINE__, strerror(errno));	
		}		
	}

	SWS_web_start();

	for ( ;; ) {
		if (SWS_sig_quit) {
			SWS_worker_exit();			
			exit(EXIT_SUCCESS);
		}
		if (SWS_sig_child) {
			SWS_wait_worker();			
		}
	}

	return 0;
}

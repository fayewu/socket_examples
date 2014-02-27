#include <signal.h>

#include "log.h"
#include "socket.h"
#include "config.h"
#include "deal_connect.h"

static void sig_int(int signo);
static void sig_child(int signo);

void
sig_int(int signo){
	int i;

	for (i = 0; i < SWS_process_num; i++) {
		kill(SWS_pids[i], SIGTERM);	
	}

	SWS_log_info("[%s:%d] recive SIGINT", __FILE__, __LINE__);
}

void
sig_child(int signo)
{
	pid_t pid;
	int status;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		SWS_log_info("[%s:%d] child %d end normally", __FILE__,
				__LINE__, pid);	
	}
}

int
main(int argc, char *argv[])
{
	struct sigaction sa_int, sa_child;
	
	SWS_log_init();
	SWS_get_config();	

	/* signal register */
	memset(&sa_int, 0 , sizeof(sa_int));
	memset(&sa_child, 0, sizeof(sa_child));
	sa_int.sa_handler = sig_int;
	sa_child.sa_handler = sig_child;

	if (sigaction(SIGINT, &sa_int, NULL) < 0) {
		SWS_log_fatal("[%s:%d] cannot set SIGINT handler", __FILE__,
				__LINE__);
	}
	if (sigaction(SIGCHLD, &sa_child, NULL) < 0) {
		SWS_log_fatal("[%s:%d] cannot set SIGCHILD handler", __FILE__,
				__LINE__);
	}

	SWS_web_start();

	return 0;
}

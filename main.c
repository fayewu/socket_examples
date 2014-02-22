#include <signal.h>

#include "socket.h"
#include "config.h"
#include "log.h"

static void sig_int(int signo);
static void sig_child(int signo);

void

//void
//sig_int(int signo)
//{
//	SWS_log_info("[%s:%d] stupid web server end normally", __FILE__,
//			__LINE__);
//	SWS_log_end();
//	exit(EXIT_SUCCESS);		
//}
//
//void
//sig_child(int signo)
//{
//	pid_t pid;
//	int status;
//
//	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
//		SWS_log_info("[%s:%d] child %d end normally", __FILE__,
//				__LINE__, pid);	
//	}
//}

int
main(int argc, char *argv[])
{
//	int listen_fd;
	struct sigaction sa_int, sa_child;
	
	SWS_log_init();
	SWS_get_config();	

//	listen_fd = SWS_listen(SWS_port, SWS_addr);

//	/* signal register */
//	memset(&sa_int, 0 , sizeof(sa_int));
//	memset(&sa_child, 0, sizeof(sa_child));
//	sa_int.sa_handler = sig_int;
//	sa_child.sa_handler = sig_child;
//
//	if (sigaction(SIGINT, &sa_int, NULL) < 0) {
//		SWS_log_fatal("[%s:%d] cannot set SIGINT handler", __FILE__,
//				__LINE__);
//	}
//	if (sigaction(SIGCHLD, &sa_child, NULL) < 0) {
//		SWS_log_fatal("[%s:%d] cannot set SIGCHILD handler", __FILE__,
//				__LINE__);
//	}

	SWS_service_start(listen_fd);

	return 0;
}

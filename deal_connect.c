#include "utils.h"
#include "config.h"
#include "socket.h"
#include "deal_connect.h"

struct SWS_worker *worker = NULL;

void
sig_int(int signo){
	int i;

	for (i = 0; i < SWS_process_num; i++) {
		kill(worker[i].pid, SIGTERM);	
	}

	SWS_log_info("[%s:%d] recive SIGINT", __FILE__, __LINE__);
}

void
SWS_web_start()
{
	int listenfd, maxfd, i;
	fd_set rset, masterset;
	struct SWS_worker *worker;

	listenfd = SWS_listen(SWS_port, SWS_addr);
	FD_ZERO(&masterset);
	FD_SET(listenfd, &masterset);
	maxfd = listenfd;

	worker = calloc(SWS_process_num, sizeof(struct SWS_worker));

	for (i = 0; i < SWS_process_num; i++) {
		SWS_worker_init(i, worker, listenfd);
	}
}

pid_t
SWS_worker_init(int i, struct SWS_worker *worker, int listenfd)
{
	int sockfd[2];	
	pid_t pid;

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
		SWS_log_fatal("[%s:%d] worker init error: %s", __FILE__,
				__LINE__, strerror(errno));
		return SWS_ERROR;
	}

	if ((pid = fork()) > 0) {
		close(sockfd[1]);	
		worker[i]->pid = pid;	
		worker[i]->pipefd = sockfd[1];
		worker[i]->status = 0;

		return pid;
	}

	dup2(sockfd[1], STDERR_FILENO);
	close(sockfd[0]);
	close(sockfd[1]);	
	close(listenfd);
	
	SWS_worker_wait_connect();
}

void
SWS_worker_wait_connect()
{
			
}

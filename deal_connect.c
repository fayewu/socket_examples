#include "utils.h"
#include "config.h"
#include "socket.h"
#include "deal_connect.h"

struct SWS_worker *worker = NULL;
int SWS_avail_process = SWS_process_num;

void
SWS_web_start()
{
	socklen_t clilen;
	int listenfd, confd, maxfd, i, retval;
	fd_set rset, masterset;
	struct SWS_worker *worker;
//	struct sockaddr_in cliaddr;

//	clilen = sizeof(cliaddr); 
	listenfd = SWS_listen(SWS_port, SWS_addr);
	FD_ZERO(&masterset);
	FD_SET(listenfd, &masterset);
	maxfd = listenfd;

	worker = calloc(SWS_process_num, sizeof(struct SWS_worker));

	for (i = 0; i < SWS_process_num; i++) {
		SWS_worker_init(i, worker, listenfd);
		FD_SET(worker[i].pipefd, &masterset);
		maxfd = max(listenfd, worker[i].pipefd);
	}

	for ( ;; ) {
		rset = masterset;	
		if (SWS_avail_process == 0) {
			FD_CLR(listenfd, &rset);	
		}
		retval = select(maxfd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {
//			if ((confd = accept(listenfd, (struct sockaddr *)cliaddr, 
//						&clilen)) < 0) {
//				if (errno == EINTR) {
//					continue;
//				}
//				SWS_log_warn("[%s:%d] accept error: %s", __FILE__,
//						__LINE__, strerror(errno));
//				}	
//			}
		}
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

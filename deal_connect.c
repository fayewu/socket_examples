#include "utils.h"
#include "config.h"
#include "socket.h"
#include "deal_connect.h"

pid_t pids[SWS_process_num]; 

void
SWS_web_start()
{
	socklen_t clilen;
	int listenfd, confd, maxfd, i, retval;
	fd_set rset, masterset;
	struct SWS_worker *worker;

	listenfd = SWS_listen(SWS_port, SWS_addr);

	SWS_lock_init();
	for (i = 0; i < SWS_process_num; i++) {
		pids[i] = SWS_worker_init(i, listenfd);	
	}

}

void
SWS_lock_init(char *pathname)
{
	int fd;
	pthread_mut
}

pid_t
SWS_worker_init(int i, int listenfd)
{
	pid_t pid;	

	if ((pid = fork()) > 0) {
		return pid;
	}

	SWS_worker_wait_connect(i, listenfd);
}

void
SWS_worker_wait_connect(int i, int listenfd)
{
	int connfd;
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);

	for ( ;; ) {
		if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, 
						&clien)) < 0) {
			if (errno == EINTR) {
				continue;	
			}
			SWS_log_warn("[%s:%d] accept error: %s", __FILE__,
					__LINE__, strerror(errno));
			}	
		} 

		SWS_web_interation(connfd);
	}

}

//pid_t
//SWS_worker_init(int i, struct SWS_worker *worker, int listenfd)
//{
//	int sockfd[2];	
//	pid_t pid;
//
//	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
//		SWS_log_fatal("[%s:%d] worker init error: %s", __FILE__,
//				__LINE__, strerror(errno));
//		return SWS_ERROR;
//	}
//
//	if ((pid = fork()) > 0) {
//		close(sockfd[1]);	
//		worker[i]->pid = pid;	
//		worker[i]->pipefd = sockfd[1];
//		worker[i]->status = 0;
//
//		return pid;
//	}
//
//	dup2(sockfd[1], STDERR_FILENO);
//	close(sockfd[0]);
//	close(sockfd[1]);	
//	close(listenfd);
//	
//	SWS_worker_wait_connect();
//}
//
//void
//SWS_worker_wait_connect()
//{
//			
//}

//int
//SWS_accept(int listenfd)
//{
//	int connfd, i, n;
//
//	if ((connfd = accept(listenfd, (struct sockaddr *)cliaddr, &clilen)) < 0) {
//		if (errno == EINTR) {
//			continue;
//		}
//		SWS_log_warn("[%s:%d] accept error: %s", __FILE__, __LINE__, 
//				strerror(errno));
//	}	
//
//	for (i = 0; i < SWS_process_num; i++) {
//		if (worker[i].status == 0) {
//			break;
//		}	
//	}
//	worker[i].status = 1;
//	SWS_avail_process--;
//
//	n = SWS_write_fd(worker[i].pipefd, "", 1, connfd);
//}
//
//int
//SWS_write_fd(int pipefd, void *ptr, size_t nbytes, int sendfd)
//{
//	struct msghdr msg;	
//	struct iovec iov[1];
//	union {
//		struct cmsghdr cm;
//		char control[CMSG_SPACE(sizeof(int))];
//	} control_un;
//	struct cmsghdr *cmptr;
//
//	msg.msg_control = control_un.control;
//	msg.msg_control = sizeof(control_un.control);
//
//	cmptr = CMSG_FIRSTHDR(&msg);
//	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
//	cmptr->cmsg_level = SOL_SOCKET;
//	cmptr->cmsg_type = SCM_RIGHTS;
//	*(int *))
//	
//}

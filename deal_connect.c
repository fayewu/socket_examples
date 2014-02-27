#include "utils.h"
#include "config.h"
#include "socket.h"
#include "deal_connect.h"

pid_t *SWS_pids; 
static pthread_mutex_t *mptr;

static void SWS_lock_init(char *pathname);
static pid_t SWS_worker_init(int i, int listenfd);
static void SWS_lock_wait();
static void SWS_worker_wait_connect(int i, int listenfd);

void
SWS_web_start()
{
	socklen_t clilen;
	int listenfd, confd, maxfd, i, retval;
	fd_set rset, masterset;
	struct SWS_worker *worker;

	listenfd = SWS_listen(SWS_port, SWS_addr);
	SWS_pids = (pid_t *)malloc(sizeof(pid_t) * SWS_process_num);	

	SWS_lock_init();
	for (i = 0; i < SWS_process_num; i++) {
		SWS_pids[i] = SWS_worker_init(i, listenfd);	

	}

}

void
SWS_lock_init(char *pathname)
{
	int fd;
	pthread_mutexattr_t mattr;

	if (fd = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROX_WRITE,
			MAP_SHARED, fd, 0) < 0) {
		SWS_log_fatal("[%s:%d] mmap error: %s", __FILE__, __LINE__,
				strerror(errno));	
	}
	close(fd);

	if (pthread_mutexattr_init(&mattr) != 0) {
		SWS_log_fatal("[%s:%d] mutexattr init error: %s", __FILE__,
				__LINE__, strerror(errno));		
	}
	if (pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED) != 0) {
		SWS_log_fatal("[%s:%d] mutexattr setshared init error: %s", 
				__FILE__, __LINE__, strerror(errno));		
	}
	if (pthread_mutex_init(mptr, &mattr) != 0) {
		SWS_log_fatal("[%s:%d] mutex init error: %s", __FILE__,
				__LINE__, strerror(errno));		
	}
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
SWS_lock_wait()
{
	if (pthread_mutex_lock(mptr) != 0) {
		SWS_log_fatal("[%s:%d] mutex lock error: %s", __FILE__,
				__LINE__, strerror(errno));		
	}	
}

void
SWS_worker_wait_connect(int i, int listenfd)
{
	int connfd;
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);

	for ( ;; ) {
		SWS_lock_wait();
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

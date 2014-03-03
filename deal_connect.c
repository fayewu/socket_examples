#include "log.h"
#include "utils.h"
#include "config.h"
#include "socket.h"
#include "interaction.h"
#include "deal_connect.h"

//pid_t *SWS_pids; 
struct SWS_worker_t *SWS_worker;
static pthread_mutex_t *mptr;

static void SWS_lock_init();
static pid_t SWS_worker_init(int i, int listenfd);
static void SWS_lock_wait();
static int SWS_worker_wait_connect(int i, int listenfd);

void
SWS_web_start()
{
	int listenfd, i;

	listenfd = SWS_listen(SWS_port, SWS_addr);
	SWS_worker = (struct SWS_worker_t *)malloc(sizeof(struct SWS_worker_t) 
			* SWS_process_num);	

	SWS_lock_init();
	for (i = 0; i < SWS_process_num; i++) {	
		SWS_worker[i].pid = SWS_worker_init(i, listenfd);
		if (i == 0) {
			close(listenfd);
		}
	}
}

void
SWS_lock_init()
{
	int fd;
	pthread_mutexattr_t mattr;
	
	if ((fd = open("/dev/zero", O_RDWR, 0)) < 0) {
		SWS_log_fatal("[%s:%d] open error: %s", __FILE__, __LINE__,
				strerror(errno));	
	}

	if ((mptr = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0)) == MAP_FAILED) {
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

	SWS_worker[i].count = 0;
	return SWS_worker_wait_connect(i, listenfd);
}

void
SWS_lock_wait()
{
	if (pthread_mutex_lock(mptr) != 0) {
		SWS_log_fatal("[%s:%d] mutex lock error: %s", __FILE__,
				__LINE__, strerror(errno));		
	}	
}

int
SWS_worker_wait_connect(int i, int listenfd)
{
	int connfd;
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);

	for ( ;; ) {
		SWS_lock_wait();
		if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, 
						&clilen)) < 0) {
			if (errno == EINTR) {
				continue;	
			}
			SWS_log_warn("[%s:%d] accept error: %s", __FILE__,
					__LINE__, strerror(errno));
		} 
		SWS_worker[i].count++;
		SWS_echo_interation();
//		SWS_web_interation(connfd);
		close(connfd);
	}

	return 0;
}

void
SWS_wait_worker()
{
	pid_t pid;		
	int status;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		SWS_log_info("[%s:%d] child %d end normally", __FILE__,
				__LINE__, pid);
	}
}

void
SWS_worker_exit()
{
	int i;

	for (i = 0; i < sizeof(SWS_worker)/sizeof(SWS_worker[0]); i++) {
		kill(SWS_worker[i].pid, SIGTERM);					
	}	

}

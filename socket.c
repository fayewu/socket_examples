#include "log.h"
#include "socket.h"
#include "interaction.h"

static int SWS_rwtime = 10;
static int SWS_timeout(int fd, int sec, int rwflag);

int
SWS_listen(const int port, const char *address)
{
	char *ptr;
	int optlen = 1;
	int keep_idle = 5, keep_interval = 1, keep_count = 3; 
	int listen_fd, backlog;
	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if (inet_aton(address, &servaddr.sin_addr)) {
		SWS_log_fatal("[%s:%d] inet_aton error: %s", 
				__FILE__, __LINE__, strerror(errno));
	}

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		SWS_log_fatal("[%s:%d] socket error: %s", 
				__FILE__, __LINE__, strerror(errno));	
	}
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optlen, 
				sizeof(optlen)) < 0) {
		SWS_log_warn("[%s:%d] set reuse addr error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
	if (setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &optlen,
				sizeof(optlen)) < 0) {
		SWS_log_warn("[%s:%d] set keepalive error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
	if (setsockopt(listen_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keep_idle,
				sizeof(keep_idle)) < 0) {
		SWS_log_warn("[%s:%d] set tcp keepidle error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
	if (setsockopt(listen_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keep_interval,
				sizeof(keep_interval)) < 0) {
		SWS_log_warn("[%s:%d] set tcp keepidle error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
	if (setsockopt(listen_fd, IPPROTO_TCP, TCP_KEEPCNT, &keep_count,
				sizeof(keep_count)) < 0) {
		SWS_log_warn("[%s:%d] set tcp keepcnt error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}

	if (bind(listen_fd, (struct sockaddr *)&servaddr,
				sizeof(servaddr)) != 0) {
		SWS_log_fatal("[%s:%d] %s", __FILE__, __LINE__,
				strerror(errno));	
	}

	if ((ptr = getenv("LISTENQ")) != NULL) {
		sscanf(ptr, "%d", &backlog);
	}
	if (listen(listen_fd, backlog) < 0) {
		SWS_log_fatal("[%s:%d] %s", __FILE__, __LINE__,
				strerror(errno));
	}

	return listen_fd;	
}

//void
//SWS_service_start()
//{
//	pid_t pid;
//	int connect_fd, listen_fd;
//	socklen_t clilen; 
//	struct sockaddr_in cliaddr;
//
//	listen_fd = SWS_listen(SWS_port, SWS_addr);
//
//	while (1) {
//		clilen = sizeof(cliaddr);	
//		if ((connect_fd = accept(listen_fd, (struct sockaddr *)&cliaddr,
//				&clilen)) < 0) {
//			if (errno == EINTR) {
//				continue;
//			}
//			SWS_log_warn("[%s:%d] accept error: %s", __FILE__,
//					__LINE__, strerror(errno));
//		}	
//
//		if ((pid = fork()) < 0) {
//			SWS_log_fatal("[%s:%d] fork error : %s", __FILE__,
//					__LINE__, strerror(errno));
//		} else if (pid > 0) {
//			close(connect_fd);
//		} else {
//			close(listen_fd);
//			SWS_web_interation(connect_fd);
//		}
//	}	
//}

/* ssize_t和size_t 是根据read的参数，可移植性*/
ssize_t
SWS_read_request_line(int fd, void *vptr, size_t maxlen)
{
	char c, *ptr = vptr;
	ssize_t n, rc;	

	for (n = 1; n < maxlen; n++) {
	again:
		if ((rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			if (c == '\n') {
				break;
			}
		} else if (rc == 0) {
			*ptr = 0;
			return (n - 1);
		} else {
			if (errno == EINTR) {
				goto again;
			}
		}
	}	

	*ptr = 0;
	return n;
}

//ssize_t
//SWS_read_header(int fd, void *vptr, size_t maxlen)
//{
//	int n, len;
//	char *ptr = vptr;
//	
//	if (SWS_timeout()) {
//		
//	}
//	n = read(fd, ptr, maxlen);
//
//	if (n <= 0) {
//		return n;
//	} else {
//		len = strlen(ptr) - 4;	
//		if (strcmp(&ptr[len], "\r\n\r\n")) {
//			return SWS_UNFINISHED;
//		}
//		return n;
//	}
//}

//void
//SWS_timeout(int fd)
//{
//	int ret;
//	fd_set set;
//	struct timeval tv;
//
//	FD_ZERO(&set);
//	FD_SET(fd, &set);
//
//	tv.tv_sec = SWS_rwtime;
//	tv.tv_uset = 0;
//
//	return select(fd + 1, &set, NULL, NULL, &tv);
//}
//

int
SWS_timeout(int fd, int sec, int rwflag)
{
	fd_set rset;	
	struct timeval tv;

	FD_ZERO(&rset);
	FD_SET(fd, &rset);

	tv.tv_sec = sec;
	tv.tv_uset = 0;

	return select(fd + 1, &rset, NULL, NULL, &tv);
}

ssize_t
SWS_read(int fd, void *vptr, size_t maxlen)
{
	int n, ret;
	char *sptr;

	while (True) {
		ret = SWS_timeout(fd, SWS_rwtime, SWS_READ_TIMEO);	

		if (ret < 0) {
			if (errno == EINTR) {
				continue;	
			}
		}

		if (ret == 0) {
			return SWS_TIMEOUT;	
		}

		break;
	}

again:	
	n = read(fd, vptr, maxlen);

	if ( n <= 0) {
		return n;
	} else {
		sptr = vptr;
		if (*(sptr + n - 1) != '\n') {
			vptr += n;
			goto again;
		}			
		return n;
	}
}

ssize_t
SWS_write(int fd, void *vptr, size_t maxlen)
{
	return write(fd, vptr, maxlen);	
}

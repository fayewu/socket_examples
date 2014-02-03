#include "socket.h"
#include "log.h"

int
SWS_listen(const int port, const char *address)
{
	int listen_fd;
	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if (inet_aton(address, &servaddr.sin_addr)) {
		SWS_log_fatal("[%s:%d] %s", __FILE__, __LINE__, 
				strerror(errno));
	}

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		SWS_log_fatal("[%s:%d] %s", __FILE__, __LINE__, 
				strerror(errno));	
	}

	if (bind(listen_fd, (struct sockaddr *)&servaddr,
				sizeof(servaddr)) != 0) {
		SWS_log_fatal("[%s:%d] %s", __FILE__, __LINE__,
				strerror(errno));	
	}

	if (listen(listen_fd, 10000) < 0) {
		SWS_log_fatal("[%s:%d] %s", __FILE__, __LINE__,
				strerror(errno));
	}

	return listen_fd;	
}

void
SWS_service_start(int listen_fd)
{
	pid_t pid;
	int connect_fd;
	socklen_t clilen; 
	struct sockaddr_in cliaddr;

	while (1) {
		clilen = sizeof(cliaddr);	
		if ((connect_fd = accept(listen_fd, (struct sockaddr *)&cliaddr,
				&clilen)) < 0) {
			if (errno == EINTR) {
				continue;
			}
			SWS_log_warn("[%s:%d] accept error: %s", __FILE__,
					__LINE__, strerror(errno));
		}	

		if ((pid = fork()) < 0) {
			SWS_log_fatal("[%s:%d] fork error : %s", __FILE__,
					__LINE__, strerror(errno));
		} else if (pid > 0) {
			close(connect_fd);
		} else {
			close(listen_fd);
//			SWS_web_interation(connect_fd);
				
		}
	}	
}

/* ssize_t和size_t 是根据read的参数，可移植性*/
ssize_t
SWS_read_request_line(int fd, void *ptr, size_t maxlen)
{
	char byte;
	ssize_t n, rc;	
	int end_flag = 0;

	for (n = 0; n < maxlen; n++) {
		if (rc= read(fd, &c, 1) < 0) {
			if (errno == EINTR) {
				n--;
				continue;
			}
			return -1;
		} else if (rc == 1){
			*ptr++ = c;	
			if (c == '\r') {
				end_flag = 1;	
			}
			if (end_flag == 1 && c!= '\n') {
				end_flag = 0;
			}
			if (c == '\n' && end_flag = 1) {
				break;	
			}
		} else {
			*ptr = 0;		
			return (n - 1);
		}
	}

	*ptr = 0;
	return n;
}

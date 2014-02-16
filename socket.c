#include "log.h"
#include "socket.h"
#include "interaction.h"

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
			SWS_web_interation(connect_fd);
		}
	}	
}

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

ssize_t
SWS_read_header(int fd, void *vptr, size_t maxlen)
{
	int n, len;
	char *ptr = vptr;
	
	n = read(fd, ptr, maxlen);

	if (n <= 0) {
		return n;
	} else {
		len = strlen(ptr) - 4;	
		if (strcmp(&ptr[len], "\r\n\r\n")) {
			return SWS_AGAIN;
		}
		return n;
	}
}

ssize_t
SWS_read_content(int fd, void *vptr, size_t maxlen)
{
	int n;

	n = read(fd, vptr, maxlen);
	if (n <= 0) {
		return n;
	} else {
		if (n < maxlen) {
			return SWS_AGAIN;
		}
		return n;
	}
}

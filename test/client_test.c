#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#define SWS_CLIENT_NUM		4
#define SWS_CONNECT_NUM		30

char line[1024] = {"hello\n"};

int
tcp_connect()
{
	int connect_fd;
	struct sockaddr_in cliaddr;

	memset(&cliaddr, 0, sizeof(cliaddr));

	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(5000);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

	if ((connect_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error\n");	
		exit(EXIT_FAILURE);
	}

	if (connect(connect_fd, (struct sockaddr *)&cliaddr, 
				sizeof(cliaddr)) < 0) {
		printf("connect error\n");
		exit(EXIT_FAILURE);
	}

	return connect_fd;
}

void
SWS_write(int fd, void *ptr, size_t maxlen)
{
again:	if (write(fd, ptr, maxlen) < 0) {
		if (errno == EINTR) {
			goto again;
		}
		printf("[%s:%d] write error\n", __FILE__, __LINE__);
	}	
}

void
SWS_read(int fd, void *ptr, size_t maxlen)
{
	int n;

again:	n = read(fd, ptr, maxlen);
	if (n == 0) {
		printf("[%s:%d] client terminated prematurely",
				__FILE__, __LINE__);		
	} else if (n < 0) {
		if (errno == EINTR) {
			goto again;
		}
		printf("[%s:%d] read error: %s", __FILE__, 
				__LINE__, strerror(errno));		
	}
}

int
main(int argc, char *argv[])
{
	int i, j, fd;
	pid_t pid;	

	for (i = 0; i < SWS_CLIENT_NUM; i++) {
		if ((pid = fork()) < 0) {
			printf("[%s:%d] fork error", __FILE__, __LINE__);	
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			for (j = 0; j < SWS_CONNECT_NUM; j++) {
				fd = tcp_connect();		
				
				SWS_write(fd, line, strlen(line));	
				close(fd);
			}	
			exit(EXIT_SUCCESS);
		}
	}

	while (wait(NULL) > 0) {
			
	}
	if (errno != ECHILD) {
		printf("[%s:%d] wait error", __FILE__, __LINE__);	
	}

	return 0;
}

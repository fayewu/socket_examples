#ifndef _SOCKET_H
#define _SOCKET_H

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

#define SWS_OVERTIME		-3
#define SWS_UNFINISHED		-2
#define SWS_DELAY_TIME		60	

extern int SWS_listen(const int port, const char *address);
extern void SWS_service_start();
extern ssize_t SWS_read_request_line(int fd, void *ptr, size_t maxlen);
extern ssize_t SWS_read_header(int fd, void *ptr, size_t maxlen);
extern ssize_t SWS_read_content(int fd, void *ptr, size_t maxlen);

#endif

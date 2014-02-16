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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SWS_AGAIN		-2
#define SWS_

extern int SWS_listen(const int port, const char *address);
extern void SWS_service_start(int listen_fd);
extern ssize_t SWS_read_request_line(int fd, void *ptr, size_t maxlen);
extern ssize_t SWS_read_header(int fd, void *ptr, size_t maxlen);
extern ssize_t SWS_read_content(int fd, void *ptr, size_t maxlen);

#endif

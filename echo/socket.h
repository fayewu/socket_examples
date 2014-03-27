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

#include "interaction.h"

#define SWS_READ_TIMEO		0
#define SWS_WRITE_TIMEO		1
#define SWS_TIMEOUT		-2

#define SWS_RWTIME		20

#define	SWS_READED		-1
#define SWS_CLOSE		0
#define SWS_WRITED		-1
#define SWS_AGAIN		-2

extern int SWS_listen(const int port, const char *address);
extern void SWS_service_start();

extern int SWS_read(int fd, struct SWS_buf_t **buf); 
extern int SWS_write(int fd, struct SWS_buf_t **buf);

//extern ssize_t SWS_read_request_line(int fd, void *ptr, size_t maxlen);
//extern ssize_t SWS_read_header(int fd, void *ptr, size_t maxlen);
//extern ssize_t SWS_read_content(int fd, void *ptr, size_t maxlen);

#endif

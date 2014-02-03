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

extern int SWS_listen(const int port, const char *address);
extern void SWS_service_start(int listen_fd);
extern void SWS_read(int connect_fd);

#endif

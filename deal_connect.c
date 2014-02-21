#include "utils.h"
#include "config.h"
#include "socket.h"
#include "deal_connect.h"

void
SWS_web_start()
{
	int listenfd, maxfd, i;
	fd_set rset, masterset;

	listenfd = SWS_listen(SWS_port, SWS_addr);
	FD_ZERO(&masterset);
	FD_SET(listenfd, &masterset);
	maxfd = listenfd;

	for (i = 0; i < SWS_process_num; i++) {
		SWS_worker_init();
	}
}

pid_t
SWS_worker_init()
{
	
}

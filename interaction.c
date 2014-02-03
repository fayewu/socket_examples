#include "socket.h"
#include "interaction.h"

static struct SWS_request_t {
	int method;	
	char *url;
	int accept_type;
}; 

void
SWS_web_interation(int connect_fd)
{
	int n;
	char recvline[SWS_request_header] = {0}; 

	/* deal with http requst header */
	if ((n = SWS_read_request_line(connect_fd, recvline, 
				SWS_read_request_line)) >= 0) {
		if (n == 0) {
			
		}
	}

//	while ((n = SWS_read_request_line(connect_fd, recvline, 
//					SWS_request_header)) > = 0) {
//		if (n == 0) {
//			SWS_log_info("[%s:%d] child end early", __FILE__,
//					__LINE__);
//			exit(EXIT_SUCCESS);
//		}	
//		if ((n == 2) && (!strcmp(recvline, "\r\n"))) {
//			break;			
//		}
//
//		if
//	}
} 		

int
SWS_parse_request_header_line()
{
	if 			
}

#include "log.h"
#include "utils.h"
#include "socket.h"
#include "interaction.h"

void
SWS_connect_init(struct SWS_request_t **request, struct SWS_connect_t *connect)
{
	struct SWS_request_t *req = *request;
	struct SWS_request_t *con = *connect;

	/* TODO memory_poll */
	req = (struct SWS_request_t *)malloc(struct SWS_request_t);
	con = (struct SWS_connect_t *)malloc(struct SWS_request_t);

	/* 1k magic number*/
	memset(req, 0, sizeof(req));
	con->buffer = (char *)malloc();
	con->buf_loc = 0;

	/* 挂载读和解析事件 */
	con->recv = SWS_read_request_line;
	con->parse = SWS_parse_request_header_line;
}

void
SWS_web_interation(int connect_fd)
{
	int n;
	struct SWS_request_t **req;
	struct SWS_connect_t **con;

	SWS_connect_init(req, con);

	for ( ;; )  {
		n = con->recv(connect_fd, &con->buffer[loc], SWS_HEADER_LEN);
	
		if (n == 0) {
			SWS_log_info("[%s:%d] client terminated prematurely",
					__FILE__, __LINE__);		
			exit(EXIT_SUCCESS);
		}

		if (n < 0 && errno == EINTR) {
			continue;
		}

		/* update buffer's used size */
		loc += n;
	}
//		n = SWS_read_request_line(connect_fd, con->buffer, 
//				SWS_request_header);		
//		if (n == 0) {
//			SWS_log_info("[%s:%d] client terminated prematurely",
//					__FILE__, __LINE__);		
//			exit(EXIT_SUCCESS);
//		}
//		if (n < 0 && errno == EINTR) {
//			continue;	
//		}
//
//		SWS_parse_request_header_line(con->buffer, req);
//
//		memset(con->buffer, 0, sizeof(con->buffer));
//		while(True) {
//			n = read(connect_fd, con->buffer, SWS_HEADER_LEN);	
//
//			if (n == 0) {
//				SWS_log_info("[%s:%d] client terminated prematurely",
//					__FILE__, __LINE__);		
//				exit(EXIT_SUCCESS);
//			}
//		}
} 		

int
SWS_parse_request_header_line(const char *header, struct SWS_request_t **request)
{
	struct SWS_request_t *req = *request;	

	sprintf(header, "%s %s %s\r\n", req->method, req->url, req->version);

	if (!strcmp(req->method, "POST") || !strcmp(req->method, "PUT")) {
		req->is_content = True;										
	}	

	/* 改变读和解析事件 */
}

int
SWS_parse_request_header();

#include "log.h"
#include "utils.h"
#include "socket.h"
#include "interaction.h"

static void SWS_connect_init(struct SWS_request_t **request, 
		struct SWS_connect_t **connect);
static void SWS_parse_request_header_line(char *header, 
	  	struct SWS_request_t **request, struct SWS_connect_t **connect);
static void SWS_parse_request_header(char *header, 
		struct SWS_request_t **request, struct SWS_connect_t **connect);
static void SWS_parse_content(char *header, struct SWS_request_t **request,
		struct SWS_connect_t **connect);


void
SWS_connect_init(struct SWS_request_t **req, struct SWS_connect_t **con)
{

	/* TODO memory_poll */
	(*req) = (struct SWS_request_t *)malloc(sizeof(struct SWS_request_t));
	(*con) = (struct SWS_connect_t *)malloc(sizeof(struct SWS_request_t));

	/* 1k magic number*/
	memset(*req, 0, sizeof(*req));
	(*con)->buffer = (char *)malloc(SWS_HEADER_LEN);
	(*con)->buf_loc = 0;

	/* 挂载读和解析事件 */
	(*con)->recv = SWS_read_request_line;
	(*con)->parse = SWS_parse_request_header_line;
}

void
SWS_web_interation(int connect_fd)
{
	int n;
	struct SWS_request_t *req;
	struct SWS_connect_t *con; 

	SWS_connect_init(&req, &con);

	for ( ;; )  {
//		memset(con->buffer, 0, SWS_HEADER_LEN);
//		n = SWS_read_request_line(connect_fd, &con->buffer[con->buf_loc],
//				SWS_HEADER_LEN);	
		n = con->recv(connect_fd, &con->buffer[con->buf_loc], 
				SWS_HEADER_LEN);
	
		if (n == 0) {
			SWS_log_info("[%s:%d] client terminated prematurely",
					__FILE__, __LINE__);		
			exit(EXIT_SUCCESS);
		}

		if (n < 0 && errno == EINTR) {
			continue;
		}

		/* update buffer's used size */
		con->buf_loc += n;
		if (n == SWS_AGAIN) {
			continue;			
		}

		con->parse(con->buffer, &req, &con);

	}
} 		

void
SWS_parse_request_header_line(char *header, struct SWS_request_t **request,
		struct SWS_connect_t **connect)
{
	struct SWS_request_t *req = *request;	
	struct SWS_connect_t *con = *connect;

	sscanf(header, "%s %s %s\r\n", req->method, req->url, req->version);
	printf("%s %s %s\n", req->method, req->url, req->version);

	if (!strcmp(req->method, "POST") || !strcmp(req->method, "PUT")) {
		req->is_content = True;										
	}	

	/* 改变读和解析事件 */
	con->recv = SWS_read_header;
	con->parse = SWS_parse_request_header;
}

void
SWS_parse_request_header(char *header, struct SWS_request_t **request,
		struct SWS_connect_t **connect)
{
	struct SWS_request_t *req = *request;	
	struct SWS_connect_t *con = *connect;
	
	if (req->is_content) {
		con->recv = SWS_read_content;
		con->parse = SWS_parse_content;
	} else {
		memset(con->buffer, 0, sizeof(SWS_HEADER_LEN));
		con->recv = SWS_read_request_line;
		con->parse = SWS_parse_request_header_line;
	}
}

void
SWS_parse_content(char *header, struct SWS_request_t **request,
		struct SWS_connect_t **connect)
{
	
}

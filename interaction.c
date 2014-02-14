#include "log.h"
#include "utils.h"
#include "socket.h"
#include "interaction.h"

static SWS_field_pt all_field[]  = {
	{"Host", SWS_parse_host},
	{"Connection", SWS_parse_connection}//,
//	{"Accept", SWS_parse_accept},
//	{"If", SWS_parse_if},
//	{"Authorization", SWS_parse_authorization},
//	{"From", SWS_parse_from},
//	{"Expect", SWS_parse_expect},
//	{"Max-Forwards", SWS_parse_max},
//	{"Proxy-Authorization", SWS_parse_proxy},
//	{"Range", SWS_parse_range},
//	{"Referer", SWS_parse_Referer}
//	{"TE", SWS_parse_TE}
} 


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
SWS_parse_request_header_line(char *header, struct SWS_request_t **req,
		struct SWS_connect_t **con)
{
	struct SWS_request_t *r = *req;	
	struct SWS_connect_t *c = *con;

	sscanf(header, "%s %s %s\r\n", r->method, r->url, r->version);

	if (!strcmp(r->method, "POST") || !strcmp(r->method, "PUT")) {
		r->is_content = True;										
	}	

	/* 改变读和解析事件 */
	c->recv = SWS_read_header;
	c->parse = SWS_parse_request_header;
}

void
SWS_parse_request_header(char *header, struct SWS_request_t **req,
		struct SWS_connect_t **con)
{
	char *line;
	struct SWS_request_t *r = *req;	
	struct SWS_connect_t *c = *con;
	
	line = strtok(header, CRLF);
	for ( ;; ) {
		if ((line = strtok(NULL, CRLF)) == NULL) {
			break;	
		}
		SWS_parse_header_line(line);	
	}		

	if (r->is_content) {
		c->recv = SWS_read_content;
		c->parse = SWS_parse_content;
	} else {
		memset(c->buffer, 0, sizeof(SWS_HEADER_LEN));
		c->recv = SWS_read_request_line;
		c->parse = SWS_parse_request_header_line;
	}
}

void
SWS_parse_header_line(char *line)
{
	sscanf(line, "%s: %s", name, content);

	if (strcmp(name, "Host"))
		
}

void
SWS_parse_content(char *header, struct SWS_request_t **request,
		struct SWS_connect_t **connect)
{
	
}

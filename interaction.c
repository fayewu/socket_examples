#include "log.h"
#include "utils.h"
#include "socket.h"
#include "interaction.h"

static SWS_field_pt SWS_all_field[]  = {
	{"Host", SWS_parse_host},
	{"Connection", SWS_parse_connection},
	{"Content-Length", SWS_parse_content_length}//,
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
}; 

char SWS_info_st[][SWS_STATUS_LEN] = {
	"Continue",
	"Switching Protocols"
};

char SWS_success_st[][SWS_STATUS_LEN] = {
	"OK",
	"Created",
	"Accepted",
	"Non-Authoritative information",
	"No Content",
	"Reset Content",
	"Partial Content"
};

char SWS_redirect_st[][SWS_HEADER_LEN] = {
	"Multiple Choices",
	"Moved Permanently",
	"Found",
	"See Other",
	"Not Modified",
	"Use Proxy",
	"Unused",
	"Temporary Redirect"
};

char SWS_clierror_st[][SWS_HEADER_LEN] = {
	"Bad Request",
	"Unauthorized",
	"Payment Required",
	"Forbidden",
	"Not Found",
	"Method Not Allowed",
	"Not Acceptable",
	"Proxy Authentication Required",
	"Request Timeout",
	"Confilict",
	"Gone",
	"Length Required",
	"Precondition Failed",
	"Request Entity Too Large",
	"Request-URI Too Long",
	"Unsupported Media Type",
	"Requested Range Not Satisfiable",
	"Expectation Failed"
};

char SWS_sererror_st[][SWS_HEADER_LEN] = {
	"Internal Server Error",
	"Not Implemented",
	"Bad Gateway",
	"Service Unavailable",
	"Gateway Timeout",
	"HTTP version Not Supported"
};


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
	memset(*con, 0, sizeof(*con));
	(*con)->buffer = malloc(SWS_HEADER_LEN);
	(*con)->buf_loc = 0;
	(*con)->read_len = SWS_HEADER_LEN;

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
				con->read_len);
	
		if (n == 0) {
			SWS_log_info("[%s:%d] client terminated prematurely",
					__FILE__, __LINE__);		
			exit(EXIT_SUCCESS);
		}
	
		if (n == -1) {
			if (errno = EINTR) {
				continue;
			}
			SWS_log_warn("[%s:%d] read error: %s", __FILE__,
						__LINE__);
			return;
		}

		/* update buffer's used size */
		con->buf_loc += n;
		if (n == SWS_AGAIN) {
			continue;			
		}

		con->parse(con->buffer, con->buf_loc, &req, &con);

	}
} 		

int
SWS_parse_request_header_line(void *header, int len, struct SWS_request_t **req,
		struct SWS_connect_t **con)
{
	enum line { 
		method = 1,
		url,
		version
	};
	void *start = header, *end;
	int status = 0, ret;
	
	for (end = header; end != header + len; end++) {
		if (end == "_" || end == '\r') {
			status += 1;	
		}	
		
		switch (status) {
		
		case method:	
			if ((ret = SWS_parse_method(start, end - start, 
							req)) < 0) {
				return ret;
			}			
			start = end + 1;
			break;						

		case url:
			if ((ret = SWS_parse_url(start, end - start,
							req)) < 0) {
				return ret;	
			}
			start = end + 1;
			break;						
		
		case version:
			if (end - start != SWS_HTTP_VERSION_LEN) {
				return 400;
			}

			// TODO:版本局限1.1 
			if (SWS_str_cmp8(start, "HTTP/1.1")) {
				(*req)->version = SWS_HTTP_1_1; 
			} else {
				return 505;
			}
			break;
		}
	}

	/* 判断停留在什么status */

	// TODO parse url

	/* 改变读和解析事件 */
	memset(c->buffer, 0, sizeof(buffer));
	c->buf_loc = 0;
	c->recv = SWS_read_header;
	c->parse = SWS_parse_request_header;

	return SWS_OK;
}

int
SWS_parse_request_header(void *header, int hlen, struct SWS_request_t **req,
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
		SWS_parse_header_line(line, req);	
	}		

	if (r->is_content) {
		c->read_len = r->content_len; 
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

int
SWS_parse_header_line(char *line, struct SWS_request_t **req)
{
	int i, nlen, clen;
	char *name, *content = line;

	for ( ; *content != ':'; *content++ ); 
	name = line;
	nlen = content - name;

	/* 越过:和空格 */
	content += 2;
	clen = &line[strlen(line) - 1] - content + 1;

	for (i = 0; i < sizeof(SWS_all_field) / sizeof (SWS_all_field[0]); i++) {
		if (!strcmp(name, SWS_all_field[i].name)) {
			SWS_all_field[i].parse(content, clen, req);
				break;
		}		
	}		
	/* TODO 自定义域 */
}

int
SWS_parse_host(char *content, int clen, struct SWS_request_t **req)
{
	struct stat st;
	
	/* not absolute url */
	if ((*req)->url[0] != '/') {
		return;		
	}	
	(*req)->host = (char *)malloc(strlen(content) + 1);	

	return SWS_ERROR;
}

int
SWS_parse_connection(char *content, int clen, struct SWS_request_t **req) 
{
	if (!strcmp(content, "close")) {
		(*req)->connection = SWS_CLOSE;	
	} else if (!strcmp(content, "keep-alive")) {
		(*req)->content = SWS_ALIVE;
	} else {
		return 400;
	}

	return SWS_OK;
}

int
SWS_parse_content_length(char *content, int clen, struct SWS_request_t **req)
{
	int len; 

	if (strlen(content) > 4) {
		return 413;
	}
	while (*content != '\0') {
		if (*content > '9' || *content < '0') {
			return 400;	
		} 
	}
	sscanf(content, "%d", len);
	if (len > SWS_request_body) {
		return 413;	
	}

	(*req)->content_len = len;
	return SWS_OK;
}	

int
SWS_parse_content(void *header, int hlen, struct SWS_request_t **req,
		struct SWS_request_t **con)
{
	return 0;
}

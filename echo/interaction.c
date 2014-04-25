#include "log.h"
#include "utils.h"
#include "socket.h"
#include "interaction.h"

int SWS_end_flag = 0;
static struct SWS_buf_t *SWS_buf; 

void
SWS_echo_interation(int connfd)
{	
	struct SWS_buf_t tmp;
	struct epoll_event *events, ev;
	int i, val, epollfd, nfds, ret;

	SWS_buf = &tmp;

	memset(SWS_buf->buf, 0, SWS_BUF_LEN);
	SWS_buf->start = SWS_buf->buf;
	SWS_buf->end = SWS_buf->buf;

	if ((val = fcntl(connfd, F_GETFL, 0)) < 0) {
		SWS_log_error("[%s:%d] fcntl error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
	
	if (fcntl(connfd, F_SETFL, val | O_NONBLOCK) < 0) {
		SWS_log_error("[%s:%d] fcntl error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
		
	epollfd = epoll_create(10);
	if (epollfd < 0) {
		SWS_log_error("[%s:%d] epoll create error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}
	ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	ev.data.fd = connfd;  

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0) {
		SWS_log_error("[%s:%d] epoll ctl error: %s", __FILE__,
				__LINE__, strerror(errno));	
	}

	events = malloc(SWS_MAXEVENTS * sizeof (struct epoll_event));

	for ( ;; ) {
		nfds = epoll_wait(epollfd, events, SWS_MAXEVENTS, -1);
		if (nfds == -1) {
			SWS_log_error("[%s:%d] epoll wait error: %s", __FILE__,
					__LINE__, strerror(errno));	
		}

		for (i = 0; i < nfds; i++) {
			if (events[i].events & EPOLLIN) {
				if (SWS_read(connfd, &SWS_buf) == SWS_CLOSE) {
					return;
				}
				
				ret = SWS_write(connfd, &SWS_buf);
				if (ret == EAGAIN) {
					ev.events = SWS_WRITE_EVENT;	
					epoll_ctl(epollfd, EPOLL_CTL_MOD, connfd, &ev);
				}
				if (ret != EAGAIN && ret != SWS_WRITED) {
					return;	
				}
			}	

			if (events[i].events & EPOLLRDHUP) {
				return;
			}

			if (events[i].events & EPOLLOUT) {
				ret = SWS_write(connfd, &SWS_buf);
				if (ret == SWS_WRITED) {
					ev.events = SWS_READ_EVENT; 
					epoll_ctl(epollfd, EPOLL_CTL_MOD, connfd, &ev);
				}
			}
		}

	}
}

//	for ( ;; ) {
//		FD_ZERO(&wset);
//		FD_ZERO(&rset);
//		if (end_flag == 0 && 
//				&SWS_buf->buf[SWS_BUF_LEN] - SWS_buf->end > 0) {
//			FD_SET(connfd, &rset);
//		}
//		if (SWS_buf->start != SWS_buf->end) {
//			FD_SET(connfd, &wset);
//		}
//
//		if ((ret = select(connfd + 1, &rset, &wset, NULL, NULL)) < 0) {
//			if (errno == EINTR) {
//				continue;
//			}
//			SWS_log_error("[%s:%d] select error: %s ", 
//					__FILE__, __LINE__, strerror(errno));
//			return;
//		} else if (ret == 0) {
//			errno = ETIMEDOUT;		
//			return;
//		}
//
//		if (FD_ISSET(connfd, &rset)) {
//			n = read(connfd, SWS_buf->start, 
//					&SWS_buf->buf[SWS_BUF_LEN] - SWS_buf->end);
//
//			if (n == 0) {
//				SWS_log_info("[%s:%d] client terminated prematurely\n",
//						__FILE__, __LINE__);		
//				return;
//				FD_CLR(connfd, &rset);	
//				end_flag = 1;
//			}
//
////			if (n == SWS_TIMEOUT) {
////				SWS_log_info("[%s:%d] read/write time out, closing...\n",
////						__FILE__, __LINE__);	
////				FD_CLR(connfd, &wset);				
////			}
//
//			if (n < 0 && errno != EWOULDBLOCK) {
//				SWS_log_error("[%s:%d] read error: %s\n", __FILE__, 
//						__LINE__, strerror(errno));		
//				return;
//			} 
//
//			SWS_buf->end += n;
//		}
//
//		if (FD_ISSET(connfd, &wset)) {
//			n = write(connfd, SWS_buf->start, 
//					SWS_buf->end - SWS_buf->start);	
//			
////			char *tmp;
////			for (tmp = SWS_buf->start; tmp != SWS_buf->end; tmp++) {
////				printf("%c", *tmp);
////			}
////			printf("\n");
//
//			if (n < 0 && errno != EWOULDBLOCK) {
//				SWS_log_error("[%s:%d] write error: %s\n", 
//					__FILE__, __LINE__, strerror(errno));		
//			}
//			
//			SWS_buf->start += n;
//
//			if ((SWS_buf->start == SWS_buf->end) && end_flag) {
//				return;		
//			}
//		}
//			
//	}
//}

//static void SWS_connect_init(struct SWS_request_t **request, 
//		struct SWS_connect_t **connect);
//
//static int SWS_parse_request_header_line(void *header, int len, 
//		struct SWS_request_t **req, struct SWS_connect_t **con);
//
//static int SWS_parse_method(void *data, int len, struct SWS_request_t **req);
//
//static int SWS_parse_url(void *data, int len, struct SWS_request_t **req);
//
//static int SWS_parse_request_header(void *header, int len, 
//		struct SWS_request_t **req, struct SWS_connect_t **con);
//
//static int SWS_parse_host(char *data, int len, struct SWS_request_t **req);
//
//static int SWS_parse_connection(char *content, int clen, 
//		struct SWS_request_t **req);
//
//static int SWS_parse_content_length(char *content, int clen, 
//		struct SWS_request_t **req);
//
//static int SWS_parse_content(char *header, int hlen, struct SWS_request_t **req,
//		struct SWS_request_t **con);
//
//
//static struct SWS_field SWS_all_field[]  = {
//	{"Host", SWS_parse_host},
//	{"Connection", SWS_parse_connection},
//	{"Content-Length", SWS_parse_content_length}//,
////	{"Accept", SWS_parse_accept},
////	{"If", SWS_parse_if},
////	{"Authorization", SWS_parse_authorization},
////	{"From", SWS_parse_from},
////	{"Expect", SWS_parse_expect},
////	{"Max-Forwards", SWS_parse_max},
////	{"Proxy-Authorization", SWS_parse_proxy},
////	{"Range", SWS_parse_range},
////	{"Referer", SWS_parse_Referer}
////	{"TE", SWS_parse_TE}
//}; 
//
//char SWS_info_st[][SWS_STATUS_LEN] = {
//	"Continue",
//	"Switching Protocols"
//};
//
//char SWS_success_st[][SWS_STATUS_LEN] = {
//	"OK",
//	"Created",
//	"Accepted",
//	"Non-Authoritative information",
//	"No Content",
//	"Reset Content",
//	"Partial Content"
//};
//
//char SWS_redirect_st[][SWS_STATUS_LEN] = {
//	"Multiple Choices",
//	"Moved Permanently",
//	"Found",
//	"See Other",
//	"Not Modified",
//	"Use Proxy",
//	"Unused",
//	"Temporary Redirect"
//};
//
//char SWS_clierror_st[][SWS_STATUS_LEN] = {
//	"Bad Request",
//	"Unauthorized",
//	"Payment Required",
//	"Forbidden",
//	"Not Found",
//	"Method Not Allowed",
//	"Not Acceptable",
//	"Proxy Authentication Required",
//	"Request Timeout",
//	"Confilict",
//	"Gone",
//	"Length Required",
//	"Precondition Failed",
//	"Request Entity Too Large",
//	"Request-URI Too Long",
//	"Unsupported Media Type",
//	"Requested Range Not Satisfiable",
//	"Expectation Failed"
//};
//
//char SWS_sererror_st[][SWS_STATUS_LEN] = {
//	"Internal Server Error",
//	"Not Implemented",
//	"Bad Gateway",
//	"Service Unavailable",
//	"Gateway Timeout",
//	"HTTP version Not Supported"
//};
//
//void
//SWS_connect_init(struct SWS_request_t **req, struct SWS_connect_t **con)
//{
//	/* TODO memory_poll */
//	(*req) = (struct SWS_request_t *)malloc(sizeof(struct SWS_request_t));
//	(*con) = (struct SWS_connect_t *)malloc(sizeof(struct SWS_request_t));
//
//	/* 1k magic number*/
//	memset(*req, 0, sizeof(*req));
//	memset(*con, 0, sizeof(*con));
//	(*con)->buffer = malloc(SWS_HEADER_LEN);
//	(*con)->buf_loc = 0;
//	(*con)->read_len = SWS_HEADER_LEN;
//
//	/* 挂载读和解析事件 */
//	(*con)->recv = SWS_read_request_line;
//	(*con)->parse = SWS_parse_request_header_line;
//}
//
//void
//SWS_web_interation(int connect_fd)
//{
//	int n;
//	struct SWS_request_t *req;
//	struct SWS_connect_t *con; 
//
//	SWS_connect_init(&req, &con);
//
//	for ( ;; )  {
//		n = con->recv(connect_fd, &con->buffer[con->buf_loc], 
//				con->read_len);
//	
//		if (n == 0) {
//			SWS_log_info("[%s:%d] client terminated prematurely",
//					__FILE__, __LINE__);		
//			exit(EXIT_SUCCESS);
//		}
//	
//		if (n == -1) {
//			if (errno == EINTR) {
//				continue;
//			}
//			SWS_log_warn("[%s:%d] read error: %s", __FILE__,
//						__LINE__);
//			return;
//		}
//
//		/* update buffer's used size */
//		con->buf_loc += n;
//		if (n == SWS_UNFINISHED) {
//			continue;			
//		}
//
//		printf("%s\n", con->buffer);
//
////		con->parse(con->buffer, con->buf_loc, &req, &con);
//
//	}
//} 		
//
//int
//SWS_parse_request_header_line(void *header, int len, struct SWS_request_t **req,
//		struct SWS_connect_t **con)
//{
////	enum line { 
////		all = 0,		
////		method,
////		url,
////		version
////	};
////	void *start = header, *end;
////	int status = 0, ret;
////	
////	for (end = header; end != header + len; end++) {
////		if (*end == "_" || *end == '\r') {
////			status += 1;	
////		}	
////		
////		switch (status) {
////		
////		case method:	
////			if ((ret = SWS_parse_method(start, end - start, req))) {
////				return ret;
////			}			
////			start = end + 1;
////			break;						
////
////		case url:
////			if ((ret = SWS_parse_url(start, end - start, req))) {
////				return ret;	
////			}
////			start = end + 1;
////			break;						
////		
////		case version:
////			if (end - start != SWS_HTTP_VERSION_LEN) {
////				return SWS_ERROR;
////			}
////
////			// TODO:版本局限1.1 
////			if (SWS_str_cmp8(start, "HTTP/1.1")) {
////				(*req)->version = SWS_HTTP_1_1; 
////			} else {
////				return SWS_ERROR;
////			}
////			break;
////		}
////	}
////
////	if (status < version) {
////		return SWS_ERROR;	
////	}
////
////	/* 改变读和解析事件 */
////	memset(c->buffer, 0, sizeof(buffer));
////	c->buf_loc = 0;
////	c->recv = SWS_read_header;
////	c->parse = SWS_parse_request_header;
//
//	return SWS_OK;
//}
//
//int
//SWS_parse_method(void *data, int len, struct SWS_request_t **req)
//{
////	switch (len) {
////	
////	case 3:
////		if (SWS_str_cmp3(data, "GET")) {
////			(*req)->method = SWS_HTTP_GET;
////		} else if (SWS_str_cmp3(data, "PUT")) {
////			(*req)->method = SWS_HTTP_PUT;
////		} else {
////			return SWS_ERROR;	
////		}
////		break;
////	case 4:
////		if (SWS_str_cmp4(data, "POST")) {
////			(*req)->method = SWS_HTTP_POST;
////		} else if (SWS_str_cmp4(data, "HEAD")) {
////			(*req)->method = SWS_HTTP_HEAD;
////		} else {
////			return SWS_ERROR;
////		}
////		break;
////
////	case 5:
////		if (SWS_str_cmp5(data, "TRACE")) {
////			(*req)->method = SWS_HTTP_TRACE;			
////		} else {
////			return SWS_ERROR;
////		}
////		break;
////
////	case 6:
////		if (SWS_str_cmp6(data, "DELETE")) {
////			(*req)->method = SWS_HTTP_DELETE;	
////		} else {
////			return SWS_ERROR;
////		}
////		break;
////
////	case 7:
////		if (SWS_str_cmp7(data, "OPTIONS")) {
////			(*req)->method = SWS_HTTP_OPTIONS;
////		} else {
////			return SWS_ERROR;
////		}
////		break;
////
////	default:
////		return SWS_ERROR;
////	}		
//	
//	return SWS_ERROR;
//}
//
//int
//SWS_parse_url(void *data, int len, struct SWS_request_t **req)
//{
//	return SWS_OK;	
//}
//
//int
//SWS_parse_request_header(void *header, int len, struct SWS_request_t **req,
//{
////	int ret
////	void *start = header, end;
////	struct SWS_request_t *r = *req;	
////	struct SWS_connect_t *c = *con;
////	
////	for (end = header; end != header + len; end++) {
////		if (*end == CR) {
////			if (*end++ != LF) {
////				return SWS_ERROR
////			}
////			if ((ret = SWS_parse_header_line(start, 
////						end - start - 1, req))) {
////				return ret;		
////			}
////			start = end + 1;
////		}
////	}
////
////	if (r->is_content) {
////		c->read_len = r->content_len; 
////	}
////
////	if (r->is_content) {
////		c->recv = SWS_read_content;
////		c->parse = SWS_parse_content;
////	} else {
////		memset(c->buffer, 0, sizeof(SWS_HEADER_LEN));
////		c->recv = SWS_read_request_line;
////		c->parse = SWS_parse_request_header_line;
////	}
//
////	//add
//	return SWS_OK;
//}
//
//int
//SWS_parse_header_line(void *line, int len, struct SWS_request_t **req)
//{
////	char *ptr, *end;
////	char *sline = (char *)line;
////
////	for (ptr = sline; ptr != sline + len; ptr++) {
////		if (*ptr > 127 || *ptr < 0) {
////			return SWS_ERROR;		
////		}
////		if (*ptr == ":") {
////			end = ptr;
////		}
////	}
//	
////	for (i = 0; i < sizeof(SWS_all_field) / sizeof (SWS_all_field[0]); i++) {
////		if (!strncmp(sline, SWS_all_field[i].name)) {
////			SWS_all_field[i].parse(content, clen, req);
////				break;
////		}		
////	}		
////	/* TODO 自定义域 */
////	//add
//	return SWS_ERROR;
//}
//
//int
//SWS_parse_host(char *content, int clen, struct SWS_request_t **req)
//{
////	struct stat st;
////	
////	/* not absolute url */
////	if ((*req)->url[0] != '/') {
////		return;		
////	}	
////	(*req)->host = (char *)malloc(strlen(content) + 1);	
////
//	return SWS_ERROR;
//}
//
//int
//SWS_parse_connection(char *content, int clen, struct SWS_request_t **req) 
//{
////	if (!strcmp(content, "close")) {
////		(*req)->connection = SWS_CLOSE;	
////	} else if (!strcmp(content, "keep-alive")) {
////		(*req)->content = SWS_ALIVE;
////	} else {
////		return 400;
////	}
////
//	return SWS_OK;
//}
//
//int
//SWS_parse_content_length(char *content, int clen, struct SWS_request_t **req)
//{
////	int len; 
////
////	if (strlen(content) > 4) {
////		return 413;
////	}
////	while (*content != '\0') {
////		if (*content > '9' || *content < '0') {
////			return 400;	
////		} 
////	}
////	sscanf(content, "%d", len);
////	if (len > SWS_request_body) {
////		return 413;	
////	}
////
////	(*req)->content_len = len;
//	return SWS_OK;
//}	
//
//int
//SWS_parse_content(char *header, int hlen, struct SWS_request_t **req,
//		struct SWS_request_t **con)
//{
//	return 0;
//}

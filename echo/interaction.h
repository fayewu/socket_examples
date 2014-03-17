#ifndef _INTERACTION_H
#define _INTERACTION_H

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SWS_BUF_LEN				1024
#define	SWS_QUEUE_LEN				100 

struct SWS_buf_t {
	char *addr;
	int loc;
};


extern void SWS_echo_interation();

//#define SWS_HEADER_LEN			1024				/* 预读1k */
//#define SWS_WEB_ROOT_LEN		sizeof(SWS_web_root) + 1
//#define SWS_STATUS_LEN			50
//
//#define SWS_ERROR			1
//#define SWS_OK				0
//
//#define SWS_CLOSE			1
//#define SWS_ALIVE			0
//
//#define SWS_HTTP_VERSION_LEN		8
//#define SWS_HTTP_1_1			1
//
//#define SWS_HTTP_GET			0
//#define SWS_HTTP_HEAD			1
//#define SWS_HTTP_POST			2
//#define SWS_HTTP_PUT			3
//#define SWS_HTTP_TRACE			4
//#define SWS_HTTP_OPTIONS		5
//#define SWS_HTTP_DELETE			6
//
//struct SWS_request_t **req;
//struct SWS_connect_t **con;
//
//typedef	ssize_t (*SWS_read_event)(int sockfd, void *ptr, size_t len);	
//typedef int (*SWS_parse_event)(void *data, int len, struct SWS_request_t **req,
//					struct SWS_connect_t **con);
//typedef int (*SWS_field_pt)(char *data, int len, struct SWS_request_t **req);
//
//struct SWS_field {
//	char *name;	
//	SWS_field_pt  parse;
//};
//
//struct SWS_connect_t {
//	char *buffer;		
//	int buf_loc;
//	int read_len;
//
//	SWS_read_event recv;	
//	SWS_parse_event parse;
//
////	int is_read;
////	int is_parse;
////	int is_write;
//};
//
//struct SWS_request_t {
//	char method[10];	
//	char *url;
//	char version[10];	/* TODO 待处理 */
//
//	struct SWS_header_t *header;
//
//	/* flags field */
//	int is_content;
//	int return_status; 
//}; 
//
//struct SWS_header_t {
//	char *host;		
//	int connection;
//	int content_len;
//};
//
///* kinds of information of status */
//extern char SWS_info_st[][SWS_STATUS_LEN];
//extern char SWS_success_st[][SWS_STATUS_LEN]; 
//extern char SWS_redirect_st[][SWS_STATUS_LEN];
//extern char SWS_clierror_st[][SWS_STATUS_LEN];
//extern char SWS_sererror_st[][SWS_STATUS_LEN];
//
//extern void SWS_web_interation(int connect_fd);

#endif

#ifndef _INTERACTION_H
#define _INTERACTION_H

#include <stdio.h>
#include <string.h>

#define  SWS_HEADER_LEN		1024		/* 预读1k */

struct SWS_request_t **req;
struct SWS_connect_t **con;

typedef	ssize_t (*SWS_read_event)(int sockfd, void *ptr, size_t len);	
typedef void (*SWS_parse_event)(char *data, struct SWS_request_t **req,
					struct SWS_connect_t **con);

//#define GET		224		 
//#define POST		326	
//#define HEAD		274
//#define PUT		249	 
//#define TRACE		367	 
//#define OPTIONS	556 

struct SWS_connect_t {
	char *buffer;		
	int buf_loc;

	SWS_read_event recv;	
	SWS_parse_event parse;
};

struct SWS_request_t {
	char method[10];	
	char *url;	/* TODO host的情况待考虑 */
	char version[10];	/* TODO 待处理 */
	int is_content;
}; 

extern void SWS_web_interation(int connect_fd);

#endif

#ifndef _INTERACTION_H
#define _INTERACTION_H

#include <stdio.h>
#include <string.h>

#define  SWS_HEADER_LEN		1024		/* 预读1k */

typedef	ssize_t (*SWS_read_event)(int sockfd, void *ptr, size_t len);	
typedef int (*SWS_parse_event)(const char *data, struct SWS_request_t **req);

//#define GET		224		 
//#define POST		326	
//#define HEAD		274
//#define PUT		249	 
//#define TRACE		367	 
//#define OPTIONS	556 

static struct SWS_connect_t {
	char *buffer;		
	int buf_loc;

	SWS_read_event *recv;	
	SWS_parse_event *parse;
};

static struct SWS_request_t {
	int method;	
	char *url;	/* TODO host的情况待考虑 */
	int version;	/* TODO 待处理 */
	int is_content;
}; 

#endif

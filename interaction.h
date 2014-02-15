#ifndef _INTERACTION_H
#define _INTERACTION_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define	SWS_HEADER_LEN			1024				/* 预读1k */
#define	SWS_WEB_ROOT_LEN		sizeof(SWS_web_root) + 1
#define SWS_STATUS_LEN			50

//#define GET		224		 
//#define POST		326	
//#define HEAD		274
//#define PUT		249	 
//#define TRACE		367	 
//#define OPTIONS	556 

struct SWS_request_t **req;
struct SWS_connect_t **con;

typedef	ssize_t (*SWS_read_event)(int sockfd, void *ptr, size_t len);	
typedef void (*SWS_parse_event)(char *data, struct SWS_request_t **req,
					struct SWS_connect_t **con);
typedef void (*SWS_field_pt)(char *content, int clen, struct SWS_request_t **req);

struct SWS_field {
	char *name;	
	SWS_field_pt  parse;
};

struct SWS_connect_t {
	char *buffer;		
	int buf_loc;

	SWS_read_event recv;	
	SWS_parse_event parse;

	int is_read;
	int is_parse;
	int is_write;
};

struct SWS_request_t {
	char method[10];	
	char *url;
	char version[10];	/* TODO 待处理 */

	struct SWS_header_t *header;

//	/* flag field */
//	int is_content;
//	int is_aburl;
//
//	/* use to write */
//	int status;
}; 

struct SWS_header_t {
	char *host;		
	int connection;
};

/* kinds of information of status */
extern char SWS_info_st[][SWS_STATUS_LEN];
extern char SWS_success_st[][SWS_STATUS_LEN]; 
extern char SWS_redirect_st[][SWS_STATUS_LEN];
extern char SWS_clierror_st[][SWS_STATUS_LEN];
extern char SWS_sererror_st[][SWS_STATUS_LEN];

extern void SWS_web_interation(int connect_fd);

#endif

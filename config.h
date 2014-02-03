#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <limits.h>
#include <libconfig.h>

#define DEFAULT_FILE		"default.cfg"

extern int SWS_port;
extern int SWS_request_header;
extern const char *SWS_addr;
extern const char *SWS_error_log;

extern void SWS_get_config();

#endif

#include "config.h"
#include "log.h"

int SWS_port;
int SWS_request_header; 
const char *SWS_addr;
const char *SWS_error_log;
const char *web_root;

/* create default configuration file */
void
SWS_get_config()
{
	config_t cfg;

	config_init(&cfg);

	if (!config_read_file(&cfg, DEFAULT_FILE)) {
		SWS_log_fatal("%s", config_error_line(&cfg));		
	}		

	if (!config_lookup_int(&cfg, "SWS_port", &SWS_port)) {
		SWS_log_fatal("default configuration file has been changed,"
				"No port!");
	}
	if (!config_lookup_string(&cfg, "SWS_address", &SWS_addr)) {
		SWS_log_fatal("default configuration file has been changed,"
				"No address!");
	}
	if (!config_lookup_string(&cfg, "SWS_error_log", &SWS_error_log)) {
		SWS_log_fatal("default configuration file has been changed,"
				"No error_log!");
	}
	if (!config_lookup_string(&cfg, "SWS_web_root", &SWS_web_root)) {
		SWS_log_fatal("default configuration file has been changed,"
				"No web_root!");
	}
	if (!config_lookup_int(&cfg, "SWS_request_header", &SWS_request_header)) {
		SWS_log_fatal("default configuration file has been changed,"
				"No url_length!");
	}

	config_destroy(&cfg);
}

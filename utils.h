#ifndef _UTILS_H
#define _UTILS_H

#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define BUF_LEN			65536

#ifdef SWS_DEBUG
#define DEBUG_INFO(s) printf s
#else
#define DEBUG_INFO(s)
#endif

#define True		1
#define False		0

#define CRLF		"\r\n"
#define CR		"\r"
#define LF		"\n"

#define SWS_str_cmp3(a, b)						      \
	a[0] == b[0] && a[1] == b[1] && a[2] == b[2]
#define SWS_str_cmp4(a, b)						      \
	a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]
#define SWS_str_cmp5(a, b)						      \
	a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]          \
	&& a[4] == b[4]
#define SWS_str_cmp6(a, b)						      \
	a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]          \
	&& a[4] == b[4] && a[5] == b[5]
#define SWS_str_cmp7(a, b)						      \
	a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]          \
	&& a[4] == b[4] && a[5] == b[5] && a[6] == b[6]
#define SWS_str_cmp8(a, b)						      \
	a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]          \
	&& a[4] == b[4] && a[5] == b[5] && a[6] == b[6] && a[7] == b[7]

extern void user_info(const char *str);
extern void user_info1(const char *format, ...);
extern void user_exit(const char *str);
extern void user_exit1(const char *format, ...);
extern void err_info(const char *str);
extern void err_exit(const char *str);
extern void err_exit1(const char *format, ...);

#endif

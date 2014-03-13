#ifndef _MEMORY_POLL_H
#define _MEMORY_POLL_H

#include <stdio.h>
#include <stdlib.h>

#define	block_length	8192		 

struct memory_unit {
	void *begin;
	size_t size;
};

struct memory_poll_block {
	void *begin;	
	struct memory_poll_block *next;
	struct memory_unit *free;
};

extern void memory_poll_init();
extern void *memory_malloc(size_t size);
extern void memory_poll_free();

#endif

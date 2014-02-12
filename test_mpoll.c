#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "memory_poll.h"

int
main()
{
	int i, j;
	clock_t begin, end;
	void *memory[5005];

	begin = clock();
	for (i = 0; i < 1000; i++) {
		for (j = 0; j < 5000; j++) {
			memory[j] = malloc(1000);	
		}
		for (j = 0; j < 5000; j++) {
			free(memory[j]);
		}
	}
	end = clock();

	printf("using malloc/free 's time is %lf\n", 
			(double)(end - begin)/CLOCKS_PER_SEC);


	begin = clock();
	for (i = 0; i < 1000; i++) {
		memory_poll_init();
		for (j = 0; j < 5000; j++) {
			memory[j] = memory_malloc(1000);
		}
		memory_poll_free();
	}	
	end = clock();
	
	printf("using memory_poll 's time is %lf\n", 
			(double)(end - begin)/CLOCKS_PER_SEC);

	return 0;
}

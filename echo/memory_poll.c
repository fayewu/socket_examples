#include "memory_poll.h"

static struct memory_poll_block *block_head = NULL; 

static void *Malloc(size_t size);
static void apply_memory_block();

void *
Malloc(size_t size)
{
	void *ptr;

	if ((ptr = malloc(size)) == NULL) {
		printf("malloc error!");
		exit(EXIT_FAILURE);
	}	

	return ptr;
}

void
apply_memory_block(size_t size)
{
	void *ptr;
	struct memory_poll_block *mptr;
	struct memory_unit *free;

	ptr = Malloc(size);	
//	printf("malloc ptr : 0x%x\n", ptr);
	mptr = Malloc(sizeof(struct memory_poll_block));
	free = Malloc(sizeof(struct memory_unit));

	mptr->next = block_head;
	block_head = mptr;
	block_head->begin = ptr;
	
	free->begin = ptr;
	free->size = block_length;
	block_head->free = free;
}

void
memory_poll_init()
{
	apply_memory_block(block_length);
}

void *
memory_malloc(size_t size)
{
	void *ptr;
	int apply_size;

//	/* todo 不为头的block可能有剩余的空间*/
//	if (block_head->free->size < size) {
//		apply_size = size > block_length? size : block_length;	
//		apply_memory_block(apply_size);
//	}

	ptr = block_head->free->begin;

	block_head->free->begin += size;
	block_head->free->size -= size;

	return ptr;
}

/* 不重复利用 */
void
memory_poll_free()
{
	struct memory_poll_block *mptr = block_head;	

	/* 删除内存块 */
	while (mptr) {
//		printf("free ptr : 0x%x\n", mptr->begin);
		free(mptr->begin);
		mptr = mptr->next;
	}

	/* 删除保存内存块的memory_poll_block结构 */
	while (block_head) {
		mptr = block_head;
		block_head = block_head->next;

		free(mptr->free);
		free(mptr);
	}

}

//int
//main()
//{
//	int i, j;
//
//	for (i = 0; i < 100; i++) {
//		memory_poll_init();
//		for (j = 0; j < 2; j++) {
//			memory_malloc(1000);
//		}
//		memory_poll_free();
//	}
//
//	return 0;
//}

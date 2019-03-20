#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>

#define META_SIZE sizeof(struct block_meta)
void *global_base = NULL;

struct block_meta
{
	size_t size;
	struct block_meta *next;
	struct block_meta *prev;

	int free;
	int magic; // For debugging only. TODO: remove this in non-debug mode.
};

struct block_meta *find_free_block(struct block_meta **last, size_t size)
{
	struct block_meta *current = global_base; //head

	while ( current && !(current->free && current->size >= (size*8) ) )
	{
		*last = current;
		current = current->next;
	}
	return current;
}

struct block_meta *request_space(struct block_meta *last, size_t size)
{
	struct block_meta *block;
	block = sbrk(0);
	void *request = sbrk((size*8) + META_SIZE);
	assert((void *)block == request); // Not thread safe.

	if (request == (void *)-1)
	{
		return NULL; // sbrk failed.
	}

	if (last)
	{ // NULL on first request.
		last->next = block;
	}

	block->size = size*8;
	block->prev = last;
	block->next = NULL;
	block->free = 0;
	block->magic = 0x12345678;
	return block;
}

struct block_meta *get_block_ptr(void *ptr)
{
	return (struct block_meta *)ptr - 1;
}


/*
  Allocate a segment of memory in the heap, of at least the given size, 
  and return a pointer to this segment.
*/
void *malloc(size_t size)
{
	struct block_meta *block;
	// TODO: align size?

	if (size <= 0)
	{
		return NULL;
	}

	if (!global_base)
	{ // First call.
		block = request_space(NULL, size);
		if (!block)
		{
			return NULL;
		}
		global_base = block;
	}

	else
	{
		struct block_meta *last = global_base;
		block = find_free_block(&last, size);
		if (!block)
		{ // Failed to find free block.
			block = request_space(last, size);

			if (!block)
			{
				return NULL;
			}
		}

		else
		{ // Found free block
			block->free = 0;
			block->magic = 0x77777777;
		}
	}

	return (block + 1);
}

/*
  Allocate a segment of memory in the heap to store nitems elements of the given size. 
  Set the entire segment of memory to 0. 
  Return a pointer to this segment.
*/
void *calloc(size_t nitems, size_t size)
{
	size_t size = nitems * size; // TODO: check for overflow.
	void *ptr = malloc(size);
	memset(ptr, 0, size*8);
	return ptr;
}

/*
  Resize the memory segment pointed to by ptr to the given size. 
  Return the pointer to the newly sized segment. 
*/
void *realloc(void *ptr, size_t size)
{
	if (!ptr)
	{
		// NULL ptr. realloc should act like malloc.
		return malloc(size);
	}

	struct block_meta *block_ptr = get_block_ptr(ptr);
	if (block_ptr->size >= size)
	{
		// We have enough space. Could free some once we implement split.
		return ptr;
	}

	// Need to really realloc. Malloc new space and free old space.
	// Then copy old data to new space.
	void *new_ptr;
	new_ptr = malloc(size);
	if (!new_ptr)
	{
		return NULL; // TODO: set errno on failure.
	}
	memcpy(new_ptr, ptr, block_ptr->size);
	free(ptr);
	return new_ptr;
}

/*
  Deallocate the memory segment pointed to by ptr.
*/
void free(void *ptr)
{
	if (!ptr)
	{
		return;
	}

	// TODO: consider merging blocks once splitting blocks is implemented.
	struct block_meta *block_ptr = get_block_ptr(ptr);
	assert(block_ptr->free == 0);
	assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
	block_ptr->free = 1;
	block_ptr->magic = 0x55555555;
}
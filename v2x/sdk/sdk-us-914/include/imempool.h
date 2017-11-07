
#ifndef IMEMPOOL_H
#define IMEMPOOL_H

#define POOL_SIZE 512 * 1024 // 512K
#define MIN_POOL_ALLOC_QUANTAS 16

typedef unsigned char byte;
typedef unsigned long ulong;

void imempool_init(void);
void* i_alloc(ulong nbytes);
void* i_calloc(int count, ulong nbytes);
void* i_free(void* memory);
void* i_realloc(void* memory, ulong nbytes);

#endif // IMEMPOOL_H

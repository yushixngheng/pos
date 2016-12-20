#ifndef _NGX_POOL_H_
#define _NGX_POOL_H_

#include "ngx_core.h"

#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT	sizeof(unsigned long)
#endif

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define NGX_POOL_ALIGNMENT	16
#define NGX_MAX_ALLOC_FROM_POOL (8*1024-1)
#define NGX_POOL_DEFAULT_SIZE	16*1024

typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};


typedef struct {
	u_char	*last;
	u_char	*end;
	ngx_pool_t *next;
	ngx_uint_t failed;
}ngx_pool_data_t;


//内存池头部结构
struct ngx_pool_s {
	ngx_pool_data_t d;
	size_t max;
	ngx_pool_t *current;
	ngx_pool_large_t *large;

};


ngx_pool_t* ngx_create_pool(size_t size);
void ngx_destroy_pool(ngx_pool_t* pool);
void ngx_reset_pool(ngx_pool_t *pool);
void* ngx_palloc(ngx_pool_t *pool, size_t size);
void* ngx_pcalloc(ngx_pool_t *pool, size_t size);
void* ngx_pnalloc(ngx_pool_t* pool, size_t size);
void* ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void* p);
void* ngx_prealloc(ngx_pool_t* pool, void* addr, size_t osize, size_t nsize);

#endif
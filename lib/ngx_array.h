#ifndef _NGX_ARRAY_H_
#define _NGX_ARRAY_H_


#include "ngx_core.h"


typedef struct {
	void	*elts;
	ngx_uint_t nelts;
	size_t size;
	ngx_uint_t nalloc;
	ngx_pool_t *pool;
}ngx_array_t;


ngx_array_t* ngx_array_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);
void ngx_array_destroy(ngx_array_t *a);
void* ngx_array_push(ngx_array_t *a);
void* ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


static ngx_inline ngx_int_t ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_pcalloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif
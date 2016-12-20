#include "ngx_core.h"


static void* ngx_palloc_block(ngx_pool_t *pool, size_t size);
static void* ngx_palloc_large(ngx_pool_t *pool, size_t size);

ngx_pool_t*
ngx_create_pool(size_t size) {
	ngx_pool_t *p;

	p = ngx_memalign(NGX_POOL_ALIGNMENT, size);
	if (p == NULL) {
			return NULL;
	}
	p->d.last = (u_char*)p + sizeof(ngx_pool_t);
	p->d.end = (u_char*)p + size;
	p->d.next = NULL;
	p->d.failed = 0;

	size = size - sizeof(ngx_pool_t);
	p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;
	p->current = p;
	p->large = NULL;

	return p;
}

void
ngx_destroy_pool(ngx_pool_t* pool) {
	ngx_pool_t *p, *n;
	ngx_pool_large_t *l;

	for (l = pool->large; l; l = l->next) {
 		//ErrorLog(INFO, "free large memory");
		if (l->alloc) {
			ngx_free(l->alloc);
		}
	}
	for (p = pool, n = p->d.next; ;p = n, n = n->d.next) {
			ngx_free(p);
			if (n == NULL) {
				break;
			}
	}
	// ErrorLog(INFO, "free large memory");
}


void
ngx_reset_pool(ngx_pool_t *pool) {
	ngx_pool_large_t *l;
	ngx_pool_t *p;

	for (l = pool->large; l; l = l->next) {

		if (l->alloc) {
			ngx_free(l->alloc);
		}
	}
    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->large = NULL;
}


void*
ngx_palloc(ngx_pool_t *pool, size_t size) {
	u_char *m;
	ngx_pool_t *p;

	if (pool->max >= size) {
		p = pool->current;
		do {
			m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);
			if ((size_t)(p->d.end - m) >= size) {

				p->d.last = m + size;
				return (void*)m;
			}
			p = p->d.next;
		}while (p);
		return ngx_palloc_block(pool, size);
	}
	return ngx_palloc_large(pool, size);
}


void*
ngx_pcalloc(ngx_pool_t *pool, size_t size) {
	void* m;

	m = ngx_palloc(pool, size);
	if (m == NULL) {
		return NULL;
	}
	memset(m, 0, size);
	return m;
}


void*
ngx_pnalloc(ngx_pool_t* pool, size_t size) {
	u_char *m;
	ngx_pool_t *p;

	if (pool->max >= size) {
		p = pool->current;
		do {

			m = p->d.last;
			if ((size_t)(p->d.end - m) >= size) {

				p->d.last = m + size;
				return (void*)m;
			}
			p = p->d.next;
		}while (p);
		return ngx_palloc_block(pool, size);
	}
	return ngx_palloc_large(pool, size);
}


static void*
ngx_palloc_block(ngx_pool_t* pool, size_t size) {
	u_char* m;
	size_t psize;
	ngx_pool_t *p, *n;

	psize = (size_t)(pool->d.end -(u_char*)pool);

	m = (u_char *)ngx_memalign(NGX_POOL_ALIGNMENT, psize);
	if (m == NULL) {
		return NULL;
	}
	n = (ngx_pool_t*)m;
	n->d.end = m + psize;
	n->d.next = NULL;
	n->d.failed = 0;
	m = m + sizeof(ngx_pool_data_t);
	m = ngx_align_ptr(m, NGX_ALIGNMENT);
	n->d.last = m + size;

	n->large = NULL;

	for (p = pool->current; p->d.next; p = p->d.next) {
		if (p->d.failed++ > 4) {
			pool->current = p->d.next;
		}
	}
	p->d.next = n;
	return m;
}


static void*
ngx_palloc_large(ngx_pool_t *pool, size_t size) {
  	void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size);
    if (p == NULL) {
        return NULL;
    }
    n = 0;

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }
        if (n++ > 3) {
            break;
        }
    }

    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;
    return p;
}


//该函数只针对申请大块内存
void* ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment) {
	void* p;
	ngx_pool_large_t *l;

	p = ngx_memalign(alignment, size);
	if (p == NULL) {
		return NULL;
	}
	l = (ngx_pool_large_t*)ngx_palloc(pool, sizeof(ngx_pool_large_t));
	if (l == NULL) {
		ngx_free(p);
		return NULL;
	}

	l->alloc = p;
	l->next = pool->large;
	pool->large = l;
	return p;
}


//该函数只针对释放大块内存
ngx_int_t
ngx_pfree(ngx_pool_t *pool, void* p) {
	ngx_pool_large_t *l;

	for (l = pool->large; l; l = l->next) {
		if (l->alloc == p) {
			ngx_free(p);
			l->alloc = NULL;
			return 0;
		}
	}
	return -1;
}


void*
ngx_prealloc(ngx_pool_t* pool, void* addr, size_t osize, size_t nsize)
{
	ngx_pool_t *p;
	u_char* m;
	ngx_pool_large_t *large;

	if (addr == NULL || osize == 0) {
		m = ngx_pcalloc(pool, nsize);
		if (m == NULL) {
			return NULL;
		}
		return m;
	}
	//在普通区域进行查找
	for (p = pool->current; p->d.next; p = p->d.next) {
		if (addr == p->d.last - osize) {	//找到对应内存地址
			//判断在对应页面上是否还存在空余空间
			if ((u_char*)addr + nsize <= p->d.end) {
				p->d.last = addr + nsize;
				return addr;
			}
		}
	}
	//在大块内存区域查找
	for (large = pool->large; large; large = large->next) {
		if (large->alloc == addr) {
			ErrorLog(ERROR, "reaaloc large p:%d, osize:%d, nsize:%d\n", addr, osize, nsize);
			large->alloc = realloc(addr, nsize);
			return large->alloc;
		}
	}
	m = ngx_pcalloc(pool, nsize);
	if (m == NULL) {
		return NULL;
	}
	ngx_cpymem(m, addr, osize);
	return m;
}

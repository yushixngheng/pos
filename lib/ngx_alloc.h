#ifndef _NGX_ALLOC_H_
#define _NGX_ALLOC_H_

#define ngx_free	free

void* ngx_alloc(size_t size);
void* ngx_calloc(size_t size);
void* ngx_memalign(size_t alignment, size_t size);



#endif
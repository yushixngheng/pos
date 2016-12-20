#ifndef _NGX_DATAPOOL_H_
#define _NGX_DATAPOOL_H_


#include "ngx_core.h"
#define NGX_DATAPOOL_UNUSED		0
#define NGX_DATAPOOL_USED     	1

#define NGX_DATAPOOL_INT		1
#define NGX_DATAPOOL_LONG		2
#define NGX_DATAPOOL_BUF		3


#define NGX_DATAPOOL_MAX_FIELDNUM			512
#define NGX_DATAPOOL_DEFAULT_FIELDNUM		100


typedef struct ngx_datapool_field_s ngx_datapool_field_t;
//typedef struct ngx_datapool_s ngx_datapool_t;

struct ngx_datapool_field_s {
	ngx_uint_t field_no;
	u_char field_type;
	ngx_int_t used;
	union {
		ngx_int_t  	i;				//int
		long   		 	l;				//long
		char*			buf;			//buf
	}value;
	size_t size;
};

struct ngx_datapool_s {
	void	*elts;
	size_t size;
	ngx_uint_t nalloc;
	ngx_pool_t *pool;
};


ngx_datapool_t* ngx_create_datapool(ngx_pool_t* pool, size_t n);
ngx_int_t ngx_clear_datapool(ngx_datapool_t* datapool);
ngx_int_t ngx_get_int_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no, ngx_int_t *value);
ngx_int_t ngx_add_int_to_datapool(ngx_datapool_t* datapool, ngx_uint_t field_no, ngx_int_t value);
ngx_int_t ngx_get_long_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no, long* value);
ngx_int_t ngx_add_long_to_datapool(ngx_datapool_t* datapool, ngx_uint_t field_no, long value);
char* ngx_get_buf_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no, ngx_int_t *len);
ngx_int_t ngx_add_buf_to_datapool(ngx_datapool_t* datapool, ngx_uint_t field_no, const u_char* value, size_t size);
ngx_int_t ngx_clear_datapool_field(ngx_datapool_t* datapool, ngx_int_t field_no);
ngx_int_t* ngx_get_intref_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no);
ngx_int_t ngx_datapool_compare_dict_value(ngx_datapool_t *datapool, ngx_int_t field_no1, ngx_int_t field_no2);


#endif
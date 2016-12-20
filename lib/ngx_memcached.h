#ifndef _NGX_MEMCACHED_H_
#define _NGX_MEMCACHED_H_


#include "ngx_core.h"


#define memcached_config "--SERVER=127.0.0.1:11211"
ngx_int_t ngx_memcached_set_value(char* namespace, char* key, char* value, int value_length, time_t expir);
char* ngx_memcached_get_value(char* namespace, char* key, ngx_int_t *len);
ngx_int_t posp_memcached_delete_key(char* namespace, char* key, time_t expir);
void ngx_memcached_free(char* namespace);

#endif
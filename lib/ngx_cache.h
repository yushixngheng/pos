#ifndef _NGX_CACHE_H_
#define _NGX_CACHE_H_

#include "ngx_core.h"
typedef struct {
	ngx_int_t dict_no;
	char* key;
}ngx_cache_bind_t;

typedef struct {
	ngx_int_t dict_no;
}ngx_cache_key_member_t;


#define NGX_CACHE_MESSAGE_PREFIX_KEY		"MESSAGE."
#define NGX_CACHE_REVERSAL_PREFIX_KEY		"REVERSAL."
#define NGX_CACHE_NOTIFY_PREFIX_KEY			"NOTIFY."


ngx_int_t ngx_generate_message_memcached_key(ngx_datapool_t *datapool, char* key);
ngx_int_t ngx_generate_reversal_memcached_key(ngx_datapool_t *datapool, char* key);
ngx_int_t ngx_generate_notify_memcached_key(ngx_datapool_t *datapool, char* key);
ngx_int_t ngx_save_reversal_info_to_memcache(ngx_datapool_t *datapool, time_t expir);
ngx_int_t ngx_load_reversal_info_from_memcache(ngx_datapool_t *datapool);
ngx_int_t ngx_save_notify_info_to_memcache(ngx_datapool_t *datapool, time_t expir);
ngx_int_t ngx_load_notify_info_from_memcache(ngx_datapool_t *datapool);
char* ngx_serialize_message_attr_to_json(ngx_message_t* message);
ngx_int_t ngx_deserialize_message_attr_from_json(ngx_message_t* message, char* json);
ngx_int_t ngx_save_message_attr_to_memcache(char* key, ngx_message_t* message, time_t expir);
ngx_int_t ngx_load_message_attr_from_memcache(char* key, ngx_message_t* message);
ngx_int_t ngx_delete_message_attr_from_memcache(char* key);

#endif
#include "ngx_core.h"
#include <libmemcached/memcached.h>


memcached_st *ngx_memcached;

ngx_int_t
ngx_memcached_set_namespace(char* namespace)
{
	if (memcached_callback_set(ngx_memcached, MEMCACHED_CALLBACK_NAMESPACE, namespace) != MEMCACHED_SUCCESS) {
		ErrorLog(ERROR, "memcached set callback MEMCACHED_CALLBACK_NAMESPACE [%s] error", namespace);
		return NGX_ERROR;
	}
	return NGX_OK;
}

void
ngx_memcached_free(char* namespace)
{
	memcached_free(ngx_memcached);
	return;
}

ngx_int_t
ngx_memcached_clear_namespace() {
	if (memcached_callback_set(ngx_memcached, MEMCACHED_CALLBACK_NAMESPACE, NULL) !=  MEMCACHED_SUCCESS) {
		ErrorLog(ERROR, "clear memcached namespace error");
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_memcached_set_value(char* namespace, char* key, char* value, int value_length, time_t expir)
{
	memcached_return_t mrt;
	if (namespace != NULL && strlen(namespace) > 0) {
		if (ngx_memcached_set_namespace(namespace) != NGX_OK) {
			return NGX_ERROR;
		}
	}
	mrt = memcached_set(ngx_memcached, key, strlen(key), value, value_length, expir, 0);
	if (mrt != MEMCACHED_SUCCESS) {
		ErrorLog(ERROR, "memcached set by key error[%d][%s], namespace[%s] key[%s]", mrt, memcached_strerror(ngx_memcached, mrt),namespace, key);
		ngx_memcached_clear_namespace();
		return NGX_ERROR;
	}
	ngx_memcached_clear_namespace();
	return NGX_OK;
}


char*
ngx_memcached_get_value(char* namespace, char* key, ngx_int_t *len)
{
	memcached_return_t mrt;
	size_t size = 0;
	uint32_t flag;
	char* v;
	if (namespace != NULL && strlen(namespace) > 0) {
		if (ngx_memcached_set_namespace(namespace) != NGX_OK) {
			return NULL;
		}
	}
	v = memcached_get(ngx_memcached, key, strlen(key), &size,  &flag, &mrt);
	if (v == NULL) {
		ErrorLog(ERROR, "memcached get value error[%d][%s], namespace[%s] key[%s]", mrt, memcached_strerror(ngx_memcached, mrt),namespace, key);
		ngx_memcached_clear_namespace();
		return NULL;
	}
	if (len != NULL) {
		*len = size;
	}
	ngx_memcached_clear_namespace();
	return v;
}


ngx_int_t
ngx_memcached_delete_key(char* namespace, char* key, time_t expir)
{
	if (namespace != NULL && strlen(namespace) > 0) {
		if (ngx_memcached_set_namespace(namespace) != NGX_OK) {
			return NGX_ERROR;
		}
	}
	if (memcached_delete(ngx_memcached, key, strlen(key), expir) != MEMCACHED_SUCCESS) {
		ngx_memcached_clear_namespace();
		return NGX_ERROR;
	}
	ngx_memcached_clear_namespace();
	return NGX_OK;
}


ngx_int_t
ngx_memcached_process_init( )
{

	ngx_memcached = memcached(memcached_config, strlen(memcached_config));
	if (ngx_memcached == NULL) {
		ErrorLog(ERROR, "create memcached st error");
		return NGX_ERROR;
	}
	if (memcached_behavior_set(ngx_memcached, MEMCACHED_BEHAVIOR_DISTRIBUTION, MEMCACHED_DISTRIBUTION_CONSISTENT) != MEMCACHED_SUCCESS) {
		ErrorLog(ERROR, "memcached behavior set MEMCACHED_DISTRIBUTION_CONSISTENT error");
		return NGX_ERROR;
	}
	if (memcached_behavior_set(ngx_memcached, MEMCACHED_BEHAVIOR_NO_BLOCK, 1) != MEMCACHED_SUCCESS) {
		ErrorLog(ERROR, "memcached behavior set MEMCACHED_DISTRIBUTION_CONSISTENT error");
		return NGX_ERROR;
	}
	return NGX_OK;
}


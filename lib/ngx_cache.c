#include <proton/message.h>
#include <proton/messenger.h>
#include "ngx_core.h"


static ngx_int_t ngx_generate_cache_key(ngx_datapool_t *datapool, ngx_cache_key_member_t *m, char* key, size_t size);
static char* ngx_serialize_cache_to_json(ngx_datapool_t *datapool, ngx_cache_bind_t *b);
static ngx_int_t ngx_deserialize_cache_from_json(ngx_datapool_t *datapool, char *json, ngx_cache_bind_t *b);


static ngx_int_t
ngx_generate_cache_key(ngx_datapool_t *datapool, ngx_cache_key_member_t *m, char* key, size_t size)
{
	ngx_int_t i, len;
	char* v;

	ngx_memzero(key, size);
	for(i = 0; ;i++) {
		if (m[i].dict_no == -1) {
			break;
		}
		len = 0;
		v = ngx_get_buf_from_datapool(datapool, m[i].dict_no, &len);
		if (v == NULL || len <= 0) {
			ErrorLog(ERROR, "get cache key member [%d] from datapool error", m[i].dict_no);
			return NGX_ERROR;
		}
		if (strlen(key) + strlen(v) >= size) {
			return NGX_ERROR;
		}
		ngx_cpymem(key + strlen(key), v, strlen(v));
	}
	return strlen(key);
}


static char*
ngx_serialize_cache_to_json(ngx_datapool_t *datapool, ngx_cache_bind_t *b)
{
	ngx_int_t i, len;
	char *v, *json;
	cJSON *root;

	root = cJSON_CreateObject();
	if (root == NULL) {
		return NULL;
	}
	for (i = 0; ;i++) {
		if (b[i].dict_no == -1) {
			break;
		}
		len = 0;
		v = ngx_get_buf_from_datapool(datapool, b[i].dict_no, &len);
		if (v == NULL || len <= 0) {
			ErrorLog(ERROR, "can not get cache item [%d] from datapool", b[i].dict_no);
			continue;
		}
		cJSON_AddStringToObject(root, b[i].key, v);
	}
	json = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return json;
}


static ngx_int_t
ngx_deserialize_cache_from_json(ngx_datapool_t *datapool, char *json, ngx_cache_bind_t *b) {
	ngx_int_t i;
	cJSON *root, *v;

	root=cJSON_Parse(json);
	if (root == NULL) {
		return NGX_ERROR;
	}
	for (i = 0; ;i++) {
		if (b[i].dict_no == -1) {
			break;
		}
		v = cJSON_GetObjectItem(root, b[i].key);
		if (v == NULL) {
			ErrorLog(ERROR, "can not deserialize cache item [%s]", b[i].key);
			continue;
		}
		ngx_add_buf_to_datapool(datapool, b[i].dict_no, v->valuestring, strlen(v->valuestring));
	}
	cJSON_Delete(root);
	return NGX_OK;
}


ngx_int_t
ngx_generate_message_memcached_key(ngx_datapool_t *datapool, char* key)
{
	char* v, * p;
	char k[39 + 1] = {0};
	ngx_int_t len;

	static ngx_cache_key_member_t mkm[] = {
				NGX_DATAPOOL_MERCHANT_CODE,
				NGX_DATAPOOL_TERMINAL_CODE,
				NGX_DATAPOOL_TRANS_TIME,
				NGX_DATAPOOL_TERM_WATER,
				-1
	};
	len = ngx_generate_cache_key(datapool, mkm, k, sizeof(k));
	if (len <= 0) {
		return NGX_ERROR;
	}
	ngx_cpymem(key, k, len);
	return len;
}


//cups trade cache key member
static ngx_cache_key_member_t ctkm[] = {
		NGX_DATAPOOL_CHANNEL_MERCHANT_CODE,
		NGX_DATAPOOL_CHANNEL_TERMINAL_CODE,
		NGX_DATAPOOL_TRANS_TIME,
		NGX_DATAPOOL_CHANNEL_TERM_WATER,
		-1
};


//cups reversal cache bind
static ngx_cache_bind_t crb[] = {
		{NGX_DATAPOOL_SYS_CODE, 		"syscode"},
		{-1,							NULL}
};

//cups notify cache bind
static 	ngx_cache_bind_t cnb[] = {
		{NGX_DATAPOOL_MERCHANT_CODE, "int_mht_code"},
		{NGX_DATAPOOL_TERMINAL_CODE, "int_term_code"},
		{NGX_DATAPOOL_TERM_WATER, "int_term_water"},
		{-1,						  NULL}
};


ngx_int_t
ngx_generate_reversal_memcached_key(ngx_datapool_t *datapool, char* key)
{
	char* v, * p;
	char k[39 + 1] = {0};
	ngx_int_t len;

	len = ngx_generate_cache_key(datapool, ctkm, k, sizeof(k));
	if (len <= 0) {
		return NGX_ERROR;
	}
	ngx_cpymem(key, k, len);
	return len;
}


ngx_int_t
ngx_generate_notify_memcached_key(ngx_datapool_t *datapool, char* key)
{
	char* v, * p;
	char k[39 + 1] = {0};
	ngx_int_t len;

	len = ngx_generate_cache_key(datapool, ctkm, k, sizeof(k));
	if (len <= 0) {
		return NGX_ERROR;
	}
	ngx_cpymem(key, k, len);
	return len;
}


ngx_int_t
ngx_save_reversal_info_to_memcache(ngx_datapool_t *datapool, time_t expir)
{
	char key[39 + 1] = {0};
	char* json;

	if (ngx_generate_cache_key(datapool, ctkm, key, sizeof(key)) < 0) {
		return NGX_ERROR;
	}
	json = ngx_serialize_cache_to_json(datapool, crb);
	if (json == NULL) {
		return NGX_ERROR;
	}
	if (ngx_memcached_set_value(NGX_CACHE_REVERSAL_PREFIX_KEY, key, json, strlen(json), expir) != NGX_OK) {
		ngx_free(json);
		return NGX_ERROR;
	}
	ngx_free(json);
	return NGX_OK;
}


ngx_int_t
ngx_load_reversal_info_from_memcache(ngx_datapool_t *datapool)
{
	char key[39 + 1] = {0};
	ngx_int_t len = 0;
	char* v;

	if (ngx_generate_cache_key(datapool, ctkm, key, sizeof(key)) < 0) {
		return NGX_ERROR;
	}
	v = ngx_memcached_get_value(NGX_CACHE_REVERSAL_PREFIX_KEY, key, &len);
	if (v == NULL) {
		return NGX_ERROR;
	}
	if (ngx_deserialize_cache_from_json(datapool, v, crb) != NGX_OK) {
		ngx_free(v);
		return NGX_ERROR;
	}
	ngx_free(v);
	return NGX_OK;
}


ngx_int_t
ngxsave_notify_info_to_memcache(ngx_datapool_t *datapool, time_t expir)
{
	char key[39 + 1] = {0};
	char* json;

	if (ngx_generate_cache_key(datapool, ctkm, key, sizeof(key)) < 0) {
		return NGX_ERROR;
	}
	json = ngx_serialize_cache_to_json(datapool, cnb);
	if (json == NULL) {
		return NGX_ERROR;
	}
	if (ngx_memcached_set_value(NGX_CACHE_NOTIFY_PREFIX_KEY, key, json, strlen(json), expir) != NGX_OK) {
		ngx_free(json);
		return NGX_ERROR;
	}
	ngx_free(json);
	return NGX_OK;
}


ngx_int_t
ngx_load_notify_info_from_memcache(ngx_datapool_t *datapool)
{
	char key[39 + 1] = {0};
	ngx_int_t len = 0;
	char* v;

	if (ngx_generate_cache_key(datapool, ctkm, key, sizeof(key)) < 0) {
		return NGX_ERROR;
	}
	v = ngx_memcached_get_value(NGX_CACHE_NOTIFY_PREFIX_KEY, key, &len);
	if (v == NULL) {
		return NGX_ERROR;
	}
	if (ngx_deserialize_cache_from_json(datapool, v, cnb) != NGX_OK) {
		ngx_free(v);
		return NGX_ERROR;
	}
	ngx_free(v);
	return NGX_OK;
}


char*
ngx_serialize_message_attr_to_json(ngx_message_t* message)
{
	cJSON *root;
	char* out;
	char u[32 + 1] = {0};
	ngx_uuid_t uuid;

	root = cJSON_CreateObject();
	if (root == NULL) {
		return NULL;
	}
	cJSON_AddStringToObject(root, "reply_to", pn_message_get_reply_to(message->pmessage));
	ngx_get_message_uuid(message, &uuid);
	hex2asc(uuid.bytes, 32, 0, u);
	cJSON_AddStringToObject(root, "uuid", u);
	out = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	if (out == NULL) {
		return NULL;
	}
	return out;
}


ngx_int_t
ngx_deserialize_message_attr_from_json(ngx_message_t* message, char* json)
{
	cJSON *root, *reply_to, *u;
	ngx_uuid_t uuid;
	root=cJSON_Parse(json);
	if (root == NULL) {
		ErrorLog(ERROR, "parse json error");
		return NGX_ERROR;
	}
	reply_to = cJSON_GetObjectItem(root, "reply_to");
	if (reply_to == NULL) {
		ErrorLog(ERROR, "get reply_to attr from json error");
		cJSON_Delete(root);
		return NGX_ERROR;
	}
	u = cJSON_GetObjectItem(root, "uuid");
	if (u == NULL) {
		ErrorLog(ERROR, "get uuid attr from json error");
		cJSON_Delete(root);
		return NGX_ERROR;
	}
	pn_message_set_reply_to(message->pmessage, reply_to->valuestring);
	asc2hex(u->valuestring, 32, 0, uuid.bytes);
	ngx_set_message_uuid(message, &uuid);
	cJSON_Delete(root);
	return NGX_OK;
}


ngx_int_t
ngx_save_message_attr_to_memcache(char* key, ngx_message_t* message, time_t expir) {
	char* attr;
	attr = ngx_serialize_message_attr_to_json(message);
	if (attr == NULL) {
		return NGX_ERROR;
	}
	if (ngx_memcached_set_value(NGX_CACHE_MESSAGE_PREFIX_KEY, key, attr, strlen(attr), expir) != NGX_OK) {
		ngx_free(attr);
		return NGX_ERROR;
	}
	ngx_free(attr);
	return NGX_OK;
}


ngx_int_t
ngx_load_message_attr_from_memcache(char* key, ngx_message_t* message)
{
	char* value;
	ngx_int_t len;

	value = ngx_memcached_get_value(NGX_CACHE_MESSAGE_PREFIX_KEY, key, &len);
	if (value == NULL) {
		return NGX_ERROR;
	}
	if (ngx_deserialize_message_attr_from_json(message, value) != NGX_OK) {
		ngx_free(value);
		return NGX_ERROR;
	}
	ngx_free(value);
	return NGX_OK;
}


ngx_int_t
ngx_delete_message_attr_from_memcache(char* key)
{
	if (ngx_memcached_delete_key(NGX_CACHE_MESSAGE_PREFIX_KEY, key, 0) != NGX_OK) {
		return NGX_ERROR;
	}
	return NGX_OK;
}

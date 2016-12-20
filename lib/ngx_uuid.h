#ifndef _NGX_UUID_H_
#define _NGX_UUID_H_

#include "ngx_core.h"
#define NGX_UUID_LEN	16

struct ngx_uuid_s{
	u_char bytes[NGX_UUID_LEN];
};

ngx_int_t ngx_random_uuid(ngx_uuid_t* uuid);
ngx_int_t ngx_format_uuid(ngx_uuid_t* uuid, char* str);

#endif
#ifndef _NGX_HTTP_H_
#define _NGX_HTTP_H_


#include "ngx_core.h"

#define NGX_HTTP_GET_METHOD		"GET"
#define NGX_HTTP_POST_METHOD		"POST"
#define NGX_HTTP_PUT_METHOD		"PUT"

//http 通用头部
#define NGX_HTTP_COMMON_HEADER_CONNECTION		"Connection"
#define NGX_HTTP_COMMON_HEADER_CACHECONTROL		"Cache-Control"
#define NGX_HTTP_COMMON_HEADER_DATE				"Date"
//http 请求头部
#define NGX_HTTP_REQUEST_HEADER_ACCEPT			"Accept"
#define NGX_HTTP_REQUEST_HEADER_HOST			"Host"
#define NGX_HTTP_REQUEST_HEADER_USERAGENT		"User-Agent"
//http 响应头部
#define NGX_HTTP_RESPONE_HEADER_SERVER			"Server"
//http 实体头部
#define NGX_HTTP_BODY_HEADER_CONTENTLENGTH		"Content-Length"
#define NGX_HTTP_BODY_HEADER_CONTENTTYPE		"Content-Type"
#define NGX_HTTP_BODY_HEADER_TRANSFERENCODING  "Transfer-Encoding"


typedef struct ngx_httprequest_s ngx_httprequest_t;
typedef struct ngx_httprespone_s ngx_httprespone_t;

struct ngx_httprequest_s {
	const char* method;		//请求方法；
	char* arg;
	ngx_queue_t header;	//http头
	ngx_pool_t* pool;		//内存池
};

struct ngx_httprespone_s {
	char** header;
};

typedef struct ngx_httpheader_s {
	char* name;
	char* value;
	ngx_int_t vsize;		//存放value的空间大小
	ngx_queue_t node;
}ngx_httpheader_t;


ngx_httprequest_t* ngx_create_httprequest(ngx_pool_t* pool, const char* method, char* arg);
ngx_int_t ngx_httprequest_set_header(ngx_httprequest_t* req, const char* name, const char* value);
ngx_int_t ngx_httprequest_remove_header(ngx_httprequest_t* req, const char *name);
char* ngx_httprequest_get_header(ngx_httprequest_t* req, const char* name);
ngx_int_t ngx_httprequest_dump_header(ngx_httprequest_t* req);
ngx_int_t ngx_httprequest_send_header(ngx_int_t c, ngx_httprequest_t *req);
ngx_int_t ngx_httprequest_send_body(ngx_int_t c, char* body, ngx_int_t len);
ngx_httprespone_t* ngx_httprespone_new(ngx_pool_t *pool, char* header);
ngx_int_t ngx_httprespone_get_header(ngx_httprespone_t *resp, const char* name, char** begptr, char** endptr);
char* ngx_httprespone_read_header(ngx_int_t s, ngx_pool_t *pool);
char* ngx_httprespone_read_line(ngx_int_t s, ngx_pool_t *pool);
char* ngx_httprespne_read_body(ngx_int_t s, ngx_pool_t *pool, ngx_httprespone_t* resp, ngx_int_t sizeint);
ngx_int_t ngx_httprespone_status(ngx_httprespone_t *resp, char *message);
ngx_int_t ngx_httprespone_dump_header(ngx_httprespone_t *resp);



#endif
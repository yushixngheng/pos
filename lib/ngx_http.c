#include "ngx_core.h"

typedef char* (*http_terminator_t)(char* start, char* peeked, ngx_int_t peeklen);

static char* ngx_http_header_terminator(char* start, char* peeked, ngx_int_t peeklen);
static char* ngx_http_line_terminator(char* start, char* peeked, ngx_int_t peeklen);
static char* ngx_httprespone_read_chunk(ngx_int_t s, ngx_pool_t *pool, http_terminator_t terminator, ngx_int_t sizehint);
static ngx_int_t ngx_httprespone_header_locate(ngx_httprespone_t *resp, const char* name, ngx_int_t start, char** begptr, char** endptr);


ngx_httprequest_t*
ngx_create_httprequest(ngx_pool_t* pool, const char* method, char* arg)
{
	ngx_httprequest_t *request;

	request = ngx_pcalloc(pool, sizeof(ngx_httprequest_t));
	if (request == NULL) {
		return NULL;
	}
	request->method  = method;
	request->arg = arg;
	request->pool = pool;
	ngx_queue_init(&request->header);
	return request;
}


ngx_int_t
ngx_httprequest_set_header(ngx_httprequest_t* req, const char* name, const char* value)
{
	ngx_httpheader_t *hdr;
	ngx_queue_t *n;

	n = ngx_queue_next(&req->header);
	while(n != &req->header) {
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);
		if (hdr->name == name || strcmp(name, hdr->name) == 0) {
			if (hdr->vsize >= strlen(value) + 1) {
				memset(hdr->value, 0, hdr->vsize);
			}
			else {
				hdr->value = ngx_pcalloc(req->pool, strlen(value) + 1);
				if (hdr->value == NULL) {
					return NGX_ERROR;
				}
				hdr->vsize = strlen(value) + 1;
			}
			ngx_cpymem(hdr->value, value, strlen(value));
			return NGX_OK;
		}
		n = ngx_queue_next(n);
	}
	//还未存在对应的http头,则新增一个头部选项
	hdr = ngx_pcalloc(req->pool, sizeof(ngx_httpheader_t));
	if (hdr == NULL) {
		return NGX_ERROR;
	}
	hdr->name = (void*)name;
	hdr->value = ngx_pcalloc(req->pool, strlen(value) + 1);
	if (hdr->value == NULL) {
		return NGX_ERROR;
	}
	ngx_cpymem(hdr->value, value, strlen(value));
	hdr->vsize = strlen(value) + 1;
	ngx_queue_insert_tail(&req->header, &hdr->node);
	return NGX_OK;
}


ngx_int_t
ngx_httprequest_remove_header(ngx_httprequest_t* req, const char *name)
{
	ngx_httpheader_t *hdr;
	ngx_queue_t *n;

	n = ngx_queue_next(&req->header);
	while (n != &req->header) {
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);
		if (hdr->name == name || strcmp(name, hdr->name) == 0) {
			ngx_queue_remove(n);
			return NGX_OK;
		}
	}
	return NGX_OK;
}


char*
ngx_httprequest_get_header(ngx_httprequest_t* req, const char* name)
{
	ngx_httpheader_t *hdr;
	ngx_queue_t *n;

	n = ngx_queue_next(&req->header);
	while (n != &req->header) {
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);
		if (hdr->name == name || strcmp(name, hdr->name) == 0) {
			return hdr->value;
		}
		n = ngx_queue_next(n);
	}
	return NULL;
}


ngx_int_t
ngx_httprequest_dump_header(ngx_httprequest_t* req)
{
	ngx_int_t i, size;
	ngx_httpheader_t *hdr;
	ngx_queue_t *n;
	char *p, *request_header;

	//计算HTTP头部大小
	size = 0;
	size += strlen(req->method) + 1 + strlen(req->arg) + 1 + 8 + 2;	//8: HTTP/1.0 len 2:CRLF len

	n = ngx_queue_next(&req->header);
	while (n != &req->header) {	//遍历http头
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);
		size += strlen(hdr->name) + 2 + strlen(hdr->value) + 2;
		n = ngx_queue_next(n);
	}
	size += 3; //\r\n\0

	//生成http头部
	p = request_header = ngx_pcalloc(req->pool, size);
	if (p == NULL) {
		return NGX_ERROR;
	}
	p = ngx_cpymem(p, req->method, strlen(req->method)); *p++ = ' ';
	p = ngx_cpymem(p, req->arg, strlen(req->arg)); *p++ = ' ';
	p = ngx_cpymem(p, "HTTP/1.1\r\n", 10);

	n = ngx_queue_next(&req->header);
	while (n != &req->header) {
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);
		p = ngx_cpymem(p, hdr->name, strlen(hdr->name));
		*p++ = ':';
		*p++ = ' ';
		p = ngx_cpymem(p, hdr->value, strlen(hdr->value));
		*p++ = '\r';
		*p++ = '\n';
		n = ngx_queue_next(n);
	}
	*p++ = '\r', *p++ = '\n', *p++ = '\0';
	assert(p - request_header == size);
	ErrorLog(ERROR, "HTTP Header:\n%s", request_header);
	return NGX_OK;
}


ngx_int_t
ngx_httprequest_send_header(ngx_int_t c, ngx_httprequest_t *req)
{	ngx_int_t i, size;
	ngx_httpheader_t *hdr;
	ngx_queue_t *n;
	char *p, *request_header;

	//计算HTTP头部大小
	size = 0;
	size += strlen(req->method) + 1 + strlen(req->arg) + 1 + 8 + 2;	//8: HTTP/1.0 len 2:CRLF len

	n = ngx_queue_next(&req->header);
	while (n != &req->header) {	//遍历http头
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);
		size += strlen(hdr->name) + 2 + strlen(hdr->value) + 2;
		n = ngx_queue_next(n);
	}
	size += 3; //\r\n\0

	//生成http头部
	p = request_header = ngx_pcalloc(req->pool, size);
	if (p == NULL) {
		return NGX_ERROR;
	}
	p = ngx_cpymem(p, req->method, strlen(req->method)); *p++ = ' ';
	p = ngx_cpymem(p, req->arg, strlen(req->arg)); *p++ = ' ';
	p = ngx_cpymem(p, "HTTP/1.1\r\n", 10);

	n = ngx_queue_next(&req->header);
	while (n != &req->header) {
		hdr = ngx_queue_data(n, ngx_httpheader_t, node);

		p = ngx_cpymem(p, hdr->name, strlen(hdr->name));
		*p++ = ':';
		*p++ = ' ';
		p = ngx_cpymem(p, hdr->value, strlen(hdr->value));
		*p++ = '\r';
		*p++ = '\n';
		n = ngx_queue_next(n);
	}
	*p++ = '\r', *p++ = '\n', *p++ = '\0';
	assert(p - request_header == size);

	if (ngx_send(c, request_header, size - 1) != size - 1) {
		ErrorLog(ERROR, "send http request header error");	
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_httprequest_send_body(ngx_int_t c, char* body, ngx_int_t len)
{
	if (ngx_send(c, body, len) != len) {
		ErrorLog(ERROR, "send http request body error");
		return NGX_ERROR;
	}
	return NGX_OK;
}


//查找报文头终止符
static char*
ngx_http_header_terminator(char* start, char* peeked, ngx_int_t peeklen)
{
	char *p, *end;

	p = peeked - start < 2 ? start : peeked - 2;
	end = peeked + peeklen;

	for (; p < end - 2; p++) {
		if (*p == '\n') {
			if (p[1] == '\r' && p[2] == '\n') {
				return p + 3;
			}
			else if (p[1] == '\n') {
				return p + 2;
			}
		}
	}
	return NULL;
}


//查找换行符号
static char*
ngx_http_line_terminator(char* start, char* peeked, ngx_int_t peeklen)
{
	char *p, *end;

	p = peeked;
	end = peeked + peeklen;

	for (; p <  end ; p++) {
		if (*p == '\n') {
			return p + 1;
		}
	}
	return NULL;
}


static char*
ngx_httprespone_read_chunk(ngx_int_t s, ngx_pool_t *pool, http_terminator_t terminator, ngx_int_t sizehint)
{
	char* buf, *end;
	ngx_int_t pklen, rdlen, remain, tail = 0;
	ngx_int_t bufsize = sizehint;

	buf = ngx_pcalloc(pool, bufsize);
	if (buf == NULL) {
		return NULL;
	}
	while (1) {
		pklen = ngx_peek(s, buf + tail, bufsize - 1 - tail);	//这里的-1代表字符串结束符'\0', tail代表已经读取的大小
		if (pklen <= 0) {
			return NULL;
		}
		end = terminator(buf, buf + tail, pklen);
		//remain代表本次需要实际读取的大小
		if (end) {	//已经找到
			remain = end - (buf + tail);
			if (remain == 0) {
				buf[tail] = '\0';
				return buf;
			}
			if (bufsize - 1 < tail + remain) {	//缓冲空间大小不足
				buf = ngx_prealloc(pool, buf, bufsize, tail + remain + 1);
				if (buf == NULL) {
					return NULL;
				}
				bufsize = tail + remain + 1;
			}
		}
		else {		//没有找到http响应头终止符,则本次peek数据全部读取
			remain = pklen;
		}
		rdlen = ngx_recv(s, buf + tail, remain);	//这里返回的rdlen 不一定和上面的peeklen相等，实际读取数据以实际返回的rdlen为准
		if (rdlen <= 0) {
			return NULL;
		}
		tail += rdlen;
		buf[tail] = '\0';

		if (rdlen == 0) {
			if (tail == 0) {
				return NULL;
			}
			else {
				return buf;
			}
		}
		if (end && rdlen == remain) {	//找到header终止符且已经全部读取
			return buf;
		}
		if (tail == bufsize - 1) {	//缓冲已经读满
			buf = ngx_prealloc(pool, buf, bufsize, bufsize * 2);
			if (buf == NULL) {
				return NULL;
			}
			bufsize = bufsize * 2;
		}
	}

	return NULL;
}


//结构化响应头
ngx_httprespone_t* ngx_httprespone_new(ngx_pool_t *pool, char* header)
{
	ngx_httprespone_t *resp;
	char* hdr, *end;
	ngx_int_t count = 0;

	resp = ngx_pcalloc(pool, sizeof(ngx_httprespone_t));
	if (resp == NULL) {
		return NULL;
	}
	hdr = header;
	while (1) {
		resp->header = ngx_prealloc(pool, resp->header, count * sizeof(char*), (count + 1) * sizeof(char*));
		if (resp->header == NULL) {
			ErrorLog(ERROR, "reaccloc resp->header from pool error 1\n");
			return NULL;
		}
		resp->header[count] = hdr;
		if (hdr[0] == '\0' || (hdr[0] == '\r' && hdr[1] == '\n')) {	//结束符
			break;
		}
		do {
			end = strchr(hdr, '\n');
			if (end) {
				hdr = end + 1;
			}
			else {
				hdr = hdr + strlen(hdr);	
			}
		}
		while (*hdr == ' ' || *hdr == '\t');	//跳过空白开头的行
		count++;
	}
	count++;
	resp->header = ngx_prealloc(pool, resp->header, count * sizeof(char*), (count + 1) * sizeof(char*));
	if (resp->header == NULL) {
		ErrorLog(ERROR, "realloc resp->header from pool error 2\n");
		return NULL;
	}
	resp->header[count] = NULL;
	return resp;
}


static ngx_int_t
ngx_httprespone_header_locate(ngx_httprespone_t *resp, const char* name, ngx_int_t start, char** begptr, char** endptr)
{
	ngx_int_t name_len, i;
	char** headers = resp->header;
	char *b, *e;

	if (!headers || !headers[0]) {
		return NGX_ERROR;
	}

	name_len = strlen(name);
	if (start > 0) {
		i = start;
	}
	else {
		i = 1;
	}

	for (; headers[i + 1]; i++) {
		b = headers[i];
		e = headers[i + 1];
		if (e - b > name_len && b[name_len] == ':' && strncasecmp(b, name, name_len) == 0) {
			b = b + name_len + 1;
			while (b < e && isspace(*b))
				b++;
			while (b < e && isspace(e[-1]))
				e--;
			*begptr = b;
			*endptr = e;
			return i;
		}
	}
	return NGX_ERROR;
}


ngx_int_t
ngx_httprespone_get_header(ngx_httprespone_t *resp, const char* name, char** begptr, char** endptr)
{
	ngx_int_t pos;

	pos =ngx_httprespone_header_locate(resp, name, 0, begptr, endptr);
	if (pos == NGX_ERROR) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


char*
ngx_httprespone_read_header(ngx_int_t s, ngx_pool_t *pool)
{
	return ngx_httprespone_read_chunk(s, pool, ngx_http_header_terminator, 512);
}


char*
ngx_httprespone_read_line(ngx_int_t s, ngx_pool_t *pool)
{
	return ngx_httprespone_read_chunk(s, pool, ngx_http_line_terminator, 128);
}


char*
ngx_httprespne_read_body(ngx_int_t s, ngx_pool_t *pool, ngx_httprespone_t* resp, ngx_int_t sizeint)
{
	ngx_int_t clen, chunk, len, rdlen, llen, chunk_len;
	ngx_int_t bufsize;
	char *b, *e;
	char* line;
	char* buf;

	if (ngx_httprespone_get_header(resp, NGX_HTTP_BODY_HEADER_TRANSFERENCODING, &b, &e) == NGX_OK) {
		if (memcmp(b, "chunked", e - b) == 0) {
			clen = 0;	//标识接收方式为chunk

		}
		else {
			return NULL;
		}
		ErrorLog(ERROR, "接收方式为chunk");
	}
	else {
		if (ngx_httprespone_get_header(resp, NGX_HTTP_BODY_HEADER_CONTENTLENGTH, &b, &e) == NGX_OK) {
			clen = ngx_atoi(b, e - b);
			if (clen < 0) {
				return NULL;
			}
		}
		else {
			return NULL;//无法在http响应头部中获取chunked信息或者content-length信息
		}
		ErrorLog(ERROR, "接收方式为其他");
	}
	rdlen = 0;
	if (clen == 0) {	//chunked接收方式
		bufsize = sizeint;
		buf = ngx_pcalloc(pool, bufsize);
		if (buf == NULL) {
			return NULL;
		}
		while (1) {
			line = ngx_httprespone_read_line(s, pool);
			assert(line);
			len = strlen(line); 
			if (len <= 2) {
				break;
			}
			while (line[len] == '\0' || (len >= 0 && isspace(line[len]))) {
				len--;
			}
			len = len + 1;
			if (len <= 0) {
				break;
			}
			len = ngx_hatoi(line, len);
			if (len == 0) {
				//已经读取到最后一块
				ngx_httprespone_read_line(s, pool);	//读取的最后chunk的/r/n
				break;
			}
			chunk_len = 0; len = len + 2;
			do {
				line = ngx_httprespone_read_line(s, pool);
				assert(line);
				llen = strlen(line);
				if (llen <= 2) {
					chunk_len = chunk_len + llen;
					continue;
				}
				llen = llen - 2;	//去掉结尾的/r/n
				if (llen >= len) {
					llen = len;
				}
				if (rdlen + llen > bufsize - 1) {
					buf = ngx_prealloc(pool, buf, bufsize, rdlen + llen + bufsize * 2);
					if (buf == NULL) {
						return NULL;
					}
					bufsize = rdlen + llen + bufsize * 2;
				}
				ngx_cpymem(buf + rdlen, line, llen);
				rdlen = rdlen + llen;
				chunk_len = chunk_len + llen + 2;
			}
			while(chunk_len < len);
		}
	}
	else {	//按照content-length大小机型接收body
		bufsize = clen + 1;
		buf = ngx_pcalloc(pool, bufsize);
		if (buf == NULL) {
			return NULL;
		}
		if (ngx_recv(s, buf, clen) != clen) {
			ErrorLog(ERROR, "posp read http body error");
			return NULL;
		}
		rdlen += clen;
	}
	buf[rdlen] = '\0';
	return buf;
}


ngx_int_t
ngx_httprespone_status(ngx_httprespone_t *resp, char *message)
{
	char *b, *e;
	ngx_int_t status;

	if (!resp->header) {	//不存在报文头
		return NGX_ERROR;
	}
	b = resp->header[0];
	e = resp->header[1];
	if (b == NULL || e == NULL) {
		return NGX_ERROR;
	}

	if (e - b < 4 || strncmp(b, "HTTP", 4) != 0) {
		return -1;
	}
	b = b + 4;
	if (b < e && *b == '/') { //解析并跳HTTP版本号
		++b;
		while (b < e && isdigit(*b))
			++b;
		if (b < e && *b == '.')
			++b;
		while (b < e && isdigit(*b))
			++b;
	}
	while (b < e && isspace(*b))//跳过版本号后面的空格
		++b;
	//判断HTTP响应状态码部分格式是否合法, HTTP状态码固定3个数字字符
	if (e - b < 3 || !isdigit(b[0]) || !isdigit(b[1]) || !isdigit(b[2]))
		return NGX_ERROR;

	status = 100 * (b[0] - '0') + 10 * (b[1] - '0') + (b[2] - '0');
	b = b + 3;

	if (message) {	//获取HTTP 状态信息
		while (b < e && isspace(*b))
			++b;
		while (b < e && isspace(e[-1]))
			--e;
		ngx_cpymem(message, b, e - b);
	}
	return status;
}


ngx_int_t
ngx_httprespone_dump_header(ngx_httprespone_t *resp)
{
	ngx_int_t i, line_len;

	for (i = 0; resp->header[i + 1]; i++) {
		line_len = resp->header[i + 1] - resp->header[i];
		//ErrorLog(ERROR, "%d", line_len);
		//ErrorLog(ERROR, "http respone header:%s", resp->header[i]);
		ErrorLog(ERROR, "http respone header:%*.*s", line_len, line_len, resp->header[i]);
	}
	return NGX_OK;
}






























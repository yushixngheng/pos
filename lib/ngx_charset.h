#ifndef _NGX_CHARSET_H_
#define _NGX_CHARSET_H_

#include "ngx_core.h"

ngx_int_t ngx_uni2gbk(u_char *in, int ilen, u_char* out, int* olen);
ngx_int_t ngx_gbk2uni(u_char* in, int ilen, u_char* out, int* olen);

#endif
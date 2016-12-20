#ifndef _NGX_ISO8583_H_
#define _NGX_ISO8583_H_


#include "ngx_core.h"


typedef struct ngx_iso8583_s ngx_iso8583_t;
typedef struct ngx_iso8583_field_s ngx_iso8583_field_t;
typedef struct ngx_iso8583_subfield_s ngx_iso8583_subfield_t;
//打包回调函数定义
typedef ngx_int_t (*ngx_package_pack)(ngx_iso8583_t *iso, ngx_iso8583_field_t *this, const u_char* field, ngx_int_t fieldlen, u_char* outdata);
//解包回调函数定义
typedef ngx_int_t (*ngx_package_unpack)(ngx_iso8583_t *iso, ngx_iso8583_field_t *this, u_char* field, ngx_int_t *fieldlen, u_char* outdata);
//域长度设置回调函数定义
typedef void (*ngx_field_length_set) (ngx_iso8583_field_t *this, ngx_int_t len, u_char *outdata, ngx_int_t *count);
//域长度获取回调函数定义
typedef void (*ngx_field_length_get) (ngx_iso8583_field_t *this, u_char *indata, ngx_int_t *len, ngx_int_t *count);


//每个域的最大长度
#define NGX_ISO8583_MAX_FIELD_LEN 999
//报文最大长度
#define NGX_ISO8583_MAX_MSG_LEN 4096
//8583包的最大域数量
#define NGX_ISO8583_MAX_FIELD_NUM	128
//8583对齐类型
#define NGX_ISO8583_ALN_LEFT		0
#define NGX_ISO8583_ALN_RIGHT		1
//8583长度格式
#define NGX_ISO8583_LEN_BCD			0
#define NGX_ISO8583_LEN_ASCII		1
//8583域长度类型
#define NGX_ISO8583_LEN_FIX			0
#define NGX_ISO8583_LEN_LLVAR		1
#define NGX_ISO8583_LEN_LLLVAR		2

#define NGX_ISO8583_TYPE_ASCII		0
#define NGX_ISO8583_TYPE_BCD		1
#define NGX_ISO8583_TYPE_BINARY		3

//域内容格式
#define NGX_ISO8583_FIELD_ASC		0
#define NGX_ISO8583_FIELD_BCD		1
#define NGX_ISO8583_FIELD_BIN		2


struct ngx_iso8583_subfield_s {
	ngx_queue_t node;
	ngx_int_t 	 len;
	ngx_int_t   dict_code;
	ngx_int_t   seqno;
};

//ISO 8583 数据域定义
struct ngx_iso8583_field_s{
	ngx_int_t len;
	ngx_int_t len_type;
	ngx_int_t align;
	char padchar;
	ngx_package_pack pack;
	ngx_package_unpack unpack;
	ngx_int_t dict_code;			//该域所对应字典
	ngx_int_t sub;					//是否包含子域配置
	ngx_queue_t *subfield;			//子域配置链表
};

typedef struct {
   ngx_int_t    len;
   ngx_int_t    off;
}ngx_iso8583_field_def_t;

struct ngx_iso8583_s{
	ngx_iso8583_field_t *fld;
	ngx_iso8583_field_def_t *flddef;
	char bitmap[128 + 1];				//bitmap[0]:用来指示位图为64位位图还是128位位图
	u_char databuf[NGX_ISO8583_MAX_MSG_LEN + NGX_ISO8583_MAX_FIELD_LEN];
	ngx_int_t used;
	ngx_int_t maxfield;
    ngx_field_length_set	set;	 /* 设置长度时调用的函数 */
    ngx_field_length_get	get;     /* 取出长度时调用的函数 */
	ngx_pool_t* pool;
};


ngx_iso8583_t *ngx_create_iso8583(ngx_pool_t* pool, ngx_int_t num);
ngx_int_t ngx_iso8583_del_one_bit(ngx_iso8583_t* iso, ngx_int_t pos);
ngx_int_t ngx_iso8583_del_all_bit(ngx_iso8583_t* iso);
ngx_int_t ngx_iso8583_set_field(ngx_iso8583_t* iso, ngx_iso8583_field_t* fld, ngx_int_t pos);
ngx_int_t ngx_iso8583_get_field(ngx_iso8583_t* iso, ngx_iso8583_field_t* fld, ngx_int_t pos);
ngx_int_t ngx_iso8583_get_dict_code(ngx_iso8583_t* iso, ngx_int_t pos);
ngx_int_t ngx_iso8583_set_bit(ngx_iso8583_t* iso, ngx_int_t pos, char *bytes, ngx_int_t len);
ngx_int_t ngx_iso8583_get_bit(ngx_iso8583_t* iso, ngx_int_t pos, char *str, ngx_int_t *len);
ngx_int_t ngx_iso8583_to_str(char *str, ngx_iso8583_t *iso, ngx_int_t *len);
ngx_int_t ngx_iso8583_from_str(const char *str, ngx_iso8583_t *iso, ngx_int_t *pos);
ngx_int_t ngx_iso8583_set_field_len_format(ngx_iso8583_t *iso, ngx_int_t len_format);
void ngx_iso8583_set_bcd_length(ngx_iso8583_field_t *this, ngx_int_t len, u_char* outdata, ngx_int_t *count);
void ngx_iso8583_get_bcd_length(ngx_iso8583_field_t * this, u_char *indata, ngx_int_t *len, ngx_int_t *count);
void ngx_iso8583_set_asc_length(ngx_iso8583_field_t *this, ngx_int_t len, u_char *outdata, ngx_int_t *count);
void ngx_iso8583_get_asc_length(ngx_iso8583_field_t * this, u_char *indata, ngx_int_t *len, ngx_int_t *count);
ngx_int_t ngx_iso8583_binary_pack(ngx_iso8583_t *iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata);
ngx_int_t ngx_iso8583_binary_unpack(ngx_iso8583_t *iso, ngx_iso8583_field_t * this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata);
ngx_int_t ngx_iso8583_bcd_msgid_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata);
ngx_int_t ngx_iso8583_bcd_msgid_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata);
ngx_int_t ngx_iso8583_asc_msgid_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata);
ngx_int_t ngx_iso8583_asc_msgid_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata);
ngx_int_t ngx_iso8583_bcd_bitmap_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata);
ngx_int_t ngx_iso8583_bcd_bitmap_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, u_char *fieldbuf, ngx_int_t *fieldlen,  u_char *indata);
ngx_int_t ngx_iso8583_asc_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata);
ngx_int_t ngx_iso8583_asc_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata);
ngx_int_t ngx_iso8583_bcd_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata);
ngx_int_t ngx_iso8583_bcd_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata);


#endif
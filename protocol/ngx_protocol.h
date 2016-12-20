#ifndef _NGH_PROTOCOL_H_
#define _NGX_PROTOCOL_H_



#define NGX_PROTOCOL_LEN_FIX			0
#define NGX_PROTOCOL_LEN_LL				1
#define NGX_PROTOCOL_LEN_LLL			2

#define NGX_PROTOCOL_FIELD_ASC			0
#define NGX_PROTOCOL_FIELD_BCD			1
#define NGX_PROTOCOL_FIELD_BINARY		2

#define NGX_PROTOCOL_ALIGN_LEFT		0
#define NGX_PROTOCOL_ALIGN_RIGHT		1


#define NGX_TRADE_ATTR_MANAGE			0x00000001		//管理类交易
#define NGX_TRADE_ATTR_ONLINE			0x00000002		//联机类交易
#define NGX_TRADE_ATTR_REVERSAL			0x00000004		//冲正类交易
#define NGX_TRADE_ATTR_NOTIFY			0x00000010		//通知类交易	(脚本上送)
#define NGX_TRADE_ATTR_BM				0x00000020		//便民类交易
#define NGX_TRADE_ATTR_ZZ				0x00000040 		//增值类交易
#define NGX_TRADE_ATTR_VA 				0x00000080		//账户类交易

#define NGX_TRADE_DIRECTION_POSITIVE	0x00000001		//正交易
#define NGX_TRADE_DIRECTION_NEGATIVE	0x00000002		//反交易

#define ngx_trade_online(attr) 		((attr) & NGX_TRADE_ATTR_ONLINE)
#define ngx_trade_manage(attr) 		((attr) & NGX_TRADE_ATTR_MANAGE)
#define ngx_trade_reversal(attr) 	((attr) & NGX_TRADE_ATTR_REVERSAL)
#define ngx_trade_notify(attr)		((attr) & NGX_TRADE_ATTR_NOTIFY)
#define ngx_trade_zz(attr) 			((attr) & NGX_TRADE_ATTR_ZZ)
#define ngx_trade_va(attr)			((attr) & NGX_TRADE_ATTR_VA)
#define ngx_trade_bm(attr)			((attr) & NGX_TRADE_ATTR_BM)

#define ngx_trade_positive(attr) ((attr >> 8) & NGX_TRADE_DIRECTION_POSITIVE)
#define ngx_trade_negative(attr) ((attr >> 8) & NGX_TRADE_DIRECTION_NEGATIVE)

#define NGX_BITMAP_FROM		0
#define NGX_BITMAP_TO			1


typedef struct {
	ngx_int_t dict_code;
	ngx_int_t len;
	ngx_int_t seqno;
	ngx_int_t type;
	ngx_int_t len_type;
	ngx_queue_t node;
}ngx_protocol_header_t;

typedef struct {
	char type[5 + 1];
	char bitmap_to[128 + 1];
	char bitmap_from[128 + 1];
	ngx_array_t*  dict_set;
	ngx_array_t*  value_set;
	int attr;
}ngx_trade_conf_t;

typedef struct ngx_protocol_s {
	char code[4 + 1];
	ngx_int_t header_flag;
	ngx_int_t iso_flag;
	ngx_int_t field_num;
	ngx_queue_t *header;
	ngx_iso8583_t *iso;
	ngx_pool_t *pool;
}ngx_protocol_t;

typedef struct {
	char* type;
	char* ori_type;
}ngx_tarde_pairs_t;

#define NGX_TCP_HEADER_LEN		2

//---ngx_protocol_t* ngx_query_protocol(ngx_pool_t *pool, char* protocolcode);
ngx_protocol_t* ngx_copy_protocol(ngx_pool_t *pool, ngx_protocol_t* protocol);
//---ngx_int_t ngx_protocol_query_info(ngx_protocol_t* protocol);
ngx_int_t ngx_protocol_pack_header(ngx_datapool_t *datapool, ngx_protocol_t *protocol, char* data);
const char* ngx_protocol_unpack_header(ngx_datapool_t *datapool, ngx_protocol_t* protocol, const char* data);
//---ngx_queue_t* ngx_protocol_query_header_conf(ngx_pool_t *pool, char* protocolcode);
//---ngx_iso8583_t* ngx_protocol_query_iso8583_conf(ngx_pool_t *pool, char* protocolcode, ngx_int_t fieldnum);
//---ngx_queue_t* ngx_protocol_query_subfield_conf(ngx_pool_t *pool, char* protocolcode, int field_code);
const char* ngx_protocol_unpack_iso8583(ngx_datapool_t* datapool, ngx_iso8583_t *iso, const char* data);
ngx_int_t ngx_protocol_unpack_subfield(ngx_datapool_t *datapool, ngx_queue_t* subfield, const char* data, ngx_int_t datalen);
ngx_int_t ngx_protocol_pack_iso8583(ngx_datapool_t *datapool, ngx_protocol_t *protocol, char* bitmap, char* data);
ngx_int_t ngx_protocol_pack_subfield(ngx_datapool_t* datapool, ngx_queue_t *subfield, char* data);
ngx_int_t ngx_protocol_pack_msgbody(ngx_datapool_t* datapool, char* data);
const char* ngx_protocol_unpack_msgbody(ngx_datapool_t* datapool, const char* data, ngx_int_t datalen);
ngx_trade_conf_t* ngx_query_trade_conf(ngx_pool_t* pool, char* protocolcode, char* trade_type);
ngx_trade_conf_t* ngx_parse_trade_conf(ngx_pool_t *pool, ngx_datapool_t* datapool, ngx_protocol_t* protocol, char mode);
ngx_int_t ngx_check_iso8583_bitmap(char* bitmap, char* bc);
void ngx_process_msg_type(ngx_datapool_t* datapool);
void ngx_swap_tpdu(char* tpdu);
ngx_int_t ngx_generate_settle_date(ngx_datapool_t *datapool, char* clear_time);
ngx_int_t ngx_get_cardcode_from_track2(char* track2, char* card_code);
char* ngx_generate_syscode_into_datapool(ngx_datapool_t *datapool);
char* ngx_generate_ori_syscode_into_datapool(ngx_datapool_t *datapool);
ngx_int_t ngx_check_ori_trade_type(char* type, char* ori_type);
ngx_int_t ngx_check_reversal_trade_type(char* type, char* reversal_type);
ngx_int_t ngx_generate_refcode_into_datapool(ngx_datapool_t *datapool);
ngx_int_t ngx_generate_bitmap_config_str(char* bitmap, ngx_int_t bitmaplen, char* bstr);
ngx_int_t ngx_set_bitmap_config_str_bit(char* bstr, ngx_int_t bit, char flag);
void ngx_disp_datapool(ngx_datapool_t* datapool);
#endif

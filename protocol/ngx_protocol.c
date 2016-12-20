#include "ngx_core.h"
#include "cJSON.h"
#include "ngx_protocol.h"



ngx_protocol_t*
ngx_copy_protocol(ngx_pool_t *pool, ngx_protocol_t* protocol)
{
	ngx_protocol_t *new;

	new = ngx_pcalloc(pool, sizeof(ngx_protocol_t));
	if (new == NULL) {
		return NULL;
	}
	ngx_cpymem(new->code, protocol->code, sizeof(protocol->code) - 1);
	new->header_flag = protocol->header_flag;
	new->iso_flag = protocol->iso_flag;
	new->field_num = protocol->field_num;
	new->header = protocol->header;				//配置链表沿用原protocol结构内队列地址
	new->iso = ngx_pcalloc(pool, sizeof(ngx_iso8583_t));
	if (new->iso == NULL) {
		return NULL;
	}
	ngx_cpymem(new->iso, protocol->iso, sizeof(ngx_iso8583_t));
	new->iso->flddef = ngx_pcalloc(pool, sizeof(ngx_iso8583_field_def_t) * (new->iso->maxfield));
	if (new->iso->flddef == NULL) {
		return NULL;
	}
	new->iso->pool = pool;
	new->pool = pool;
	return new;
}

ngx_int_t
ngx_protocol_pack_header(ngx_datapool_t *datapool, ngx_protocol_t *protocol, char* data)
{
	char* p, *value;
	ngx_queue_t* q, *h;
	ngx_protocol_header_t* header;
	ngx_int_t len, len1;

	h = q = protocol->header;
	p = data;
	while (ngx_queue_next(q) != h) {		//循环队列
		q = ngx_queue_next(q);
		header = ngx_queue_data(q, ngx_protocol_header_t, node);
		value = ngx_get_buf_from_datapool(datapool,  header->dict_code, &len);
		if (value == NULL) {
			return NGX_ERROR;
		}
		if (header->len_type == NGX_PROTOCOL_LEN_FIX) {
		}
		else if (header->len_type == NGX_PROTOCOL_LEN_LL) {
			if (byte2bcd(len, p) != NGX_OK) {
				return NGX_ERROR;
			}
			p = p + 1;
		}
		else {
			if (int2bcd(p, len) != NGX_OK) {
				return NGX_ERROR;
			}
			p = p + 2;
		}
		if (header->type == NGX_PROTOCOL_FIELD_ASC) {
			memcpy(p, value, len);
			p = p + len;
		}
		else if (header->type == NGX_PROTOCOL_FIELD_BCD) {
			len1 = ((len % 2) == 0) ? len /2 : len / 2 + 1;
			if (asc2hex(value, len, 0, p) != NGX_OK) {
				return NGX_ERROR;
			}
			p = p + len1;
		}
		else {	//BINARY字段类型
			memcpy(p, value, len);
			p = p + len;
		}
	}
	return p - data;
}


const char*
ngx_protocol_unpack_header(ngx_datapool_t *datapool, ngx_protocol_t* protocol, const char* data)
{
	const char* p = data;
	ngx_queue_t* q, *h;
	ngx_protocol_header_t* header;
	int len, len1;
	char value[999];

	h = q = protocol->header;
	while (ngx_queue_next(q) != h) {		//循环队列
		q = ngx_queue_next(q);
		header = ngx_queue_data(q, ngx_protocol_header_t, node);
		memset(value, 0, sizeof(value));
		if (header->len_type == NGX_PROTOCOL_LEN_FIX) {
				len = header->len;
			}
			else if (header->len_type == NGX_PROTOCOL_LEN_LL) {
				if (bcd2byte(p[0], &len) != NGX_OK) {
					return NULL;
				}
				p = p + 1;
			}
			else {	//LLL 长度类型
				//计算长度
				if (bcd2int(p, &len) != NGX_OK) {
					return NULL;
				}
				p = p + 2;
			}
			//解析字段内容
			if (header->type == NGX_PROTOCOL_FIELD_ASC) {
				memcpy(value, p, len);
				p = p + len;
			}
			else if (header->type == NGX_PROTOCOL_FIELD_BCD) {
				len1 = ((len % 2) == 0) ? len /2 : len / 2 + 1;
				if (hex2asc(p, len1 * 2, 0, value) != NGX_OK) {
					return NULL;
				}
				p = p + len1;
			}
			else {	//BINARY字段类型
				memcpy(value, p, len);
				p = p + len;
			}
			//字段保存到数据池
			if (ngx_add_buf_to_datapool(datapool, header->dict_code, value, len) != NGX_OK) {
				return NULL;
			}
		}
		return p;
}



const char*
ngx_protocol_unpack_iso8583(ngx_datapool_t* datapool, ngx_iso8583_t *iso, const char* data)
{
	const char* p = data;
	ngx_int_t bitmaplen;
	ngx_int_t i, j, n, len, unpacklen;
	ngx_int_t dict_code, field_code;
	ngx_iso8583_field_t field;
	char msgtype[4 + 1] = {0};
	u_char bitmask;
	u_char value[999];

	//---ngx_iso8583_get_field(protocol->iso, &field, 0);
	ngx_iso8583_get_field(iso, &field, 0);
	if (field.unpack == ngx_iso8583_asc_msgid_unpack) {
		ngx_cpymem(msgtype, p, 4);
		len = 4;
	}
	else {
		hex2asc(p, 4, 0, msgtype);
		len = 2;
	}
	ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_MSG_TYPE, msgtype, strlen(msgtype));
	unpacklen = ngx_iso8583_from_str(data, iso, &field_code);
	if (unpacklen <= 0) {
		ErrorLog(ERROR, "unpack iso8583 message error[%d]", field_code);
		return NULL;
	}
	//获取位图
	p = p + len;
	if (p[0] & 0x80) {
		bitmaplen = 16;
	}
	else {
		bitmaplen = 8;
	}
	//添加位图信息到数据池
	if (ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_BITMAP, p, bitmaplen) != NGX_OK) {
		return NULL;
	}
	char* temp;
	//解析8583包体
	n = 0;
	for (i = 0; i < bitmaplen; i++) {
		bitmask = 0x80 ;
		for(j=1; j<9; j++, bitmask>>=1) {
			if ((p[i] & bitmask) == 0)
				continue ;
			n = (i<<3) + j;
			if (n == 1) {	//continue;
				continue;
			}
			memset(value, 0, sizeof(value));
			//获取对应域的数据
			if (ngx_iso8583_get_bit(iso, n, value, &len) != NGX_OK) {
				ErrorLog(ERROR, "get iso8583 field[%d] error", n);
				return NULL;
			}
			if (ngx_add_buf_to_datapool(datapool, n, value, len) != NGX_OK) {
				ErrorLog(ERROR, "add field[%d] to datapool[%d] error", n, dict_code);
				return NULL;
			}
		}
	}
	return data + unpacklen;
}


ngx_int_t
ngx_protocol_unpack_subfield(ngx_datapool_t *datapool, ngx_queue_t* subfield, const char* data, ngx_int_t datalen)
{
	const char* p = data;
	ngx_queue_t* q;
	ngx_iso8583_subfield_t* sub;
	int len, len1;
	char value[999];

	q = subfield;
	while (ngx_queue_next(q) != subfield) {		//循环队列
		q = ngx_queue_next(q);
		sub = ngx_queue_data(q, ngx_iso8583_subfield_t, node);
		memset(value, 0, sizeof(value));
		len = sub->len;
		memcpy(value, p, len);
		p = p + len;
		//字段保存到数据池
		if (ngx_add_buf_to_datapool(datapool, sub->dict_code, value, len) != NGX_OK) {
			return NGX_ERROR;
		}
		if (p == data + datalen) {		//由于实际数据可能短于协议配置，所以这里还要判断是否已经对数据分析结束
			break;
		}
	}
	return NGX_OK;
}


ngx_int_t ngx_protocol_pack_iso8583(ngx_datapool_t *datapool, ngx_protocol_t *protocol, char* bitmap, char* data)
{
	ngx_int_t i, field_no, len;
	ngx_iso8583_field_t field;
	char tmp[999];
	char* value;

	ngx_iso8583_del_all_bit(protocol->iso);
	for (i = 0; i < strlen(bitmap); i++) {
		len = 0;


		if (i == 0) {
			value = ngx_get_buf_from_datapool(datapool, i, &len);
			if (ngx_iso8583_set_bit(protocol->iso, i, value, len) != NGX_OK) {
				ErrorLog(ERROR, "set field [%d] error", i);
				return NGX_ERROR;
			}
			field_no = i;
		}
		else {
			//从这里开始位图bitmap 1 代表第2域，后续同理
			if (ngx_iso8583_get_field(protocol->iso, &field, i + 1) != NGX_OK) {
				continue;
			}
			if (field.sub) {	//存在子域配置，则以子域配置为准
				memset(tmp, 0, sizeof(tmp));
				len = ngx_protocol_pack_subfield(datapool, field.subfield, tmp);
				if (len < 0) {
					ErrorLog(ERROR, "format [%d]‘s subfield error", i + 1);
					return NGX_ERROR;
				}
				value = tmp;
			}
			else {
				value = ngx_get_buf_from_datapool(datapool, field.dict_code, &len);
			}
			field_no = i + 1;
		}
		if (bitmap[i] == 'M' || i == 0) {	//第0域(消息类型)并填
			if (value == NULL || len <= 0) {
				ErrorLog(ERROR, "field [%d] of bitmap do not exist in datapool [%d]", i + 1, field.dict_code);
				return NGX_ERROR;
			}
			if (ngx_iso8583_set_bit(protocol->iso, field_no, value, len) != NGX_OK) {
				ErrorLog(ERROR, "set field[%d] dict_code[%d] len[%d] [%s] error", i + 1, field.dict_code, len, value);
				return NGX_ERROR;
			}

		}
		else if (bitmap[i] == 'C') {

			if (value == NULL || len <= 0) {
				continue;
			}
			if (ngx_iso8583_set_bit(protocol->iso, field_no, value, len) != NGX_OK) {
				ErrorLog(ERROR, "set field [%d] error", i + 1);
				return NGX_ERROR;
			}
		}
	}
	if (ngx_iso8583_to_str(data, protocol->iso, &len) != NGX_OK) {
		ErrorLog(ERROR, "switch iso8583 to buffer error");
		return NGX_ERROR;
	}
	return len;
}


ngx_int_t
ngx_protocol_pack_subfield(ngx_datapool_t* datapool, ngx_queue_t *subfield, char* data)
{
	ngx_queue_t* q;
	ngx_int_t len, len1;
	char* p, *value;
	ngx_iso8583_subfield_t* sub;
	q = subfield;
	p = data;
	while (ngx_queue_next(q) != subfield) {		//循环队列
		q = ngx_queue_next(q);
		sub = ngx_queue_data(q, ngx_iso8583_subfield_t, node);
		len = 0;
		value = ngx_get_buf_from_datapool(datapool, sub->dict_code, &len);
		if (value == NULL || len <= 0) {
			continue;
		}
		memcpy(p , value, len);
		p = p + len;

	};
	return p - data;
}


ngx_int_t
ngx_protocol_pack_msgbody(ngx_datapool_t* datapool, char* data)
{
	char* field;
	ngx_int_t len = 0;

	field = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_MSG_BODY, &len);
	if (field != NULL) {
		memcpy(data, field, len);
	}
	return len;
}

const char*
ngx_protocol_unpack_msgbody(ngx_datapool_t* datapool, const char* data, ngx_int_t datalen)
{

	if (ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_MSG_BODY, data, datalen) != NGX_OK) {
		return NULL;
	}
	return data + datalen;
}




ngx_int_t
ngx_check_iso8583_bitmap(char* bitmap, char* bc)
{
	printf("begin\n");
	printf("%s\n", bitmap);
	printf("%s\n", bc);
	ngx_int_t i, j, n;
	u_char bitmask;
	for (i = 0; i < strlen(bc); i++) {	//判断POS上送的报文位图
		if (i == 0 ) {
			continue;
		}
		j = i / 8;
		n = i % 8;
		bitmask = 0x80 >> n ;
		printf("bc[%d]", i+1, bc[i]);
		if ( bc[i] == 'O' ||
			(bc[i] == 'M' && (bitmap[j] & bitmask)) ||
			bc[i] == 'C'
		) {
			continue;
		}
		ErrorLog(ERROR, "bitmap[%d] [%c] bitmask:[0x%02x] bitmap[%d]:[0x%02x] not match", i+1, bc[i], bitmask, j, bitmap[j]);
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_get_cardcode_from_track2(char* track2, char* card_code)
{
	ngx_int_t i;

	for(i=0; i<37; i++) {
		if (('D' == *(track2+i))||('d' == *(track2+i))||('=' == *(track2+i))||('>' == *(track2+i))) {
			break;
		}
	}
	if (i>19) {
		return NGX_ERROR;
	}
	memcpy(card_code,track2,i);
	return NGX_OK;
}


void
ngx_swap_tpdu(char* tpdu)
{
	char tmp[4] = {0};

	ngx_cpymem(tmp, tpdu + 2, 4);
	ngx_cpymem(tpdu + 2, tpdu + 6, 4);
	ngx_cpymem(tpdu + 6, tmp, 4);
}


void
ngx_process_msg_type(ngx_datapool_t* datapool)
{
	char* m;

	m = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_MSG_TYPE, NULL);
	sprintf(m, "%04d", ngx_atoi(m, strlen(m)) + 10);
}


char*
ngx_generate_syscode_into_datapool(ngx_datapool_t *datapool)
{
	char *mht_code, *term_code, *trade_water, *batch_code, *sys_date;
	char syscode[39 + 1] = {0};

	mht_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_MERCHANT_CODE, NULL);
	term_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERMINAL_CODE, NULL);
	trade_water = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERM_WATER, NULL);
	batch_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_BATCH_CODE, NULL);
	sys_date = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SYS_DATE, NULL);

	if (mht_code == NULL || term_code == NULL || trade_water == NULL || batch_code == NULL) {
		return NULL;
	}
	sprintf(syscode, "%s%s%s%s%4.4s", mht_code, term_code, trade_water, batch_code, sys_date);
	if (ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_SYS_CODE, syscode, strlen(syscode)) != NGX_OK) {
		return NULL;
	}
	ErrorLog(ERROR, "syscode:[%s]", ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SYS_CODE, NULL));
	return ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SYS_CODE, NULL);
}


char*
ngx_generate_ori_syscode_into_datapool(ngx_datapool_t *datapool)
{
	char *mht_code, *term_code, *ori_trade_water, *ori_batch_code, *ori_date;
	char ori_syscode[39 + 1] = {0};

	mht_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_MERCHANT_CODE, NULL);
	term_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERMINAL_CODE, NULL);
	ori_trade_water = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_ORI_TERM_WATER, NULL);
	ori_batch_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_ORI_BATCH_CODE, NULL);
	ori_date = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_ORI_SYS_DATE, NULL);
	if (ori_date == NULL) {	//终端上送原交易日期
		ori_date = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SYS_DATE, NULL);
	}

	if (mht_code == NULL || term_code == NULL || ori_trade_water == NULL || ori_batch_code == NULL) {
		return NULL;
	}
	sprintf(ori_syscode, "%s%s%s%s%4.4s", mht_code, term_code, ori_trade_water, ori_batch_code, ori_date);
	if (ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_ORI_SYS_CODE, ori_syscode, strlen(ori_syscode)) != NGX_OK) {

		return NULL;
	}
	ErrorLog(ERROR, "ori syscode:[%s]", ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_ORI_SYS_CODE, NULL));
	return ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_ORI_SYS_CODE, NULL);
}




ngx_int_t
ngx_generate_settle_date(ngx_datapool_t *datapool, char* clear_time)
{
	char date_time[14 + 1] = {0};
	char settle_date[8 + 1] = {0};

	ngx_get_datetime(date_time);
	if (memcmp(date_time + 8, clear_time, 4) > 0) {
		//当前日期加一
		nowdate_add_day(1, settle_date);
	}
	else {
		ngx_cpymem(settle_date, date_time, 8);
	}
	ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_SETTLE_DATE, settle_date + 4, 4);
	return NGX_OK;
}


ngx_int_t
ngx_generate_refcode_into_datapool(ngx_datapool_t *datapool)
{
	char* v;
	char rc[12 + 1] = {0};

	v = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERM_WATER, NULL);
	ngx_cpymem(rc, v, 6);
	v = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_CHANNEL_TERM_WATER, NULL);
	ngx_cpymem(rc + 6, v, 6);
	if (ngx_add_buf_to_datapool(datapool, NGX_DATAPOOL_REF_CODE, rc, strlen(rc)) != NGX_OK)
	{
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_generate_bitmap_config_str(char* bitmap, ngx_int_t bitmaplen, char* bstr)
{
	ngx_int_t i, j, n;
	u_char bitmask;

	if (bitmaplen == 8) {	//8字节位图
		bstr[0] = 'O';
	}
	else {					//16字节位图
		bstr[0] = 'M';
	}

	for (i = 0; i < bitmaplen; i++) {
		bitmask = 0x80 ;
		for(j=1; j<9; j++, bitmask>>=1) {
			n = (i<<3) + j;
			if (n == 1) {
				continue;
			}
			if ((bitmap[i] & bitmask) == 0) {
				bstr[n - 1] = 'O';
			}
			else {	//p[i] & bitmask == 1
				bstr[n - 1] = 'M';
			}
		}
	}
	return NGX_OK;
}


ngx_int_t
ngx_set_bitmap_config_str_bit(char* bstr, ngx_int_t bit, char flag)
{

	if (bit <= 0 ||  bit > strlen(bstr)) {
		return NGX_ERROR;
	}
	bstr[bit - 1] = flag; 
	ErrorLog(ERROR, "bstr len:%d flag:%c, bstr[%d-1]:%c", strlen(bstr), flag, bit, bstr[bit-1]);
	return NGX_OK;
}



void
ngx_disp_datapool(ngx_datapool_t* datapool)
{
	int i;
	const u_char* buf;
	ngx_int_t len;
	ngx_datapool_field_t *field;
	for (i = 0; i < datapool->nalloc; i++) {
		field = (ngx_datapool_field_t*)(datapool->elts + i * datapool->size);
		if (field->field_no == NGX_DATAPOOL_MSG_BODY) {
			continue;
		}
		if (field->used != NGX_DATAPOOL_UNUSED) {
			switch (field->field_type) {
			case NGX_DATAPOOL_INT:
				ErrorLog(INFO, "[dict_code:%03d]:%d", field->field_no, field->value.i);
				break;
			case NGX_DATAPOOL_LONG:
				ErrorLog(INFO, "[dict_code:%03d]:%d", field->field_no, field->value.l);
				break;
			case NGX_DATAPOOL_BUF:
				buf = ngx_get_buf_from_datapool(datapool, field->field_no, &len);
				if (buf != NULL) {
					ErrorLog(INFO, "[dict_code:%03d]:%s", field->field_no, buf);
				}
				break;
			default:
				continue;
				break;
			}
		}
	}
}



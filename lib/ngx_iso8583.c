#include "ngx_core.h"

static ngx_int_t ngx_iso8583_add_pad_char(ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata, u_char head);

ngx_iso8583_t *
ngx_create_iso8583(ngx_pool_t* pool, ngx_int_t num)
{
	ngx_iso8583_t *iso;

    if (num < 0 || num > NGX_ISO8583_MAX_FIELD_NUM) {
        return NULL;
    }

    iso = ngx_pcalloc(pool, sizeof(ngx_iso8583_t));
	if (iso == NULL) {
		return NULL;
	}
	iso->pool = pool;
    num = num + 1;
    iso->fld = ngx_pcalloc(pool, sizeof(ngx_iso8583_field_t) * num);
    if (iso->fld == NULL) {
        return NULL;
    }
    iso->flddef = ngx_pcalloc(pool, sizeof(ngx_iso8583_field_def_t) * num);
    if (iso->flddef == NULL) {
        return NULL;
    }
	//默认域长度以bcd码格式进行处理
    iso->get = ngx_iso8583_get_bcd_length;
    iso->set = ngx_iso8583_set_bcd_length;
    iso->maxfield = num;
	return iso;
}


ngx_int_t
ngx_iso8583_del_one_bit(ngx_iso8583_t* iso, ngx_int_t pos)
{
    ngx_int_t i;

    if (pos < 0 || pos >= iso->maxfield) {
        ErrorLog(ERROR, "exceed max field num");
        return NGX_ERROR;
    }
    if (pos > 1) { //避免无法设置128域
		iso->bitmap[pos-1] = 0;
	}
    if (iso->flddef[pos].off != 0 && iso->flddef[pos].len != 0) {
        /* move  forward all the field data in iso->szDatabuf*/
        for (i = 0; i < NGX_ISO8583_MAX_FIELD_NUM; i++) {
            if ((iso->flddef[i].len > 0) && (iso->flddef[i].off > iso->flddef[pos].off)) {
                iso->flddef[i].off -= iso->flddef[pos].len;
			}
        }
        iso->used -= iso->flddef[pos].len;
        for (i = iso->flddef[pos].off; i < iso->used; i++) {
            iso->databuf[i] = iso->databuf[i + iso->flddef[pos].len];
		}
    }
    iso->flddef[pos].off = 0;
    iso->flddef[pos].len = 0;
    return NGX_OK;
}


ngx_int_t
ngx_iso8583_del_all_bit(ngx_iso8583_t* iso)
{
	ngx_int_t i;

    for (i = 0; i < iso->maxfield; i++) {
        iso->bitmap[i] = 0;
        iso->flddef[i].off = 0;
        iso->flddef[i].len = 0;
    }
    memset(iso->databuf,0,NGX_ISO8583_MAX_MSG_LEN + NGX_ISO8583_MAX_FIELD_LEN);
    iso->used = 0;
    return NGX_OK;
}


ngx_int_t
ngx_iso8583_get_dict_code(ngx_iso8583_t* iso, ngx_int_t pos)
{
    if (pos < 0 || pos >= iso->maxfield) {
        return NGX_ERROR;
    }
    if ((iso->fld)[pos].len <= 0) {
        return NGX_ERROR;
    }
    return (iso->fld)[pos].dict_code;
}


ngx_int_t
ngx_iso8583_set_field(ngx_iso8583_t* iso, ngx_iso8583_field_t* fld, ngx_int_t pos)
{
    if (pos < 0 || pos >= iso->maxfield) {
        return NGX_ERROR;
    }
    if (fld->len > 999) {
    	return NGX_ERROR;
    }

    memcpy((iso->fld) + pos, fld, sizeof(ngx_iso8583_field_t));
    return NGX_OK;
}


ngx_int_t
ngx_iso8583_get_field(ngx_iso8583_t* iso, ngx_iso8583_field_t* fld, ngx_int_t pos)
{
    if (pos < 0 || pos >= iso->maxfield) {
        return NGX_ERROR;
    }
    if (iso->fld[pos].pack == NULL || iso->fld[pos].unpack == NULL) {   //表示该域没有在配置表中进行定义
        return NGX_ERROR;
    }
    memcpy(fld, &iso->fld[pos], sizeof(ngx_iso8583_field_t));
    return  NGX_OK;
}


ngx_int_t
ngx_iso8583_set_bit(ngx_iso8583_t* iso, ngx_int_t pos, char *bytes, ngx_int_t len)
{
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN];

    ngx_iso8583_del_one_bit(iso, pos);
    if (pos < 0  || pos >= iso->maxfield) {
        ErrorLog(ERROR, "exceed max field num");
    	return NGX_ERROR;
    }
    if (pos > 1 && pos <= 128) {
    	iso->bitmap[pos - 1] = 1;		//设置位图
    }
    if (pos >= 65 && pos <= 128) {
        iso->bitmap[0] = 1;	//设置扩展位图位
	}
    if (len > NGX_ISO8583_MAX_FIELD_LEN) {
        len = NGX_ISO8583_MAX_FIELD_LEN;
	}
    iso->flddef[pos].off = iso->used;
    memcpy(iso->databuf + iso->flddef[pos].off, bytes, len);
    iso->flddef[pos].len = len;
    if ( (0 >= iso->fld[pos].pack(iso, &iso->fld[pos],
												iso->databuf + iso->flddef[pos].off,
												iso->flddef[pos].len, tmp))
		|| (0 >= iso->fld[pos].unpack(iso, &iso->fld[pos],
													iso->databuf + iso->flddef[pos].off,
													&iso->flddef[pos].len, tmp))
		) {
			iso->flddef[pos].len = 0;
			iso->flddef[pos].off = 0;
            ErrorLog(ERROR, "set field [%d] error", pos);
			return NGX_ERROR;
	}
    iso->used += iso->flddef[pos].len;
    return NGX_OK;
}


ngx_int_t
ngx_iso8583_get_bit(ngx_iso8583_t* iso, ngx_int_t pos, char *str, ngx_int_t *len)
{
    if (pos > 0 && 0 == iso->bitmap[pos - 1]) {
        str[0] = '\0';
        *len = 0;
        return NGX_OK;
    }

    memcpy(str, iso->databuf + iso->flddef[pos].off, iso->flddef[pos].len);
    if (*len > iso->flddef[pos].len) {
        str[iso->flddef[pos].len] = '\0';
	}
    *len = iso->flddef[pos].len;
    return NGX_OK;
}


ngx_int_t
ngx_iso8583_to_str(char *str, ngx_iso8583_t *iso, ngx_int_t *len)
{
    ngx_int_t i = 0;
    ngx_int_t packed = 0;   /* 8583报文已经被解开的字节数   */
    ngx_int_t used = 0;     /* 当前域在8583报文占用的字节数 */
    ngx_int_t num = 0;

    num = iso->bitmap[0] ? 128 : 64;
	//设置位图
    ngx_iso8583_set_bit(iso, 1, iso->bitmap, num);
    for (i = 0; i <= num; i++) {
        if (i == 0 || i == 1 || iso->bitmap[i - 1]) {
            used = iso->fld[i].pack(iso, &iso->fld[i],
											iso->databuf + iso->flddef[i].off,
											iso->flddef[i].len,
											(u_char*) str + packed);
			if (used < 0) {
				*len = -1;
				return NGX_ERROR;
			}
            packed += used;
        }
    }
    *len = packed;
	return NGX_OK;
}


ngx_int_t
ngx_iso8583_from_str(const char *str, ngx_iso8583_t *iso, ngx_int_t *pos)
{
    ngx_int_t i = 0;
    ngx_int_t unpacked = 0; /* 8583报文已经被解开的字节数   */
    ngx_int_t used = 0;     /* 当前域在8583报文占用的字节数 */

    ngx_iso8583_del_all_bit(iso);
    for (i = 0; i < iso->maxfield; i++) {
        /* field 0 (MSG_ID) and field 1 (BITMAP) need not check */
        if (i >= 2 && 0 == iso->bitmap[i - 1]) {
			continue;
		}
        iso->flddef[i].off = iso->used;
        used = iso->fld[i].unpack(iso, &iso->fld[i],
										  iso->databuf + iso->flddef[i].off,
										  &iso->flddef[i].len,
										  (u_char*) str + unpacked);

        if ((used <= 0)
			|| ((iso->used + iso->flddef[i].len) > NGX_ISO8583_MAX_FIELD_LEN))
        {
            if (NULL != pos) {
                *pos = i;
			}
            iso->flddef[i].off = 0;
            return NGX_ERROR;
        }
        unpacked += used;
        iso->used += iso->flddef[i].len;

        if (i == 1) { //获取位图
            memset( iso->bitmap, 0, sizeof(iso->bitmap)) ;
            memcpy( iso->bitmap, iso->databuf + iso->flddef[i].off, iso->flddef[i].len);
        }
	}
    return unpacked;
}


//在包含首字符的时候，首字符不包括在域的最大长度限制之内
static ngx_int_t
ngx_iso8583_add_pad_char(ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata, u_char head)
{
    ngx_int_t len,count = 0;
    //计算填充之后的总长度 len
    if (this->len_type == NGX_ISO8583_LEN_FIX) {
        len = this->len;
	}
    else {
        len = ngx_min(this->len, fieldlen);
	}
    len += head;
    fieldlen = ngx_min(len, fieldlen);
	if (head == 1) {				//存在头字符，头字符不包含在填充范围
		outdata[count++] = fieldbuf[0];
		--len;
		--fieldlen;
	}
    /* add left padchar */
	if ((this->len_type == NGX_ISO8583_LEN_FIX) && (this->align == NGX_ISO8583_ALN_RIGHT)) {
		memset(outdata + count, this->padchar, len - fieldlen);
		count += len - fieldlen;
	}
    memcpy(outdata + count, (char *) fieldbuf + head, fieldlen);
    count += fieldlen;
    /* add right padchar */
    if ((this->len_type == NGX_ISO8583_LEN_FIX) && (this->align == NGX_ISO8583_ALN_LEFT)) {
        memset(outdata + count, this->padchar, len - fieldlen);
        count += len - fieldlen;
    }
    return count;
}


ngx_int_t
ngx_iso8583_set_field_len_format(ngx_iso8583_t *iso, ngx_int_t len_format)
{

    if (len_format == NGX_ISO8583_LEN_ASCII) {
        iso->get = ngx_iso8583_get_asc_length;
        iso->set = ngx_iso8583_set_asc_length;
    }
    else {
        iso->get = ngx_iso8583_get_bcd_length;
        iso->set = ngx_iso8583_set_bcd_length;
    }

    return NGX_OK;
}


//长度相关回调函数
void
ngx_iso8583_set_bcd_length(ngx_iso8583_field_t *this, ngx_int_t len, u_char* outdata, ngx_int_t *count)
{
    u_char *p = outdata;

    if (this->len_type == NGX_ISO8583_LEN_LLVAR) {
        (*p++) = (u_char)(len % 10) + (len / 10) * 16;
	}
    else {
		if (this->len_type == NGX_ISO8583_LEN_LLLVAR) {
			(*p++) = (u_char)(len / 100);
			(*p++) = (u_char)((len % 100) / 10) * 16 + (len % 100) % 10;
		}
	}

    *count = (p - outdata);
    return;
}


void
ngx_iso8583_get_bcd_length(ngx_iso8583_field_t * this, u_char *indata, ngx_int_t *len, ngx_int_t *count)
{
    u_char *p = indata;

    if (this->len_type == NGX_ISO8583_LEN_FIX) {
        *len = this->len;
	}
    else {
        *len = (*p) - ((*p) >> 4) * 6;
        p++;
        if (this->len_type == NGX_ISO8583_LEN_LLLVAR) {
            *len = (*len) * 100 + ((*p) - ((*p) >> 4) * 6);
            p++;
        }
        if (*len > this->len) {
            *len = this->len;
		}
    }
    *count = (p - indata);
    return;
}


void
ngx_iso8583_set_asc_length(ngx_iso8583_field_t *this, ngx_int_t len, u_char *outdata, ngx_int_t *count)
{
    u_char *p = outdata;
    if (this->len_type == NGX_ISO8583_LEN_LLVAR) {
        (*p++) = (u_char)(len / 10) + 0x30;
        (*p++) = (u_char)(len % 10) + 0x30;
    }
    else if (this->len_type == NGX_ISO8583_LEN_LLLVAR) {
        (*p++) = (u_char)(len / 100) + 0x30;
        (*p++) = (u_char)((len % 100) / 10)  + 0x30;
        (*p++) = (u_char)(len % 10)  + 0x30;
    }
    *count = (p - outdata);
    return;
}


void
ngx_iso8583_get_asc_length(ngx_iso8583_field_t * this, u_char *indata, ngx_int_t *len, ngx_int_t *count)
{
    u_char *p = indata;

    if (this->len_type == NGX_ISO8583_LEN_FIX) {
        *len = this->len;
	}
    else {
        if (this->len_type == NGX_ISO8583_LEN_LLLVAR) {
            *len = ((*p++) - 0x30) * 100;
		}
		else {
			*len = 0;
		}
        *len += ((*p++) - 0x30) * 10;
        *len += (*p++) - 0x30;
        if (*len > this->len) {
            *len = this->len;
		}
    }
    *count = (p - indata);
    return;
}


//打包解包相关回调函数
ngx_int_t
ngx_iso8583_binary_pack(ngx_iso8583_t *iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata)
{
    ngx_int_t len, count;
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN];

    len = ngx_iso8583_add_pad_char(this, fieldbuf, fieldlen, tmp, 0);
    iso->set(this, len, outdata, &count);
    memcpy(outdata + count, (char *) tmp, len);
    return count + len;
}


ngx_int_t
ngx_iso8583_binary_unpack(ngx_iso8583_t *iso, ngx_iso8583_field_t * this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata)
{
    ngx_int_t len, count;

    iso->get(this, indata, &len, &count);
    memcpy(fieldbuf, indata + count, len);
    *fieldlen = len;
    return count + len;
}


ngx_int_t
ngx_iso8583_bcd_msgid_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata)
{
    int len = this->len >> 1;

	if (asc2hex(fieldbuf, this->len, 0, outdata) != NGX_OK) {
		return NGX_ERROR;
	}
    return len;
}



ngx_int_t
ngx_iso8583_bcd_msgid_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata)
{
    int len;

    len = this->len >> 1;
	if (hex2asc(indata, this->len, 0, fieldbuf) != NGX_OK) {
		return NGX_ERROR;
	}
    *fieldlen = this->len;
    return len;
}


ngx_int_t
ngx_iso8583_asc_msgid_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata)
{
    memcpy(outdata, fieldbuf, this->len);
    return this->len;
}


ngx_int_t
ngx_iso8583_asc_msgid_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata)
{

    memcpy(fieldbuf, indata, this->len);
    *fieldlen = this->len;
    return this->len;
}


ngx_int_t
ngx_iso8583_bcd_bitmap_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata)
{
    int i, j;
    ngx_int_t len;
    u_char bitmap[16], bitmask;

    len = fieldlen >> 3;
    memset(bitmap, 0, sizeof(bitmap));
    for (i = 0; i < len; i++) {
        bitmask = 0x80;
        for (j = 0; j < 8; j++, bitmask >>= 1) {
            if (fieldbuf[i * 8 + j]) {
                bitmap[i] |= bitmask;
            }
        }
    }
    memcpy(outdata, bitmap, len);
    return len;
}


ngx_int_t
ngx_iso8583_bcd_bitmap_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, u_char *fieldbuf, ngx_int_t *fieldlen,  u_char *indata)
{
    int i,j;
    ngx_int_t len;
    u_char bitmask;

    if (indata[0] & 0x80) {
        len = 16;
    }
    else {
        len = 8;
    }
    for (i = 0; i < len; i++) {
        bitmask = indata[i];
        for (j = 0; j < 8; j++, bitmask <<= 1) {
            fieldbuf[(i << 3) + j] = ((bitmask & 0x80) ? 1 : 0);
        }
    }
    *fieldlen = (len << 3);
    return len;
}


ngx_int_t
ngx_iso8583_asc_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata)
{
    int i;
    ngx_int_t len, count;
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN];

    for (i = 0; i < fieldlen; i++) {
        if (!ngx_isprint(fieldbuf[i])) {
            return NGX_ERROR;
		}
    }
    len = ngx_iso8583_add_pad_char(this, fieldbuf, fieldlen, tmp, 0);
    iso->set(this, len, outdata, &count);
    memcpy(outdata + count, (char *)tmp, len);
    return count + len;
}


ngx_int_t
ngx_iso8583_asc_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata)
{
    int i;
    ngx_int_t len, count;

    iso->get(this, indata, &len, &count);

    memcpy(fieldbuf, indata + count, len);
    *fieldlen = len;

    for (i = 0; i < *fieldlen; i++) {
        if (!ngx_isprint(fieldbuf[i])) {
            *fieldlen = 0;
            return NGX_ERROR;
        }
    }
    return count + len;
}


ngx_int_t
ngx_iso8583_bcd_pack(ngx_iso8583_t* iso, ngx_iso8583_field_t * this, const u_char *fieldbuf, ngx_int_t fieldlen, u_char *outdata)
{
    int i;
    ngx_int_t len, count;
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN];

    for (i = 0; i < fieldlen; i++) {
        if (!ngx_isprint(fieldbuf[i])) {
            return NGX_ERROR;
		}
    }
    len = ngx_iso8583_add_pad_char(this, fieldbuf, fieldlen, tmp, 0);
    iso->set(this, len, outdata, &count);
    /* 对于奇数长度的，最后半个也按补齐字符压缩 */
    if ((len & 1) && (this->align == NGX_ISO8583_ALN_LEFT)) {
        tmp[len++] = this->padchar;
	}

	asc2hex(tmp, len, (u_char) ((this->align == NGX_ISO8583_ALN_LEFT) ? 0 : 1),
				outdata + count );
	return count + (++len >> 1);
}


ngx_int_t ngx_iso8583_bcd_unpack(ngx_iso8583_t* iso, ngx_iso8583_field_t *this, u_char *fieldbuf, ngx_int_t *fieldlen, u_char *indata)
{
    int i;
    ngx_int_t len, count;

    iso->get(this, indata, &len, &count);

	hex2asc(indata + count, len, (u_char) ((this->align == NGX_ISO8583_ALN_LEFT) ? 0 : 1), fieldbuf);
	*fieldlen = len;

    for (i = 0; i < *fieldlen; i++) {
        if (!ngx_isprint(fieldbuf[i])) {
            *fieldlen = 0;
            return NGX_ERROR;
        }
    }
    return count + (++len >> 1);
}


//iso8583 test code
static ngx_iso8583_field_t ums8583def [] =  {
    {  4, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_bcd_msgid_pack,  ngx_iso8583_bcd_msgid_unpack},  /* 0   "MESSAGE TYPE INDICATOR" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_bcd_bitmap_pack, ngx_iso8583_bcd_bitmap_unpack}, /* 1   "BIT MAP" */
    { 19, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 2   "PAN - PRIMARY ACCOUNT NUMBER" */
    {  6, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 3   "PROCESSING CODE" */
    { 12, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 4   "AMOUNT, TRANSACTION" */
    { 12, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 5   "AMOUNT, SETTLEMENT" */
    { 12, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 6   "AMOUNT, CARDHOLDER BILLING" */
    { 10, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 7   "TRANSMISSION DATE AND TIME" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 8   "AMOUNT, CARDHOLDER BILLING FEE" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 9   "CONVERSION RATE, SETTLEMENT" */
    {  2, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 10  "CONVERSION RATE, CARDHOLDER BILLING" */
    {  6, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 11  "SYSTEM TRACE AUDIT NUMBER" */
    {  6, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 12  "TIME, LOCAL TRANSACTION" */
    {  4, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 13  "DATE, LOCAL TRANSACTION" */
    {  4, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 14  "DATE, EXPIRATION" */
    {  4, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 15  "DATE, SETTLEMENT" */
    {  4, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 16  "DATE, CONVERSION" */
    {  4, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 17  "DATE, CAPTURE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 18  "MERCHANTS TYPE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 19  "ACQUIRING INSTITUTION COUNTRY CODE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 20  "PAN EXTENDED COUNTRY CODE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 21  "FORWARDING INSTITUTION COUNTRY CODE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 22  "POINT OF SERVICE ENTRY MODE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 23  "CARD SEQUENCE NUMBER" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 24  "NETWORK INTERNATIONAL IDENTIFIEER" */
    {  2, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 25  "POINT OF SERVICE CONDITION CODE" */
    {  2, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 26  "POINT OF SERVICE PIN CAPTURE CODE" */
    {  2, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 27  "AUTHORIZATION IDENTIFICATION RESP LEN" */
    { 28, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 28  "AMOUNT, TRANSACTION FEE" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack}, /* 29  "AMOUNT, SETTLEMENT FEE" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack}, /* 30  "AMOUNT, TRANSACTION PROCESSING FEE" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack}, /* 31  "AMOUNT, SETTLEMENT PROCESSING FEE" */
    { 11, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_LEFT, '\0',  ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 32  "ACQUIRING INSTITUTION IDENT CODE" */
    { 11, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 33  "FORWARDING INSTITUTION IDENT CODE" */
    { 28, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 34  "PAN EXTENDED" */
    { 37, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 35  "TRACK 2 DATA" */
    {104, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 36  "TRACK 3 DATA" */
    { 12, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 37  "RETRIEVAL REFERENCE NUMBER" */
    {  6, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 38  "AUTHORIZATION IDENTIFICATION RESPONSE" */
    {  2, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 39  "RESPONSE CODE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 40  "SERVICE RESTRICTION CODE" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 41  "CARD ACCEPTOR TERMINAL IDENTIFICACION" */
    { 15, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 42  "CARD ACCEPTOR IDENTIFICATION CODE"  */
    { 40, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 43  "CARD ACCEPTOR NAME/LOCATION" */
    { 25, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_RIGHT, ' ',  ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack},   /* 44  "ADITIONAL RESPONSE DATA" */ 
    { 76, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 45  "TRACK 1 DATA" */
    {999, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 46  "ADITIONAL DATA - ISO" */
    {999, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 47  "ADITIONAL DATA - NATIONAL" */
    {322, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_RIGHT, ' ',  ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 48  "ADITIONAL DATA - PRIVATE" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 49  "CURRENCY CODE, TRANSACTION" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 50  "CURRENCY CODE, SETTLEMENT" */
    {  3, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 51  "CURRENCY CODE, CARDHOLDER BILLING"    */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack}, /* 52  "PIN DATA"    */
    { 16, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 53  "SECURITY RELATED CONTROL INFORMATION" */
    {20,  NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_RIGHT, ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 54  "ADDITIONAL AMOUNTS" */
    {700, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack},   /* 55  "RESERVED ISO" */
    { 99, NGX_ISO8583_LEN_LLVAR,  NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 56  "RESERVED ISO" */
    {999, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_asc_pack,        ngx_iso8583_asc_unpack},   /* 57  "RESERVED NATIONAL" */
    {255, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  ' ',  ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack},   /* 58  "RESERVED NATIONAL" */
    {999, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack}, /* 59  "RESERVED NATIONAL" */
    { 17, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack}, /* 60  "RESERVED PRIVATE" */
    { 29, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_bcd_pack,        ngx_iso8583_bcd_unpack},   /* 61  "RESERVED PRIVATE" */
    {999, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack},   /* 62  "RESERVED PRIVATE" */
    {163, NGX_ISO8583_LEN_LLLVAR, NGX_ISO8583_ALN_RIGHT, '\0', ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack},   /* 63  "RESERVED PRIVATE" */
    {  8, NGX_ISO8583_LEN_FIX,    NGX_ISO8583_ALN_LEFT,  '\0', ngx_iso8583_binary_pack,     ngx_iso8583_binary_unpack}, /* 64  "MESSAGE AUTHENTICATION CODE FIELD" */
};


ngx_int_t
ngx_iso8583_test(ngx_pool_t *pool)
{
    ngx_iso8583_t *iso;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    ngx_int_t len, pos;
    ErrorLog(ERROR, "ngx_iso8583_test begin");
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        return NGX_ERROR;
    }
    memcpy(iso->fld, (char*)ums8583def, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);
    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, 0,  "0200", 4);
    ngx_iso8583_set_bit(iso, 3,  "100000", 6);
    ngx_iso8583_set_bit(iso, 4,  "000000001000", 12);
    ngx_iso8583_set_bit(iso, 11, "000001", 6);
    ngx_iso8583_set_bit(iso, 22, "020", 3);
    ngx_iso8583_set_bit(iso, 25, "00", 2);
    ngx_iso8583_set_bit(iso, 41, "12345678", 8);
    ngx_iso8583_set_bit(iso, 42, "123456789012345", 15);
    ngx_iso8583_set_bit(iso, 49, "156", 3);
    ngx_iso8583_set_bit(iso, 60, "22000001000500000", 17);
    ngx_iso8583_set_bit(iso, 61, "12345678901", 11);
    ngx_iso8583_set_bit(iso, 62, "\x01\x02\x03\x04\x05\x06\x07", 7);
    ngx_iso8583_set_bit(iso, 64, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);
    if (ngx_iso8583_to_str(pack, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        return NGX_ERROR;
    }
    ErrorLog(ERROR, "example iso8583 package:");
    ErrorLog(ERROR, "%s", pack);
    ngx_iso8583_del_all_bit(iso);
    if (ngx_iso8583_from_str(pack, iso, &pos) <= 0) {
        ErrorLog(ERROR, "unpack iso8583 field[%d] error", pos);
        return NGX_ERROR;
    }
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 0, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 0, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 3, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 3, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 4, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 4, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 11, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 11, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 22, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 22, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 25, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 25, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 41, tmp, &len);
    ErrorLog(ERROR,  "field %d:[%s]", 41, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 42, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 42, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 49, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 49, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 60, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 60, tmp);
    memset(tmp, 0,sizeof(tmp));
    ngx_iso8583_get_bit(iso, 61, tmp, &len);
    ErrorLog(ERROR, "field %d:[%s]", 61, tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 62, tmp, &len);
    ErrorLog(ERROR, "field %d:", 62);
    ErrorLog(ERROR, "%s", tmp);
    memset(tmp, 0, sizeof(tmp));
    ngx_iso8583_get_bit(iso, 64, tmp, &len);
    ErrorLog(ERROR, "field %d:", 64);
    ErrorLog(ERROR, "%s", tmp);
    ErrorLog(ERROR, "ngx_iso8583_test end");
    return NGX_OK;
}
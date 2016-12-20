#include "ngx_core.h"


static ngx_datapool_field_t* ngx_find_field_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no);
static ngx_datapool_field_t* ngx_get_field_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no);


ngx_datapool_t*
ngx_create_datapool(ngx_pool_t* pool, size_t n) {
	ngx_datapool_t *datapool;

	datapool = ngx_palloc(pool, sizeof(ngx_datapool_t));
	if (datapool == NULL) {
		return NULL;
	}
    datapool->size = sizeof(ngx_datapool_field_t);
    datapool->nalloc = n;
    datapool->pool = pool;

    datapool->elts = ngx_pcalloc(pool, n * sizeof(ngx_datapool_field_t));
    if (datapool->elts == NULL) {
        return NULL;
    }
	return datapool;
}


ngx_int_t
ngx_clear_datapool_field(ngx_datapool_t* datapool, ngx_int_t field_no) {
	ngx_datapool_field_t *field;
	if (field_no >= datapool->nalloc) {
		return -1;
	}
	field = (ngx_datapool_field_t*)(datapool->elts + field_no * datapool->size);
	if (field->used == NGX_DATAPOOL_UNUSED) {
		return 0;
	}
	field->size = 0;
	field->used = 0;
	if (field->field_type != NGX_DATAPOOL_BUF) {
		field->value.l = 0L;
	}
	return 0;
}


ngx_int_t
ngx_clear_datapool(ngx_datapool_t* datapool) {
	memset(datapool->elts, 0, datapool->nalloc * datapool->size);
	return 0;
}


ngx_int_t
ngx_get_int_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no, ngx_int_t *value) {
	ngx_datapool_field_t *field;

	field = ngx_find_field_from_datapool(datapool, field_no);
	if (field == NULL) {
		return 0;
	}
	if (field->field_type != NGX_DATAPOOL_INT) {
		return -1;
	}
	*value = field->value.i;
	return field->size;
}


ngx_int_t*
ngx_get_intref_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no) {
	ngx_datapool_field_t *field;

	field = ngx_find_field_from_datapool(datapool, field_no);
	if (field == NULL) {
		return NULL;
	}
	if (field->field_type != NGX_DATAPOOL_INT) {
		return NULL;
	}
	return &field->value.i;
}


ngx_int_t
ngx_add_int_to_datapool(ngx_datapool_t* datapool, ngx_uint_t field_no, ngx_int_t value) {
 	ngx_datapool_field_t *field;

 	field = ngx_get_field_from_datapool(datapool, field_no);
 	if (field == NULL) {
 		return -1;
 	}
  	if (field->used == NGX_DATAPOOL_USED && field->field_type != NGX_DATAPOOL_INT) {
 		return -1;
 	}
 	field->field_no = field_no;
 	field->field_type = NGX_DATAPOOL_INT;
 	field->used = NGX_DATAPOOL_USED;
 	field->value.i = value;
 	field->size = sizeof(ngx_int_t);
	return 0;
}


ngx_int_t
ngx_get_long_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no, long* value) {
	ngx_datapool_field_t *field;

	field = ngx_find_field_from_datapool(datapool, field_no);
	if (field == NULL) {
		return 0;
	}
	if (field->field_type != NGX_DATAPOOL_LONG) {
		return -1;
	}
	*value = field->value.l;
	return field->size;
}


ngx_int_t
ngx_add_long_to_datapool(ngx_datapool_t* datapool, ngx_uint_t field_no, long value) {
	ngx_datapool_field_t *field;

 	field = ngx_get_field_from_datapool(datapool, field_no);
 	if (field == NULL) {
 		return -1;
 	}
 	if (field->used == NGX_DATAPOOL_USED && field->field_type != NGX_DATAPOOL_LONG) {
 		return -1;
 	}
 	field->field_no = field_no;
 	field->field_type = NGX_DATAPOOL_LONG;
 	field->used = NGX_DATAPOOL_USED;
 	field->value.l = value;
 	field->size = sizeof(ngx_int_t);
	return 0;
}


char*
ngx_get_buf_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no, ngx_int_t *len) {
	ngx_datapool_field_t *field;

	field = ngx_find_field_from_datapool(datapool, field_no);
	if (field == NULL) {
		return NULL;
	}
	if (field->field_type != NGX_DATAPOOL_BUF) {
		return NULL;
	}
	if (len	!= NULL) {
		*len = field->size;
	}
	return field->value.buf + 2;
}


ngx_int_t
ngx_add_buf_to_datapool(ngx_datapool_t* datapool, ngx_uint_t field_no, const u_char* value, size_t size) {
	ngx_datapool_field_t *field;
	int len;

 	field = ngx_get_field_from_datapool(datapool, field_no);
 	if (field == NULL) {
 		return -1;
 	}
 	if (field->used == NGX_DATAPOOL_UNUSED) {//第一次使用该编号
 		field->value.buf = ngx_pcalloc(datapool->pool, size + 3);	 //缓冲的头两个字节用来表示后面缓冲的大小,以及字节最后一个0x00填充符号
 		if (field->value.buf == NULL) {
 			return -1;
 		}
 		ngx_int_to_c2(field->value.buf, size);
 		ngx_memmove(field->value.buf + 2, value, size);
 	}
 	else {
 		if (field->field_type != NGX_DATAPOOL_BUF) {
 			return -1;
 		}
 		len = ngx_c2_to_int(field->value.buf);
 		if (len >= size) {
 			memset(field->value.buf + 2, 0, field->size);
 			ngx_memmove(field->value.buf + 2, value, size);
 		}
 		else {	//原来的缓冲不够大，重新申请一块缓冲
 			field->value.buf = ngx_pcalloc(datapool->pool, size + 2);
	  		if (field->value.buf == NULL) {
	 			return -1;
	 		}
	 		ngx_int_to_c2(field->value.buf, size);
	 		ngx_memmove(field->value.buf + 2, value, size);
 		}
 	}
 	field->field_no = field_no;
 	field->field_type = NGX_DATAPOOL_BUF;
 	field->used = NGX_DATAPOOL_USED;
 	field->size = size;
	return 0;
}


ngx_int_t
ngx_datapool_compare_dict_value(ngx_datapool_t *datapool, ngx_int_t field_no1, ngx_int_t field_no2) {
	ngx_datapool_field_t *field1, *field2;

	field1 = ngx_get_field_from_datapool(datapool, field_no1);
	field2 = ngx_get_field_from_datapool(datapool, field_no2);
	if (field1 == NULL || field2 == NULL) {
		return -1;
	}
	if (field1->used == NGX_DATAPOOL_UNUSED || field2->used == NGX_DATAPOOL_UNUSED) {
		return -1;
	}
	if (field1->field_type != field2->field_type) {
		return -1;
	}
	if (field_no1 == field_no2) {
		return 0;
	}
	switch (field1->field_type) {
	case NGX_DATAPOOL_INT:
		if (field1->value.i != field2->value.i) {
			return -1;
		}
		return 0;
		break;
	case NGX_DATAPOOL_LONG:
		if (field1->value.l != field2->value.l) {
			return -1;
		}
		return 0;
		break;
	case NGX_DATAPOOL_BUF:
		if (field1->size != field2->size) {
			return -1;
		}
		if (field1->value.buf == field2->value.buf) {
			return 0;
		}
		if (memcmp(field1->value.buf + 2, field2->value.buf + 2, field1->size) != 0) {
			return -1;
		}
		return 0;
		break;
	}
	return 0;
}


static ngx_datapool_field_t*
ngx_get_field_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no) {
	ngx_datapool_field_t *field;
	void* new;
	ngx_uint_t n;
	if (field_no >= NGX_DATAPOOL_MAX_FIELDNUM) {
		ErrorLog(ERROR, "field no excess max field no");
		return NULL;
	}
	if (field_no < datapool->nalloc) {			//在目前分配空间范围之内
		field = (ngx_datapool_field_t*)(datapool->elts + field_no * datapool->size);
	}
	else {	//超出目前分配空间，重新分配
			n = datapool->nalloc * 2;
			n = (n > NGX_DATAPOOL_MAX_FIELDNUM) ? NGX_DATAPOOL_MAX_FIELDNUM : n;
			new = ngx_palloc(datapool->pool, n * datapool->size);
			if (new == NULL) {
				return NULL;
			}
			memcpy(new, datapool->elts, datapool->nalloc * datapool->size);
			datapool->elts = new;
			datapool->nalloc = n;
			field = (ngx_datapool_field_t*)(datapool->elts + field_no * datapool->size);
	}
	return field;
}


static ngx_datapool_field_t*
ngx_find_field_from_datapool(ngx_datapool_t *datapool, ngx_uint_t field_no) {
	ngx_datapool_field_t *field;

	if (field_no >= NGX_DATAPOOL_MAX_FIELDNUM) {
		return NULL;
	}
	if (field_no >= datapool->nalloc) {
		return NULL;
	}
	field = (ngx_datapool_field_t*)(datapool->elts + datapool->size * field_no);
	if (field->used != NGX_DATAPOOL_USED) {
		return NULL;
	}
	return field;
}





#include "ngx_core.h"
#include "cJSON.h"


ngx_messenger_t *
ngx_create_messenger(ngx_pool_t* pool)
{
	ngx_messenger_t *messenger;
	pn_messenger_t *pmessenger;
	pmessenger = pn_messenger(NULL);
	if (pmessenger == NULL) {
		return NULL;
	}
	messenger = ngx_pcalloc(pool, sizeof(ngx_messenger_t));
	if (messenger == NULL) {
		pn_messenger_free(pmessenger);
		return NULL;
	}
	messenger->pmessenger = pmessenger;
	//启动队列
	pn_messenger_start(pmessenger);
	//初始化队列
	//posp_queue_init(&messenger->queue);
	return messenger;
}


ngx_message_t*
ngx_create_message( )
{
	ngx_message_t* message;
	pn_message_t* pmessage;
	pmessage = pn_message();
	if (pmessage == NULL) {
		return NULL;
	}
	message = ngx_calloc(sizeof(ngx_message_t));
	if (message == NULL) {
		pn_message_free(pmessage);
		return NULL;
	}
	message->pmessage = pmessage;
	return message;
}


ngx_int_t
ngx_free_message(ngx_message_t* message)
{
	pn_message_free(message->pmessage);
	ngx_free(message);
	return NGX_OK;
}


ngx_int_t
ngx_close_messenger_send_link(ngx_messenger_t *messenger, char* addr)
{
	pn_link_t *link;
	link = pn_messenger_get_link(messenger->pmessenger, addr, 1);
	if (link == NULL) {
		return NGX_ERROR;
	}
	pn_link_close(link);
	pn_link_free(link);
	return NGX_OK;
}


ngx_int_t
ngx_close_messenger_recv_link(ngx_messenger_t* messenger, char* addr)
{
	pn_link_t *link;
	link = pn_messenger_get_link(messenger->pmessenger, addr, 0);
	if (link == NULL) {
		return NGX_ERROR;
	}
	pn_link_close(link);
	pn_link_free(link);
	return NGX_OK;
}


ngx_int_t
ngx_put_message(ngx_messenger_t *messenger, ngx_message_t *message)
{
	int rv;

	rv = pn_messenger_put(messenger->pmessenger, message->pmessage);
	if (rv != 0) {
	        ErrorLog(ERROR, "messenger put error %d", rv);
	        return NGX_ERROR;
	}
	while (1) {
	        rv = pn_messenger_send(messenger->pmessenger, -1);
	        if (rv == -8) {
	                ErrorLog(ERROR, "messenger send interrupt, continue");
	                continue;
	        }
	        else if (rv == -9) {
	                ErrorLog(ERROR, "messenger send inprocess, continue");
	                continue;
	        }
	        else if (rv == 0) {
	                ErrorLog(ERROR, "messenger send succ");
	        }
	        else {
	         		ErrorLog(ERROR, "messenger send error, %d", rv);
	                //return POSP_ERROR;
	                //目前暂时不进行发送报错处理
	        }
	        break;
	}

	return NGX_OK;
}


ngx_int_t
ngx_send_message(ngx_messenger_t *messenger)
{
	if (pn_messenger_send(messenger->pmessenger, -1) != 0) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_recv_message(ngx_messenger_t *messenger, ngx_int_t num)
{
	ngx_message_t* message;
	pn_tracker_t tracker;

	if (messenger == NULL) {
		return NGX_ERROR;
	}
	if (pn_messenger_recv(messenger->pmessenger, num) != 0) {

		return NGX_ERROR;
	}
	while (pn_messenger_incoming(messenger->pmessenger)) {
		ErrorLog(ERROR, "recving a message....");
		message = ngx_create_message();
		if (message == NULL) {
			ErrorLog(ERROR, "create message object error");
			return NGX_ERROR;
		}
		if (pn_messenger_get(messenger->pmessenger, message->pmessage) != 0) {
			ErrorLog(ERROR, " get message from messenger error");
			return NGX_ERROR;
		}
		if (!pn_message_is_first_acquirer(message->pmessage)) {
			ErrorLog(ERROR, "not fisrt acquire message, drop it");
			ngx_free_message(message);
			continue;
		}
		ErrorLog(ERROR, "insert message queue");
		//posp_queue_insert_tail(&messenger->queue, &message->node);	//插入到队列末尾
	}
	return NGX_OK;
}


ngx_int_t
ngx_set_message_subject(ngx_message_t* message, char* subject)
{
	if (pn_message_set_subject(message->pmessage, subject) != 0) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_set_message_target(ngx_message_t* message, const char* target)
{
	if (pn_message_set_address(message->pmessage, target) != 0) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t ngx_set_message_reply_to(ngx_message_t* message, char* reply_to)
{
	if (pn_message_set_reply_to(message->pmessage, reply_to) != 0) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


const char*
ngx_get_message_reply_to(ngx_message_t* message)
{
	const char* reply_to;
	reply_to = pn_message_get_reply_to(message->pmessage);
	if (reply_to == NULL) {
		return NULL;
	}
	return reply_to;
}


ngx_int_t
ngx_set_message_lifetime(ngx_message_t* message, ngx_msec_t lifetime)
{
	if (pn_message_set_ttl(message->pmessage, lifetime) != 0) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_clear_message_body(ngx_message_t* message)
{
	pn_data_t* body;
	body = pn_message_body(message->pmessage);
	pn_data_clear(body);
	return NGX_OK;
}


ngx_int_t
ngx_set_message_body_type(ngx_message_t* message, ngx_int_t type)
{
	pn_data_t *body;

	switch (type) {
	case NGX_MESSAGE_TYPE_MAP:
		body = pn_message_body(message->pmessage);
		pn_data_clear(body);
		pn_data_put_map(body);
		break;
	default:
		break;
	}
	return NGX_OK;
}


ngx_int_t
ngx_put_message_map_buf_value(ngx_message_t* message, char* key, char* value, ngx_int_t len)
{
	pn_data_t *body;

	body = pn_message_body(message->pmessage);
	pn_data_enter(body);
	//移动到MAP的最后位置
	while (pn_data_next(body)) {
	}
	pn_data_put_string(body, pn_bytes(strlen(key), key));
	pn_data_put_binary(body, pn_bytes(len, value));
	pn_data_exit(body);
	return NGX_OK;
}


ngx_int_t
ngx_put_message_map_int_value(ngx_message_t* message, char* key, int value)
{
	pn_data_t *body;

	body = pn_message_body(message->pmessage);
	pn_data_enter(body);
	//移动到MAP的最后位置
	while (pn_data_next(body)) {
	}
	pn_data_put_string(body, pn_bytes(strlen(key), key));
	pn_data_put_int(body, value);
	pn_data_exit(body);
	return NGX_OK;
}


ngx_int_t
ngx_put_message_map_long_value(ngx_message_t* message, char* key, long value)
{
	pn_data_t *body;

	body = pn_message_body(message->pmessage);
	pn_data_enter(body);
	//移动到MAP的最后位置
	while (pn_data_next(body)) {
	}
	pn_data_put_string(body, pn_bytes(strlen(key), key));
	pn_data_put_long(body, value);
	pn_data_exit(body);
	return NGX_OK;
}


ngx_int_t
ngx_put_message_map_string_value(ngx_message_t* message, char* key, char* value)
{
	pn_data_t *body;

	body = pn_message_body(message->pmessage);
	pn_data_enter(body);
	//移动到MAP的最后位置
	while (pn_data_next(body)) {
	}
	pn_data_put_string(body, pn_bytes(strlen(key), key));
	pn_data_put_string(body, pn_bytes(strlen(value), value));
	pn_data_exit(body);
	return NGX_OK;
}

/*
ngx_int_t
ngx_format_datapool_to_message_body(ngx_datapool_t* datapool, ngx_message_t *message)
{
	ngx_int_t i;
	ngx_datapool_field_t *field;
	pn_data_t *body;
	char key[10];

	body = pn_message_body(message->pmessage);
	pn_data_clear(body);
	pn_data_put_map(body);
	pn_data_enter(body);
	for (i = 0; i < datapool->nalloc; i++) {
		field = (ngx_datapool_field_t*)datapool->elts + i;
		if (field->used ==  NGX_DATAPOOL_UNUSED) {
			continue;
		}
		memset(key, 0, sizeof(key));
		sprintf(key, "%d", field->field_no);
		pn_data_put_string(body, pn_bytes(strlen(key), key));
		switch (field->field_type) {
		case NGX_DATAPOOL_INT:
			pn_data_put_int(body, field->value.i);
			break;
		case NGX_DATAPOOL_LONG:
			pn_data_put_long(body, field->value.l);
			break;
		case NGX_DATAPOOL_BUF:
			pn_data_put_binary(body, pn_bytes(field->size, field->value.buf + 2));
			break;
		default:
			pn_data_exit(body);
			return NGX_ERROR;
			break;
		}
	}
	pn_data_exit(body);
	return NGX_OK;
}


ngx_int_t
ngx_format_message_body_to_datapool(ngx_message_t* message, ngx_datapool_t* datapool)
{
	ngx_int_t i;
	ngx_datapool_field_t *field;
	pn_data_t *body;
	ngx_uint_t field_no;
	pn_bytes_t bytes;

	ngx_clear_datapool(datapool);
	body = pn_message_body(message->pmessage);
	pn_data_next(body);
	//遍历message boyd
	pn_data_enter(body);
	while (pn_data_next(body)) {
		//获取key
		bytes = pn_data_get_string(body);
		field_no = ngx_atoi(bytes.start, bytes.size);
		//获取value
		pn_data_next(body);
		switch (pn_data_type(body)) {
		case PN_INT:
			ngx_add_int_to_datapool(datapool, field_no, pn_data_get_int(body));
			break;
		case PN_LONG:
			ngx_add_long_to_datapool(datapool, field_no, pn_data_get_long(body));
			break;
		case PN_BINARY:
			bytes = pn_data_get_binary(body);
			ngx_add_buf_to_datapool(datapool, field_no, bytes.start, bytes.size);
			break;
		default:
			pn_data_exit(body);
			return NGX_ERROR;
			break;
		}
	}
	pn_data_exit(body);
	return NGX_OK;
}
*/

ngx_int_t
ngx_set_message_uuid(ngx_message_t* message, ngx_uuid_t* uuid)
{
	pn_data_t* id;
	pn_uuid_t u;

	memcpy(u.bytes, uuid->bytes, NGX_UUID_LEN);
	id = pn_message_id(message->pmessage);
	pn_data_put_uuid(id, u);
	return NGX_OK;
}


ngx_int_t
ngx_get_message_uuid(ngx_message_t *message, ngx_uuid_t* uuid)
{
	pn_data_t *id;
	pn_uuid_t u;

	id = pn_message_id(message->pmessage);
	u = pn_data_get_uuid(id);
	memcpy(uuid->bytes, u.bytes, NGX_UUID_LEN);
	return NGX_OK;
}


/*ngx_message_t*
ngx_get_message_by_uuid(ngx_messenger_t *messenger, ngx_uuid_t* uuid)
{
	pn_uuid_t u;
	pn_data_t *data;
	ngx_queue_t* q, *h;
	ngx_message_t* message;

	h = q = &messenger->queue;
	while (ngx_queue_next(q) != h) {		//循环队列
		q = ngx_queue_next(q);
		message = ngx_queue_data(q, ngx_message_t, node);
		data = pn_message_id(message->pmessage);
		u = pn_data_get_uuid(data);
		if (memcmp(u.bytes, uuid->bytes, NGX_UUID_LEN) == 0) {
			ngx_queue_remove(&message->node);
			return message;
		}
	}
	return NULL;
}*/


ngx_int_t
ngx_messenger_subscribe(ngx_messenger_t* messenger, const char* source)
{

	messenger->subscription = pn_messenger_subscribe(messenger->pmessenger, source);
	if (messenger->subscription == NULL) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


ngx_int_t
ngx_messenger_get_subscribe_addr(ngx_messenger_t* messenger, char* addr)
{
	const char* a = pn_subscription_address(messenger->subscription);
	if (a == NULL) {
		return NGX_ERROR;
	}
	if (addr != NULL) {
		strcpy(addr, a);
	}
	return NGX_OK;
}


ngx_int_t
ngx_messenger_set_mode(ngx_messenger_t* messenger, ngx_int_t mode)
{
	ngx_int_t rv;
	switch (mode) {
	case NGX_MESSENGER_SYNC:
		rv = pn_messenger_set_blocking(messenger->pmessenger, 0);
		break;
	case NGX_MESSENGER_ASYNC:
		rv = pn_messenger_set_blocking(messenger->pmessenger, 1);
		break;
	default:
		return NGX_ERROR;
		break;
	}
	if (rv != 0) {
		return NGX_ERROR;
	}
	return NGX_OK;
}


/*ngx_int_t
ngx_process_messenger(ngx_cycle_t* cycle)
{
	ngx_messenger_t *messenger = cycle->messenger;
	ngx_message_t *message;
	ngx_queue_t* q;

	if (messenger != NULL &&  messenger->subscription != NULL) {
	//一次性尽可能多的接收消息
		ngx_recv_message(messenger, -1);
		//调用消息处理回调
		if (messenger->handler !=  NULL) {
			while (!ngx_queue_empty(&messenger->queue)) {	//遍历队列
				q = ngx_queue_head(&messenger->queue);
				message = ngx_queue_data(q, ngx_message_t, node);
				ngx_queue_remove(&message->node);
				message->messenger = messenger;
				ErrorLog(ERROR, "begin message handler");
				messenger->handler(message);
			}
		}
	}
	return NGX_OK;
}*/


ngx_int_t ngx_message_body_dump(ngx_message_t* message)
{
	pn_data_t* body;
	ErrorLog(ERROR, "PID:[%d] message body:\n", getpid());
	body = pn_message_body(message->pmessage);
	pn_data_dump(body);
	return NGX_OK;
}


ngx_int_t
ngx_format_datapool_to_message_body(ngx_datapool_t* datapool, pn_message_t* message)
{
	ngx_int_t i;
	ngx_datapool_field_t *field;
	pn_data_t *body;
	char key[10];

	body = pn_message_body(message);
	pn_data_clear(body);
	pn_data_put_map(body);
	pn_data_enter(body);
	for (i = 0; i < datapool->nalloc; i++) {
		field = (ngx_datapool_field_t*)datapool->elts + i;
		if (field->used ==  NGX_DATAPOOL_UNUSED) {
			continue;
		}
		memset(key, 0, sizeof(key));
		sprintf(key, "%d", field->field_no);
		pn_data_put_string(body, pn_bytes(strlen(key), key));
		switch (field->field_type) {
		case NGX_DATAPOOL_INT:
			pn_data_put_int(body, field->value.i);
			break;
		case NGX_DATAPOOL_LONG:
			pn_data_put_long(body, field->value.l);
			break;
		case NGX_DATAPOOL_BUF:
			pn_data_put_binary(body, pn_bytes(field->size, field->value.buf + 2));
			break;
		default:
			pn_data_exit(body);
			return NGX_ERROR;
			break;
		}
	}
	pn_data_exit(body);
	return NGX_OK;
}


ngx_int_t
ngx_format_message_body_to_datapool(pn_message_t* message, ngx_datapool_t* datapool) {
	ngx_int_t i;
	ngx_datapool_field_t *field;
	pn_data_t *body;
	ngx_uint_t field_no;
	pn_bytes_t bytes;

	ngx_clear_datapool(datapool);
	body = pn_message_body(message);
	pn_data_next(body);
	//遍历message boyd
	pn_data_enter(body);
	while (pn_data_next(body)) {
		//获取key
		bytes = pn_data_get_string(body);
		field_no = ngx_atoi(bytes.start, bytes.size);
		//获取value
		pn_data_next(body);
		switch (pn_data_type(body)) {
		case PN_INT:
			ngx_add_int_to_datapool(datapool, field_no, pn_data_get_int(body));
			break;
		case PN_LONG:
			ngx_add_long_to_datapool(datapool, field_no, pn_data_get_long(body));
			break;
		case PN_BINARY:
			bytes = pn_data_get_binary(body);
			ngx_add_buf_to_datapool(datapool, field_no, bytes.start, bytes.size);
			break;
		default:
			pn_data_exit(body);
			return NGX_ERROR;
			break;
		}
	}
	pn_data_exit(body);
	return NGX_OK;
}


#ifndef _NGX_MESSENGER_H_
#define _NGX_MESSENGER_H_

#include "ngx_core.h"

#include <proton/message.h>
#include <proton/messenger.h>

#define NGX_MESSENGER_SYNC		0
#define NGX_MESSENGER_ASYNC	1

#define NGX_MESSAGE_TYPE_MAP		0
#define NGX_MESSAGE_TYPE_LIST		1
#define NGX_MESSAGE_TYPE_BINARY	2


//消息处理回调函数
typedef struct ngx_message_s ngx_message_t;
typedef struct ngx_messenger_s ngx_messenger_t;
typedef ngx_int_t (*ngx_message_handler_pt)(ngx_message_t *message);
typedef ngx_uint_t  ngx_rbtree_key_t;
typedef ngx_rbtree_key_t      ngx_msec_t;

struct ngx_messenger_s {
 	pn_messenger_t *pmessenger;
 	pn_subscription_t *subscription;
 	//ngx_queue_t queue;
 	ngx_message_handler_pt handler;
};

struct ngx_message_s {
	pn_message_t *pmessage;
	//posp_queue_t node;
	ngx_messenger_t *messenger;
};

ngx_messenger_t* ngx_create_messenger(ngx_pool_t* pool);
ngx_message_t* ngx_create_message();
ngx_int_t ngx_free_message(ngx_message_t* message);
ngx_int_t ngx_close_messenger_send_link(ngx_messenger_t *messenger, char* addr);
ngx_int_t ngx_close_messenger_recv_link(ngx_messenger_t* messenger, char* addr);
ngx_int_t ngx_put_message(ngx_messenger_t *messenger, ngx_message_t *message);
ngx_int_t ngx_send_message(ngx_messenger_t *messenger);
ngx_int_t ngx_recv_message(ngx_messenger_t *messenger, ngx_int_t num);
ngx_int_t ngx_set_message_subject(ngx_message_t* message, char* subject);
ngx_int_t ngx_set_message_target(ngx_message_t* message, const char* target);
ngx_int_t ngx_set_message_reply_to(ngx_message_t* message, char* reply_to);
const char* ngx_get_message_reply_to(ngx_message_t* message);
//ngx_int_t ngx_set_message_lifetime(ngx_message_t* message, ngx_msec_t lifetime);
ngx_int_t ngx_clear_message_body(ngx_message_t* message);
//ngx_int_t ngx_format_datapool_to_message_body(ngx_datapool_t* datapool, ngx_message_t *message);
//ngx_int_t ngx_format_message_body_to_datapool(ngx_message_t* message, ngx_datapool_t* datapool);
ngx_int_t ngx_set_message_uuid(ngx_message_t* message, ngx_uuid_t* uuid);
ngx_int_t ngx_get_message_uuid(ngx_message_t *message, ngx_uuid_t* uuid);
ngx_message_t* ngx_get_message_by_uuid(ngx_messenger_t *messenger, ngx_uuid_t* uuid);
ngx_int_t ngx_messenger_subscribe(ngx_messenger_t* messenger, const char* source);
ngx_int_t ngx_messenger_get_subscribe_addr(ngx_messenger_t* messenger, char* addr);
ngx_int_t ngx_messenger_set_mode(ngx_messenger_t* messenger, ngx_int_t mode);
//ngx_int_t ngx_process_messenger(ngx_cycle_t* cycle);
ngx_int_t ngx_message_body_dump(ngx_message_t* message);
ngx_int_t ngx_set_message_body_type(ngx_message_t* message, ngx_int_t type);
ngx_int_t ngx_put_message_map_buf_value(ngx_message_t* message, char* key, char* value, ngx_int_t len);
ngx_int_t ngx_put_message_map_int_value(ngx_message_t* message, char* key, int value);
ngx_int_t ngx_put_message_map_long_value(ngx_message_t* message, char* key, long value);
ngx_int_t ngx_put_message_map_string_value(ngx_message_t* message, char* key, char* value);
ngx_int_t ngx_format_datapool_to_message_body(ngx_datapool_t* datapool, pn_message_t* message);
ngx_int_t ngx_format_message_body_to_datapool(pn_message_t* message, ngx_datapool_t* datapool);
#endif
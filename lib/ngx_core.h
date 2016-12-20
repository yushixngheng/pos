#ifndef _NGX_CORE_H_
#define _NGX_CORE_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <stddef.h>
#include <semaphore.h>
#include <dlfcn.h>
#include <errno.h>
#include <iconv.h>
#include <assert.h>
#include <proton/message.h>
#include <proton/messenger.h>
#include <libmemcached/memcached.h>
#include <arpa/inet.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <termio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <setjmp.h>


typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
typedef intptr_t        ngx_flag_t;

typedef struct ngx_pool_s ngx_pool_t;
typedef struct ngx_uuid_s ngx_uuid_t;
typedef struct ngx_datapool_s ngx_datapool_t;

#define ngx_abs(value)       (((value) >= 0) ? (value) : - (value))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))

#define ngx_isprint(c) ((char)(c) >= 0x20 && (char)(c) < 0x7f)
#define ngx_isdigit(c) ((char)(c) >= 0x40 && (char)(c) <= 0x49)
#define ngx_swap32(num) ((((num)>>24)&0xff) | (((num)<<8)&0xff0000) | (((num)>>8)&0xff00) | (((num)<<24)&0xff000000))


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"


#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN	  	 -2
#define  NGX_NOTFOUND	 -3

#define ngx_inline inline


#include "ngx_datapool.h"
#include "ngx_alloc.h"
#include "ngx_pool.h"
#include "ngx_uuid.h"
#include "log.h"
#include "ngx_array.h"
#include "ngx_str.h"
#include "ngx_get_conf.h"
#include "ngx_dict.h"
#include "cJSON.h"
#include "ngx_queue.h"
#include "ngx_memcached.h"
#include "ngx_messenger.h"
#include "ngx_cache.h"
#include "ngx_socket.h"
#include "ngx_http.h"
#include "ngx_iso8583.h"
#include "ngx_charset.h"
#include "ngx_crypt.h"
#include "dataapi.h"
#include "dbdes.h"



#endif
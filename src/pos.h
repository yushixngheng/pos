#ifndef _POS_H_
#define _POS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "ngx_core.h"


#define SYS_CONF_NAME "yjx.conf"
#define MHT_CONF_PATH "yujx/conf"
#define MHT_TRADE_LOG "yujx/log"

#define COMMAND_COMMON				"COMMON"
#define COMMAND_DOWNKEY     		"DOWNKEY"		//远程下载密钥
#define COMMAND_LOGIN				"LOGIN"			//签到
#define COMMAND_LOGOUT      		"LOGOUT"		//签退
#define COMMAND_PAY         		"PAY"			//消费
#define COMMAND_VOID_PAY    		"VOID_PAY"		//消费撤销
#define COMMAND_PREAUTH    			"PREAUTH"		//预授权
#define COMMAND_VOID_PREAUTH    	"VOID_PREAUTH"	//预授权撤销
#define COMMAND_AUTHPAY    			"AUTHPAY"		//预授权完成
#define COMMAND_VOID_AUTHPAY    	"VOID_AUTHPAY"	//预授权完成撤销
#define COMMAND_PAY_REVERAL			"PAY_REVERAL"	//消费冲正



#define MSG_TYPE_DOWNKEY         "99"
#define MSG_TYPE_LOGIN           "00"
#define MSG_TYPE_LOGOUT          "00"
#define MSG_TYPE_PAY             "22"
#define MSG_TYPE_PAY_REVERSAL    "22"
#define MSG_TYPE_VOID_PAY        "23"
#define MSG_TYPE_PREAUTH         "10"
#define MSG_TYPE_VOID_PREAUTH    "11"
#define MSG_TYPE_AUTHPAY         "20"
#define MSG_TYPE_VOID_AUTHPAY    "21"

#define BATCH_CODE_DOWNKEY      "000000"

#define NET_CODE_DOWNKEY    "682"
#define NET_CODE_LOGIN      "003" //POS终端签到（双倍长密钥算法)
#define NET_CODE_LOGOUT     "002"
#define NET_CODE_DEFAULT    "000"//缺省值（默认值）
#define INPUT_ABILITY       "5"//POS 60.4终端读取能力
#define CONDITION_CODE      "0"//POS 60.5基于PBOC借贷记IC卡条件代码

#define MSG_TYPE_LEN        2
#define BATCH_CODE_LEN      6
#define NET_CODE_LEN        3

#define TCP_HEADER_LEN  	2
#define TPDU_LEN		 	5
#define PACKET_HEADER_LEN   6


typedef struct {
    ngx_datapool_t* datapool;
    ngx_pool_t* pool;
}datapool_t;

typedef ngx_int_t (*cmd_proc)();

typedef struct {
		char* 	     	name;		//所属指令
		cmd_proc 	proc;		//指令处理函数
}pos_cmd_t;

typedef struct {
	char* optname;				//短选项名称
	char* optnamel;				//长选项名称
	char* cmd;					//所属指令 (COMMON代表通用选项)
	ngx_int_t   flag;			//选项属性     0不带参数, 1带参数
	char* value;				//选项默认值
}pos_cmd_option_t;


static void pos_help();
static ngx_socket_t pos_connect_server();
static int pos_send_handler(ngx_socket_t s, char *buf, int len);
static int pos_recv_handler(ngx_socket_t s, char *buf);
static char* pos_get_option(char* optname);
static ngx_int_t db_init();
static int pos_parse_option(int argc, char** argv);
static void change_term_water();
static void trade_result_save(ngx_datapool_t* datapool, char *trade_type, char* org_trade_ref_code);
static void pin_pan(char* card_code, char* passwd, char* tpk, char *out);
static void pos_sales_slip(ngx_pool_t *pool, ngx_datapool_t* datapool);
static ngx_int_t pos_iso8583_downkey();
static ngx_int_t pos_iso8583_login();
static ngx_int_t pos_iso8583_logout();
static ngx_int_t pos_iso8583_pay();
static ngx_int_t pos_iso8583_void_pay();
static ngx_int_t pos_iso8583_preauth();
static ngx_int_t pos_iso8583_void_preauth();
static ngx_int_t pos_iso8583_authpay();
static ngx_int_t pos_iso8583_void_authpay();
static ngx_int_t pos_iso8583_pay_reveral();


#endif
#include "pos.h"


//iso8583 test code
static ngx_iso8583_field_t ums8583def_test [] =  {
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

static pos_cmd_t* cmd = NULL;       //命令

//选项配置表
static pos_cmd_option_t cmd_option[] = {

                        {"a",           "addr",                  COMMAND_COMMON,                 1,  "192.168.13.100"},//pos机器后台ip
                        {"p",           "port",                  COMMAND_COMMON,                 1,  "10028"},         //pos后台端口
                        {"t",           "timedout",              COMMAND_COMMON,                 1,  "60"},
                        {"",            "tpdu",                  COMMAND_COMMON,                 1,  "6000010000"},
                        {"",            "header",                COMMAND_COMMON,                 1,  "603100311511"},
                        {"",            "buffer-size",           COMMAND_COMMON,                 1,  "2048"},
                        {"mht",         "mht_code",              COMMAND_COMMON,                 1,  "037471751370022"},
                        {"term",        "term_code",             COMMAND_COMMON,                 1,  "80000001"},
                        {"card",        "card_code",             COMMAND_COMMON,                 1,  "6227001217480226101"},
                        {"passwd",      "card_password",         COMMAND_COMMON,                 1,  "123456"},
                        {"amt",         "trade_amt",             COMMAND_COMMON,                 1,  "000000000001"},
                        {"type",        "mht_type",              COMMAND_COMMON,                 1,  "t"},//t表传统pos机器(大pos) m表示mops
                        {"barcode",     "barcode",               COMMAND_COMMON,                 1,  NULL},//条码信息
                        {"pay_mode",    "barcode_pay_mode",      COMMAND_COMMON,                 1,  "01"},//条码支付渠道  00 无卡交易;01 支付宝交易; 02 微信支付交易
                        {"",            "ref_code",              COMMAND_COMMON,                 1,  NULL},
                        {"",            "help",                  COMMAND_COMMON,                 0,  NULL},
                        {"",            "downkey",               COMMAND_DOWNKEY,                0,  NULL},
                        {"",            "login",                 COMMAND_LOGIN,                  0,  NULL},
                        {"",            "logout",                COMMAND_LOGOUT,                 0,  NULL},
                        {"",            "pay",                   COMMAND_PAY,                    0,  NULL},
                        {"",            "void_pay",              COMMAND_VOID_PAY,               0,  NULL},
                        {"",            "preauth",               COMMAND_PREAUTH,                0,  NULL},
                        {"",            "void_preauth",          COMMAND_VOID_PREAUTH,           0,  NULL},
                        {"",            "authpay",               COMMAND_AUTHPAY,                0,  NULL},
                        {"",            "void_authpay",          COMMAND_VOID_AUTHPAY,           0,  NULL},
                        {"",            "pay_reveral",           COMMAND_PAY_REVERAL,            0,  NULL},
                        {NULL, NULL, NULL, 0, NULL}

                        };

//命名表
static pos_cmd_t cmd_table[] = {

                        {COMMAND_DOWNKEY,               pos_iso8583_downkey},
                        {COMMAND_LOGIN,                 pos_iso8583_login},
                        {COMMAND_LOGOUT,                pos_iso8583_logout},
                        {COMMAND_PAY,                   pos_iso8583_pay},
                        {COMMAND_PAY_REVERAL,           pos_iso8583_pay_reveral},
                        {COMMAND_VOID_PAY,              pos_iso8583_void_pay},
                        {COMMAND_PREAUTH,               pos_iso8583_preauth},
                        {COMMAND_VOID_PREAUTH,          pos_iso8583_void_preauth},
                        {COMMAND_AUTHPAY,               pos_iso8583_authpay},
                        {COMMAND_VOID_AUTHPAY,          pos_iso8583_void_authpay},
                        {COMMAND_PAY_REVERAL,           pos_iso8583_pay_reveral},
                        {NULL, NULL}

                        };


static void
pos_help()
{
    printf("Usage: pos [COMMON OPTIONS] COMMAND [COMMAND OPTIONS]\n");
    return;
}


static char*
pos_get_option(char* optname)
{
    int i;
    pos_cmd_option_t *opt;

    for (i = 0; ;i++) {
        opt = &cmd_option[i];
        if (opt->cmd == NULL) {
            break;
        }
        if (
            (strcmp(opt->optname, optname) == 0) ||
            (strcmp(opt->optnamel, optname) == 0)
            ) {
            return opt->value;
        }
    }
    return NULL;
}


static
ngx_socket_t pos_connect_server()
{
    ngx_socket_t s;
    char *addr, *port, *timedout;
    struct sockaddr_in sa;
    int socklen;
    int err;

    addr = pos_get_option("a");
    port = pos_get_option("p");
    timedout = pos_get_option("t");
    s = ngx_socket(AF_INET, SOCK_STREAM, 0);
    if (s == (ngx_socket_t) -1) {
        err = errno;
        ErrorLog(ERROR, "create socket error %d\n", err);
        return;
    }
    if (ngx_socket_reuseaddr(s) != 0) {
        if (ngx_close_socket(s) != 0) {
        }
        return;
    }
    if (ngx_socket_timedout(s, atoi(timedout) * 1000) != 0) {
        if (ngx_close_socket(s) != 0) {
        }
        return;
    }
    sa.sin_family = AF_INET;
    sa.sin_port   = htons(atoi(port));
    sa.sin_addr.s_addr = inet_addr(addr);
    socklen = sizeof(struct sockaddr_in);
    if (ngx_connect_peer(s, (struct sockaddr*)&sa, socklen) != 0) {
        return NGX_ERROR;
    }
    return s;
}

static int
pos_send_handler(ngx_socket_t s, char *buf, int len)
{

    ssize_t size;
    size = ngx_send(s, (char*)buf, len);
    if (size <= 0 || size != len) {//链接已经被关闭
        return NGX_ERROR;
    }
    return NGX_OK;
}


static int
pos_recv_handler(ngx_socket_t s, char *buf)
{

    ssize_t size;
    size = ngx_recv(s, buf, 1024);
    if (size <= 0) {//说明连接被关闭
        return NGX_ERROR;
    }
    return size;
}


static void
change_term_water()
{
    char mht_conf[1024] = {0}, batch_code[6 + 1] = {0}, term_water[6 + 1] = {0};

    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));

    ngx_get_conf(mht_conf, "current_term_water", term_water);
    sprintf(term_water, "%06d", atoi(term_water) + 1);
    if (file_line_change(mht_conf, "current_term_water", term_water) != NGX_OK) {
        ErrorLog(ERROR, "change current_term_water error");
        return;
    }
    return;
}


static void
pin_pan(char* card_code, char* passwd, char* tpk, char *out)
{

    int i;
    char passwd_pan[16 + 1] = {0}, passwd_pan_hex[9 + 1] = {0};
    char card_tmp[12 + 1] = {0}, card_pan[16 + 1] = {0}, card_pan_hex[9 + 1] = {0};
    char pin_data[9 + 1] = {0}, tpk_hex[16 + 1] = {0};
    sprintf(passwd_pan, "%02d%s", strlen(passwd), passwd);
    for (i = strlen(passwd) + 2; i < 16; i++) {
        sprintf(passwd_pan + i, "F");
    }

    memcpy(card_tmp, card_code + strlen(card_code)-12-1, 12);
    sprintf(card_pan, "0000%s", card_tmp);
    asc2hex(passwd_pan, 16, 0, passwd_pan_hex);
    asc2hex(card_pan, 16, 0, card_pan_hex);

    for (i = 0; i < 9; i++)
        pin_data[i] = card_pan_hex[i] ^ passwd_pan_hex[i];
    asc2hex(tpk, 32, 0, tpk_hex);
    ngx_3des(out, pin_data, tpk_hex, NGX_DES_CRYPT);
}


static void
trade_result_save(ngx_datapool_t* datapool, char *trade_type, char* org_trade_ref_code)
{
    char *ref_code_card_code, *ref_code_trade_amt, *ref_code_term_water, *ref_code, *resp_code;
    char *ref_code_auth_code, *ref_code_sys_date, *ref_code_sys_time, *ref_code_mht_code, *ref_code_term_code;
    char ref_code_batch_code[6 + 1] = {0}, mht_conf[1024] = {0};
    FILE *fd;
    ref_code_card_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_CARD_CODE, NULL);
    ref_code_trade_amt = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TRADE_AMT, NULL);
    memcpy(ref_code_batch_code, ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_FRI60, NULL) + 2, 6);
    ref_code_term_water = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERM_WATER, NULL);
    ref_code_sys_date = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SYS_DATE, NULL);
    ref_code_sys_time = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SYS_TIME, NULL);
    ref_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_REF_CODE, NULL);
    resp_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_RESP_CODE, NULL);
    ref_code_term_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERMINAL_CODE, NULL);
    ref_code_mht_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_MERCHANT_CODE, NULL);
    ref_code_term_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_TERMINAL_CODE, NULL);
    ref_code_auth_code = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_AUTH_CODE, NULL);

    if (strcmp(resp_code, "00") != NGX_OK && strcmp(resp_code, "09") != NGX_OK) {
        ErrorLog(INFO, "交易不成功 不保存到文件里");
        if (strcmp(resp_code, "94") == NGX_OK) {
            ErrorLog(INFO, "交易流水号重复,请签退后再签到");
            printf("交易流水号重复,请签退后再签到");
        }
        return;
    }
    sprintf(mht_conf, "%s/%s/%s_%s.conf", getenv("HOME"), MHT_CONF_PATH, ref_code_mht_code, ref_code_term_code);
    if ((fd = fopen( mht_conf, "a+")) == NULL) {
        ErrorLog(ERROR,"open the file error");
        return;
    }
    fprintf(fd, "#%s参考号\n", trade_type);
    fprintf(fd, "[%s]\n", ref_code);
    fprintf(fd, "ref_code_mht_code=%s\n", ref_code_mht_code);
    fprintf(fd, "ref_code_term_code=%s\n", ref_code_term_code);
    fprintf(fd, "ref_code_batch_code=%s\n", ref_code_batch_code);
    fprintf(fd, "ref_code_term_water=%s\n", ref_code_term_water);
    fprintf(fd, "ref_code_sys_date=%s\n", ref_code_sys_date);
    fprintf(fd, "ref_code_sys_time=%s\n", ref_code_sys_time);
    if (ref_code_card_code != NULL) {
        fprintf(fd, "ref_code_card_code=%s\n", ref_code_card_code);
    }
    fprintf(fd, "ref_code_trade_amt=%s\n", ref_code_trade_amt);
    if (ref_code_auth_code != NULL) {
        fprintf(fd, "ref_code_auth_code=%s\n", ref_code_auth_code);
    }
    if (org_trade_ref_code != NULL) { //交易类型是反交易
        fprintf(fd, "org_trade_ref_code=%s\n", org_trade_ref_code);
    }
    fprintf(fd, "\n");
    fclose(fd);
    return;
}


//签购单打印
static void
pos_sales_slip(ngx_pool_t *pool, ngx_datapool_t* datapool)
{
    ngx_array_t* dict_set;
    char *fri59;
    ngx_str_t *n;
    char mn_utf[200 + 1] = {0};
    ngx_int_t begin, i;
    int mht_name_len;

    //解析59域获取签购单信息
    fri59 = ngx_get_buf_from_datapool(datapool, NGX_DATAPOOL_SALES_SLIP, NULL);
    if(fri59 != NULL) {
        dict_set = ngx_array_create(pool, 6, sizeof(ngx_str_t));
        ngx_separate_string(fri59, '|', dict_set);
        if (dict_set->nelts != 3) {
                ErrorLog(ERROR, "fri59 elt number error");
                return;
        }
        //printf("dict_set->nelts=%d\n", dict_set->nelts);
        begin = NGX_DATAPOOL_SALES_SLIP_MERCHANT_CODE;
        n = dict_set->elts;
        for (i = 0; i < dict_set->nelts; i++) { //将签购单相关信息添加进数据池
            if (n[i].len > 0) {
                if (i == 2) {//商户名GBK转UTF8
                    //memset(mn_utf, 0x20, sizeof(mn_utf) -1);
                    mht_name_len = sizeof(mn_utf) - 1;
                    if (ngx_gbk2uni(n[i].data, n[i].len, mn_utf, &mht_name_len) != NGX_OK) {
                        ErrorLog(ERROR, "convert account name from gbk to utf8 error");
                        return;
                    }
                    //ErrorLog(INFO, "mn_utf[%s]mht_name_len[%d]strlen(mn_utf)[%d]", mn_utf, mht_name_len, strlen(mn_utf));
                    //ngx_trim(mn_utf);
                    ngx_add_buf_to_datapool(datapool, begin + i, mn_utf, mht_name_len);
                } else {
                    ngx_add_buf_to_datapool(datapool, begin + i, n[i].data, n[i].len);
                }
            }
        }
    }
    return;
}


static ngx_int_t
pos_iso8583_downkey()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2] = {0}, fri60[17 + 1] = {0};
    char p1[2048] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char *mht_code, *term_code;
    char *buffer, *tpdu, *header;
    char* p;
    char *fri62;
    char kek_asc[32 + 1] = {0};
    char kek_hex[16 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, mht_conf[1024] = {0};
    char *trade_type ="远程密钥下载";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取商户号终端号
    mht_code = pos_get_option("mht");
    term_code = pos_get_option("term");

    //填充60域
    sprintf(fri60, MSG_TYPE_DOWNKEY, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, BATCH_CODE_DOWNKEY, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DOWNKEY, NET_CODE_LEN); //设置网络管理信息码

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE,  "0800", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE, "0800", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));;
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存终端主密钥明文
    fri62 = ngx_get_buf_from_datapool(rt->datapool, NGX_DATAPOOL_FRI62, NULL);
    char zmk_hex[16 + 1] = {0};
    char zmk[32 + 1] = {0};
    char sys_conf_path[1024] = {0};
    FILE *fd;
    sprintf(sys_conf_path, "%s/%s/%s", getenv("HOME"), MHT_CONF_PATH, SYS_CONF_NAME);
    ngx_get_conf(sys_conf_path, "pos_kek", kek_asc);
    asc2hex(kek_asc, 32, 0, kek_hex);
    //解密主密钥
    ngx_3des(zmk_hex, fri62, kek_hex, NGX_DES_DECRYPT);
    ngx_3des(zmk_hex + 8, fri62 + 8, kek_hex, NGX_DES_DECRYPT);
    hex2asc(zmk_hex, 32, 0, zmk);

    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    if (file_line_change(mht_conf, "zmk", zmk) != NGX_OK) {
        if ((fd = fopen( mht_conf, "w+")) == NULL) {
            ErrorLog(ERROR,"open the file error");
            return NGX_ERROR;
        }
        fprintf(fd, "zmk=%s\n", zmk);
        fclose(fd);
    }
    if (ngx_get_conf(mht_conf, "current_batch_code", batch_code) != NGX_OK) {
        strcpy(batch_code, "0000001");
    } else {
        sprintf(batch_code, "%06d", atoi(batch_code));
    }
    if (ngx_get_conf(mht_conf, "current_term_water", term_water) != NGX_OK) {
        strcpy(term_water, "0000001");
    } else {
        sprintf(term_water, "%06d", atoi(term_water) + 1);
    }
    if (file_line_change(mht_conf, "current_batch_code", batch_code) != NGX_OK) {
        if ((fd = fopen( mht_conf, "aw+")) == NULL) {
            ErrorLog(ERROR,"open the file error");
            return NGX_ERROR;
        }
        fprintf(fd, "current_batch_code=000001\n");
        fclose(fd);
    }
    ErrorLog(ERROR,"1111111111111111111111111");
    if (file_line_change(mht_conf, "current_term_water", term_water) != NGX_OK) {
        if ((fd = fopen( mht_conf, "aw+")) == NULL) {
            ErrorLog(ERROR,"open the file error");
            return NGX_ERROR;
        }
        fprintf(fd, "current_term_water=000001\n");
        fclose(fd);
    }
    if (file_line_change(mht_conf, "current_login_flag", "0") != NGX_OK) {
        if ((fd = fopen( mht_conf, "aw+")) == NULL) {
            ErrorLog(ERROR,"open the file error");
            return NGX_ERROR;
        }
        fprintf(fd, "current_login_flag=0\n");
        fclose(fd);
    }

    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_login()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2] = {0}, batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri60[17 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char *mht_code, *term_code;
    char *buffer, *tpdu, *header;
    char* p;
    char mht_conf[1024] = {0};
    char *trade_type = "签到";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取商户号终端号
    mht_code = pos_get_option("mht");
    term_code = pos_get_option("term");

    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    if (ngx_get_conf(mht_conf, "current_batch_code", batch_code) != NGX_OK) {
        ErrorLog(ERROR, "该终端未做远程密钥下载, 请先远程密钥下载获取终端主密钥");
        printf("该终端未做远程密钥下载, 请先远程密钥下载获取终端主密钥\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_get_conf(mht_conf, "current_term_water", term_water);

    //填充60域
    sprintf(fri60, MSG_TYPE_LOGIN, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_LOGIN, NET_CODE_LEN); //设置网络管理信息码

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE,  "0800", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI63, "001", 3);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0800", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI63, "001", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

     //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);

    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //将ztpk明文 ztak明文保存到商户信息文件里
    char *fri62;
    char zmk[32 + 1] = {0}, zmk_hex[32 + 1] = {0}, ztpk_hex[16 + 1] = {0}, ztpk[32 + 1] = {0}, ztak_hex[8 + 1] = {0}, ztak[16 + 1] = {0};
    FILE *fd;

    ngx_get_conf(mht_conf, "zmk", zmk);
    fri62 = ngx_get_buf_from_datapool(rt->datapool, NGX_DATAPOOL_FRI62, NULL);

    asc2hex(zmk, 32, 0, zmk_hex);
    //解密主密钥
    ngx_3des(ztpk_hex, fri62, zmk_hex, NGX_DES_DECRYPT);
    ngx_3des(ztpk_hex + 8, fri62 + 8, zmk_hex, NGX_DES_DECRYPT);
    hex2asc(ztpk_hex, 32, 0, ztpk);
    ngx_3des(ztak_hex, fri62 + 20, zmk_hex, NGX_DES_DECRYPT);
    hex2asc(ztak_hex, 16, 0, ztak);

    ErrorLog(INFO, "ztpk[%s], ztak[%s]", ztpk, ztak);
    if (file_line_change(mht_conf, "ztpk", ztpk) != NGX_OK || file_line_change(mht_conf, "ztak", ztak) != NGX_OK) {
        if ((fd = fopen( mht_conf, "aw+")) == NULL) {
            ErrorLog(ERROR,"open the file error");
            return NGX_ERROR;
        }
        fprintf(fd, "ztpk=%s\n", ztpk);
        fprintf(fd, "ztak=%s\n", ztak);
        fprintf(fd, "\n");
        fclose(fd);
    }

    char *fri60_recv, *term_water_recv;
    char pos_current_batch_code[6 + 1] = {0}, pos_current_term_water[6 + 1] = {0};
    fri60_recv = ngx_get_buf_from_datapool(rt->datapool, 60, NULL);
    memcpy(pos_current_batch_code, fri60_recv + 2, 6);
    term_water_recv = ngx_get_buf_from_datapool(rt->datapool, NGX_DATAPOOL_TERM_WATER, NULL);
    sprintf(pos_current_term_water, "%06d", atoi(term_water_recv) + 1);
    if (file_line_change(mht_conf, "current_batch_code", pos_current_batch_code) != NGX_OK) {
        ErrorLog(ERROR,"change pos_current_batch_code error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    if (file_line_change(mht_conf, "current_term_water", pos_current_term_water) != NGX_OK) {
        ErrorLog(ERROR,"change pos_current_term_water error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;

    }
    if (file_line_change(mht_conf, "current_login_flag", "1") != NGX_OK) {
        ErrorLog(ERROR,"change pos_current_login_flag error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;

    }
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_logout()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2] = {0}, batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri60[17 + 1] = {0};
    char p1[2048] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char *mht_code, *term_code;
    char *buffer, *tpdu, *header;
    char* p;
    char mht_conf[1024] = {0};
    char *trade_type = "签退";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取商户号终端号
    mht_code = pos_get_option("mht");
    term_code = pos_get_option("term");
    //获取pos机的当前的批次号流水号
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    if (ngx_get_conf(mht_conf, "current_batch_code", batch_code) != NGX_OK) {
        ErrorLog(ERROR, "该终端未做远程密钥下载, 请先远程密钥下载获取终端主密钥");
        printf("该终端未做远程密钥下载, 请先远程密钥下载获取终端主密钥\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_get_conf(mht_conf, "current_term_water", term_water);

    //填充60域
    sprintf(fri60, MSG_TYPE_LOGOUT, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_LOGOUT, NET_CODE_LEN); //设置网络管理信息码

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE,  "0820", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE, "0820", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

   //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //修改商户pos机器上的批次号流水号
    char *fri60_recv, *term_water_recv;
    char pos_current_batch_code[6 + 1] = {0}, pos_current_term_water[6 + 1] = {0};
    fri60_recv = ngx_get_buf_from_datapool(rt->datapool, 60, NULL);
    memcpy(pos_current_batch_code, fri60_recv + 2, 6);
    sprintf(pos_current_batch_code, "%06d", atoi(pos_current_batch_code) + 1);
    term_water_recv = ngx_get_buf_from_datapool(rt->datapool, NGX_DATAPOOL_TERM_WATER, NULL);
    sprintf(pos_current_term_water, "%06d", atoi(term_water_recv) + 1);
    if (file_line_change(mht_conf, "current_batch_code", pos_current_batch_code) != NGX_OK) {
        ErrorLog(ERROR,"change pos_current_batch_code error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    if (file_line_change(mht_conf, "current_term_water", pos_current_term_water) != NGX_OK) {
        ErrorLog(ERROR,"change pos_current_term_water error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    if (file_line_change(mht_conf, "current_login_flag", "0") != NGX_OK) {
        ErrorLog(ERROR,"change pos_current_login_flag error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_pay()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tak[16 + 1] = {0}, tpk[32 + 1] = {0}, tak_hex[8 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0},fri60[17 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char *mht_code, *term_code, *card_code, *trade_amt, *passwd, *mht_type;
    char *buffer, *tpdu, *header;
    char* p;
    char mht_conf[1024] = {0}, login_flag[1 + 1] = {0};
    char *trade_type = "消费";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取商户号终端号
    mht_code = pos_get_option("mht");
    term_code = pos_get_option("term");
    //获取卡号和交易金额
    card_code = pos_get_option("card");
    trade_amt = pos_get_option("amt");

    //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ngx_destroy_pool(pool);
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
        return NGX_ERROR;
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //填充35域二磁道信息
    sprintf(fri35, "%sD42075207311020000", card_code);
    //密码加密
    // passwd = pos_get_option("passwd");
    // pin_pan(card_code, passwd, tpk, fri52);
    //填充60域
    sprintf(fri60, MSG_TYPE_PAY, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0200", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "000000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "00", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    // ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    mht_type = pos_get_option("mht_type");
    if(mht_type[0] == 'm' || mht_type[0] == 'M' ) {
        ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SALES_SLIP, "mpos15601627165", 15);//mpos59域需要上送mpos+手机号
    }
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac(ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    //ANSI X9.9MAC算法(cbc mac)
    //ngx_calc_x99_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE, "0200", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE, "000000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SERVICE_CODE, "00", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    // ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    if(mht_type[0] == 'm' || mht_type[0] == 'M' ) {
        ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SALES_SLIP, "mpos15601627165", 15);
    }
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

    //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //签购单处理
    pos_sales_slip(pool, rt->datapool);
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, NULL);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_void_pay()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tpk[32 + 1] = {0},tak[16 + 1] = {0}, tak_hex[8 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri2[19 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0}, fri60[17 + 1] = {0};
    char org_batch_code[6 + 1] = {0}, org_term_water[6 + 1] = {0}, org_sys_date[4 +1] = {0}, fri61[29 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char mht_code[15 + 1] = {0}, term_code[8 + 1] = {0}, card_code[19 + 1] = {0}, trade_amt[12 + 1] = {0}, mht_conf[1024] = {0}, login_flag[1 + 1] = {0};
    char *buffer, *tpdu, *header, *ref_code, *passwd;
    char* p;
    char *trade_type = "消费撤销";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取参考号
    ref_code = pos_get_option("ref_code");
    if (ref_code == NULL) {
        ErrorLog(ERROR, "该交易缺少交易参数ref_code");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //查询原交易商户号终端号
    if (GetConf(mht_conf, ref_code, "ref_code_mht_code", mht_code) != NGX_OK) {
        ErrorLog(ERROR, "未找到这笔交易, 请确认这笔交易的参考号");
        printf("未找到这笔交易, 请确认该这交易的参考号\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    GetConf(mht_conf, ref_code, "ref_code_term_code", term_code);
    //查询原交易卡号和交易金额
    GetConf(mht_conf, ref_code, "ref_code_card_code", card_code);
    GetConf(mht_conf, ref_code, "ref_code_trade_amt", trade_amt);
    //查询原交易批次号流水号日期
    GetConf(mht_conf, ref_code, "ref_code_batch_code", org_batch_code);
    GetConf(mht_conf, ref_code, "ref_code_term_water", org_term_water);

    //填充35域二磁道信息
    sprintf(fri35, "%sD42075207311020000", card_code);
    //密码加密
    passwd = pos_get_option("passwd");
    pin_pan(card_code, passwd, tpk, fri52);
    //填充60域
    sprintf(fri60, MSG_TYPE_VOID_PAY, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码
    //填充61域
    sprintf(fri61, org_batch_code, 6); //设置原始交易批次号
    sprintf(fri61 + 6, org_term_water, 6); //设置原始交易POS流水号

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0200", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "200000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "00", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_REF_CODE, ref_code, 12);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac(ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0200", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CARD_CODE,  card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  "200000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT,  trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SERVICE_CODE, "00", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRACK2,  fri35, strlen(fri35));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_REF_CODE, ref_code, 12);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

    //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(ERROR, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(ERROR, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //签购单处理
    pos_sales_slip(pool, rt->datapool);
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, ref_code);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_preauth()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tak[16 + 1] = {0}, tpk[32 + 1] = {0}, tak_hex[8 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0}, fri60[17 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char *mht_code, *term_code, *card_code, *trade_amt, *passwd;
    char *buffer, *tpdu, *header;
    char* p;
    char mht_conf[1024] = {0}, login_flag[1 + 1] = {0};
    char *trade_type = "预授权";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取商户号终端号
    mht_code = pos_get_option("mht");
    term_code = pos_get_option("term");
    //获取卡号和交易金额
    card_code = pos_get_option("card");
    trade_amt = pos_get_option("amt");

    //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ngx_destroy_pool(pool);
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //填充35域二磁道信息
    sprintf(fri35, "%sD42075207311020000", card_code);
    //密码加密
    passwd = pos_get_option("passwd");
    pin_pan(card_code, passwd, tpk, fri52);

    //填充60域
    sprintf(fri60, MSG_TYPE_PREAUTH, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0100", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "030000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac (ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0100", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CARD_CODE,  card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  "030000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRACK2,  fri35, strlen(fri35));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

     //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //签购单处理
    pos_sales_slip(pool, rt->datapool);
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, NULL);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_void_preauth()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tpk[32 + 1] = {0}, tak[16 + 1] = {0}, tak_hex[8 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0}, fri60[17 + 1] = {0}, login_flag[1 + 1] = {0};
    char org_batch_code[6 + 1] = {0}, org_term_water[6 + 1] = {0}, fri61[29 + 1] = {0}, org_sys_date[4 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char mht_code[15 + 1] = {0}, term_code[8 + 1] = {0}, card_code[19 + 1] = {0}, trade_amt[12 + 1] = {0}, mht_conf[1024] = {0}, auth_code[8 + 1] = {0};
    char *buffer, *tpdu, *header, *ref_code, *passwd;
    char* p;
    char *trade_type ="预授权撤销";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取参考号
    ref_code = pos_get_option("ref_code");
    if (ref_code == NULL) {
        ErrorLog(ERROR, "该交易缺少交易参数ref_code");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

   //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ngx_destroy_pool(pool);
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //查询原交易商户号终端号
    if (GetConf(mht_conf, ref_code, "ref_code_mht_code", mht_code) != NGX_OK) {
        ErrorLog(ERROR, "未找到这笔交易, 请确认这笔交易的参考号");
        printf("未找到这笔交易, 请确认该这交易的参考号\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    GetConf(mht_conf, ref_code, "ref_code_term_code", term_code);
    //查询原交易卡号和交易金额
    GetConf(mht_conf, ref_code, "ref_code_card_code", card_code);
    GetConf(mht_conf, ref_code, "ref_code_trade_amt", trade_amt);
    //查询原交易批次号流水号日期
    GetConf(mht_conf, ref_code, "ref_code_batch_code", org_batch_code);
    GetConf(mht_conf, ref_code, "ref_code_term_water", org_term_water);
    GetConf(mht_conf, ref_code, "ref_code_sys_date", org_sys_date);
    //查询原交易授权码
    GetConf(mht_conf, ref_code, "ref_code_auth_code", auth_code);

    //填充35域二磁道信息
    sprintf(fri35, "%sD42075207311020000", card_code);
    //密码加密
    passwd = pos_get_option("passwd");
    pin_pan(card_code, passwd, tpk, fri52);
    //填充60域
    sprintf(fri60, MSG_TYPE_VOID_PREAUTH, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码
    //填充61域
    sprintf(fri61, org_batch_code, 6); //设置原始交易批次号
    sprintf(fri61 + 6, org_term_water, 6); //设置原始交易POS流水号
    sprintf(fri61 + 12, org_sys_date, 4); //设置原始交易POS交易日期

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0100", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "200000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_AUTH_CODE, auth_code, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac(ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0100", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CARD_CODE,  card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  "200000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRACK2,  fri35, strlen(fri35));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_AUTH_CODE, auth_code, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "156", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

     //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //签购单处理
    pos_sales_slip(pool, rt->datapool);
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, ref_code);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_authpay()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tpk[32 + 1] = {0}, tak[16 + 1] = {0}, tak_hex[8 + 1] = {0}, login_flag[1 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0}, fri60[17 + 1] = {0};
    char org_batch_code[6 + 1] = {0}, org_term_water[6 + 1] = {0}, fri61[29 + 1] = {0}, org_sys_date[4 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char mht_code[15 + 1] = {0}, term_code[8 + 1] = {0}, card_code[19 + 1] = {0}, trade_amt[12 + 1] = {0}, mht_conf[1024] = {0}, auth_code[8 + 1] = {0};
    char *buffer, *tpdu, *header, *ref_code, *passwd;
    char* p;
    char *trade_type = "预授权完成(请求)"; //pos机器上的预授权完成是指预授权完成请求

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取参考号
    ref_code = pos_get_option("ref_code");
    if (ref_code == NULL) {
        ErrorLog(ERROR, "该交易缺少交易参数ref_code");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ngx_destroy_pool(pool);
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //查询原交易商户号终端号
    if (GetConf(mht_conf, ref_code, "ref_code_mht_code", mht_code) != NGX_OK) {
        ErrorLog(ERROR, "未找到这笔交易, 请确认这笔交易的参考号");
        printf("未找到这笔交易, 请确认该这交易的参考号\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    GetConf(mht_conf, ref_code, "ref_code_term_code", term_code);
    //查询原交易卡号和交易金额
    GetConf(mht_conf, ref_code, "ref_code_card_code", card_code);
    GetConf(mht_conf, ref_code, "ref_code_trade_amt", trade_amt);
    //查询原交易批次号流水号日期
    GetConf(mht_conf, ref_code, "ref_code_batch_code", org_batch_code);
    GetConf(mht_conf, ref_code, "ref_code_term_water", org_term_water);
    GetConf(mht_conf, ref_code, "ref_code_sys_date", org_sys_date);
    //查询原交易授权码
    GetConf(mht_conf, ref_code, "ref_code_auth_code", auth_code);
    //填充35域二磁道信息
    sprintf(fri35, "%sD42075207311020000", card_code);
    //密码加密
    passwd = pos_get_option("passwd");
    pin_pan(card_code, passwd, tpk, fri52);
    //填充60域
    sprintf(fri60, MSG_TYPE_AUTHPAY, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码
    //填充61域
    sprintf(fri61, org_batch_code, 6); //设置原始交易批次号
    sprintf(fri61 + 6, org_term_water, 6); //设置原始交易POS流水号
    sprintf(fri61 + 12, org_sys_date, 4); //设置原始交易POS交易日期

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0200", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "000000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_AUTH_CODE, auth_code, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac(ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0200", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CARD_CODE,  card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  "000000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRACK2,  fri35, strlen(fri35));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_AUTH_CODE, auth_code, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

     //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //签购单处理
    pos_sales_slip(pool, rt->datapool);
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, ref_code);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_void_authpay()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tpk[32 + 1] = {0}, tak[16 + 1] = {0}, tak_hex[8 + 1] = {0}, login_flag[1 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0}, fri60[17 + 1] = {0};
    char org_batch_code[6 + 1] = {0}, org_term_water[6 + 1] = {0}, fri61[29 + 1] = {0}, org_sys_date[4 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char mht_code[15 + 1] = {0}, term_code[8 + 1] = {0}, card_code[19 + 1] = {0}, trade_amt[12 + 1] = {0}, mht_conf[1024] = {0}, auth_code[8 + 1] = {0};
    char *buffer, *tpdu, *header, *ref_code, *passwd;
    char* p;
    char *trade_type = "预授权完成撤销";
    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取参考号
    ref_code = pos_get_option("ref_code");
    if (ref_code == NULL) {
        ErrorLog(ERROR, "该交易缺少交易参数ref_code");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ngx_destroy_pool(pool);
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
        return NGX_ERROR;
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //查询原交易商户号终端号
    if (GetConf(mht_conf, ref_code, "ref_code_mht_code", mht_code) != NGX_OK) {
        ErrorLog(ERROR, "未找到这笔交易, 请确认这笔交易的参考号");
        printf("未找到这笔交易, 请确认该这交易的参考号\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    GetConf(mht_conf, ref_code, "ref_code_term_code", term_code);
    //查询原交易卡号和交易金额
    GetConf(mht_conf, ref_code, "ref_code_card_code", card_code);
    GetConf(mht_conf, ref_code, "ref_code_trade_amt", trade_amt);
    //查询原交易批次号流水号日期
    GetConf(mht_conf, ref_code, "ref_code_batch_code", org_batch_code);
    GetConf(mht_conf, ref_code, "ref_code_term_water", org_term_water);
    GetConf(mht_conf, ref_code, "ref_code_sys_date", org_sys_date);
    //查询原交易授权码
    GetConf(mht_conf, ref_code, "ref_code_auth_code", auth_code);

    //填充35域二磁道信息
    sprintf(fri35, "%sD42075207311020000", card_code);
    //密码加密
    passwd = pos_get_option("passwd");
    pin_pan(card_code, passwd, tpk, fri52);
    //填充60域
    sprintf(fri60, MSG_TYPE_VOID_AUTHPAY, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码
    //填充61域
    sprintf(fri61, org_batch_code, 6); //设置原始交易批次号
    sprintf(fri61 + 6, org_term_water, 6); //设置原始交易POS流水号
    sprintf(fri61 + 12, org_sys_date, 4); //设置原始交易POS交易日期

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0200", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "200000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT,  trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_REF_CODE, ref_code, 12);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac(ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0200", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  "200000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT,  trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SERVICE_CODE, "06", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN_MAX_LEN, "12", 2);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRACK2, fri35, strlen(fri35));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_REF_CODE, ref_code, 12);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PIN, fri52, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SECURITY_INFO, "2600000000000000", 16);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_ORI_DATA, fri61, strlen(fri61));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

     //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(INFO, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(INFO, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //签购单处理
    pos_sales_slip(pool, rt->datapool);
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, ref_code);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static ngx_int_t
pos_iso8583_pay_reveral()
{
    ngx_iso8583_t *iso;
    ngx_pool_t *pool;
    ngx_socket_t s;
    datapool_t *st, *rt;
    char recv_buf[1024] = {0}, hex[2 + 1] = {0}, macblock[8 + 1] = {0}, tpk[32 + 1] = {0},tak[16 + 1] = {0}, tak_hex[8 + 1] = {0};
    char batch_code[6 + 1] = {0}, term_water[6 + 1] = {0}, fri2[19 + 1] = {0}, fri35[37 +1] = {0}, fri52[8 + 1] = {0}, fri60[17 + 1] = {0};
    char org_batch_code[6 + 1] = {0}, org_term_water[6 + 1] = {0}, org_sys_date[4 +1] = {0}, fri61[29 + 1] = {0};
    ngx_int_t length, len, pos, end_len;
    u_char pack[NGX_ISO8583_MAX_MSG_LEN] = {0};
    u_char tmp[NGX_ISO8583_MAX_FIELD_LEN] = {0};
    char mht_code[15 + 1] = {0}, term_code[8 + 1] = {0}, card_code[19 + 1] = {0}, trade_amt[12 + 1] = {0}, mht_conf[1024] = {0}, login_flag[1 + 1] = {0};
    char *buffer, *tpdu, *header, *ref_code, *passwd, *mht_type;
    char* p;
    char *trade_type = "消费冲正";

    pool = ngx_create_pool(NGX_POOL_DEFAULT_SIZE);
    if (pool == NULL) {
        return NGX_ERROR;
    }
    st = ngx_pcalloc(pool, sizeof(datapool_t));
    if (st == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    st->pool = pool;
    st->datapool = ngx_create_datapool(st->pool, NGX_DATAPOOL_POSP_SIZE);
    if (st->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt = ngx_pcalloc(pool, sizeof(datapool_t));
    if (rt == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    rt->pool = pool;
    rt->datapool = ngx_create_datapool(rt->pool, NGX_DATAPOOL_POSP_SIZE);
    if (rt->datapool == NULL) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    iso = ngx_create_iso8583(pool, 64);
    if (iso == NULL) {
        ErrorLog(ERROR, "create iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    memcpy(iso->fld, (char*)ums8583def_test, sizeof(ngx_iso8583_field_t) * (64 + 1));
    //设置长度字段为ASCII格式
    //ngx_iso8583_set_field_len_format(iso, NGX_ISO8583_LEN_ASCII);

    buffer = ngx_pcalloc(pool, atoi(pos_get_option("buffer-size")));
    if (buffer == NULL) {
        ErrorLog(ERROR, "create communicate buffer error\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    p = buffer;

    //设置TPDU
    asc2hex(pos_get_option("tpdu"), TPDU_LEN * 2, 0, p + TCP_HEADER_LEN);
    //设置报文头
    asc2hex(pos_get_option("header"), PACKET_HEADER_LEN * 2, 0, p + TCP_HEADER_LEN + TPDU_LEN);
    p = p + TCP_HEADER_LEN + TPDU_LEN + PACKET_HEADER_LEN;

    //获取参考号
    ref_code = pos_get_option("ref_code");
    if (ref_code == NULL) {
        ErrorLog(ERROR, "该交易缺少交易参数ref_code");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //获取mht_conf的参数
    sprintf(mht_conf, "%s/%s/%s_%s.conf",getenv("HOME"), MHT_CONF_PATH, pos_get_option("mht"), pos_get_option("term"));
    ngx_get_conf(mht_conf, "current_login_flag", login_flag);
    if (strcmp(login_flag, "1") != NGX_OK) {
        ngx_destroy_pool(pool);
        ErrorLog(ERROR, "该终端未签到,请签到");
        printf("该终端未签到,请签到\n");
        return NGX_ERROR;
    }
    ngx_get_conf(mht_conf, "current_batch_code", batch_code);
    ngx_get_conf(mht_conf, "current_term_water", term_water);
    ngx_get_conf(mht_conf, "ztpk", tpk);
    ngx_get_conf(mht_conf, "ztak", tak);

    //查询原交易商户号终端号
    if (GetConf(mht_conf, ref_code, "ref_code_mht_code", mht_code) != NGX_OK) {
        ErrorLog(ERROR, "未找到这笔交易, 请确认这笔交易的参考号");
        printf("未找到这笔交易, 请确认该这交易的参考号\n");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    GetConf(mht_conf, ref_code, "ref_code_term_code", term_code);
    //查询原交易卡号和交易金额
    GetConf(mht_conf, ref_code, "ref_code_card_code", card_code);
    GetConf(mht_conf, ref_code, "ref_code_trade_amt", trade_amt);
    //查询原交易批次号流水号日期
    GetConf(mht_conf, ref_code, "ref_code_batch_code", org_batch_code);
    GetConf(mht_conf, ref_code, "ref_code_term_water", org_term_water);
    GetConf(mht_conf, ref_code, "ref_code_sys_date", org_sys_date);

    //填充60域
    sprintf(fri60, MSG_TYPE_PAY_REVERSAL, MSG_TYPE_LEN); //设置消息类型
    sprintf(fri60 + 2, batch_code, BATCH_CODE_LEN); //设置批次号
    sprintf(fri60 + 8, NET_CODE_DEFAULT, NET_CODE_LEN); //设置网络管理信息码
    sprintf(fri60 + 11, INPUT_ABILITY, 1); //设置终端读取能力
    sprintf(fri60 + 12, CONDITION_CODE, 1); //设置基于PBOC借/贷记标准的IC卡条件代码

    //测试打包
    ngx_iso8583_del_all_bit(iso);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MSG_TYPE, "0400", 4);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CARD_CODE, card_code, strlen(card_code));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_PROC_CODE, "000000", 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TRADE_AMT, trade_amt, strlen(trade_amt));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERM_WATER, org_term_water, 6);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SERVICE_CODE, "00", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_RESP_CODE, "96", 2);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    mht_type = pos_get_option("mht_type");
    if(mht_type[0] == 'm' || mht_type[0] == 'M' ) {
        ngx_iso8583_set_bit(iso, NGX_DATAPOOL_SALES_SLIP, "mpos15601627165", 15);
    }
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_iso8583_set_bit(iso, NGX_DATAPOOL_MAC, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

    //打包整个8583报文
    if (ngx_iso8583_to_str(p, iso, &len) != NGX_OK) {
        ErrorLog(ERROR, "pack iso8583 error");
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }

    //计算mac(ecb mac)
    asc2hex(tak, 16, 0, tak_hex);
    ngx_calc_ecb_mac(macblock, p, len - 8, tak_hex, '0');
    memcpy(p + len - 8, macblock, 8);

    //加入到数据池里面
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MSG_TYPE,  "0400", 4);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CARD_CODE,  card_code, strlen(card_code));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_PROC_CODE,  "000000", 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_AMT,  trade_amt, strlen(trade_amt));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERM_WATER, org_term_water, 6);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_INPUT_METHOD, "021", 3);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TERMINAL_CODE, term_code, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MERCHANT_CODE, mht_code, 15);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_CURRENCY_CODE, "156", 3);
    if(mht_type[0] == 'm' || mht_type[0] == 'M' ) {
        ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_SALES_SLIP, "mpos15601627165", 15);
    }
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_FRI60, fri60, strlen(fri60));
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_MAC, macblock, 8);
    ngx_add_buf_to_datapool(st->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));

    //计算整个报文长度
    len = len + PACKET_HEADER_LEN + TPDU_LEN;
    ngx_int_to_c2(buffer, len);
    len = len + TCP_HEADER_LEN;

    //发到通道前打印所要发送的内容
    ErrorLog(ERROR, "ngx_iso8583_test send channel");
    ErrorLog_buf(INFO, buffer, len );
    ngx_disp_datapool(st->datapool);

    //连接通道
    s = pos_connect_server();
    if (s < 0) {
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //发送报文
    if (pos_send_handler(s, buffer, len) != 0) {
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    //pos机器流水号自增1
    change_term_water();
    //接收报文
    length = pos_recv_handler(s, recv_buf);
    if( length == -1) {
        ErrorLog(ERROR, "后台通讯超时");
        ngx_close_socket(s);
        ngx_destroy_pool(pool);
        return NGX_ERROR;
    }
    ngx_close_socket(s);

    ngx_iso8583_del_all_bit(iso);
    //打印接收到的报文
    ErrorLog(ERROR, "ngx_iso8583_test recv channel");
    ErrorLog_buf(INFO, recv_buf, length);
    //解析8583报文
    ngx_protocol_unpack_iso8583(rt->datapool, iso, recv_buf + 13);
    ngx_add_buf_to_datapool(rt->datapool, NGX_DATAPOOL_TRADE_TYPE, trade_type, strlen(trade_type));
    //打印8583报文
    ngx_disp_datapool(rt->datapool);

    //储存交易结果
    trade_result_save(rt->datapool, trade_type, ref_code);
    ngx_destroy_pool(pool);
    return NGX_OK;
}


static int
pos_parse_option(int argc, char** argv)
{
    int i, n, flag = 0;
    char *p, *arg;
    pos_cmd_option_t* opt;
    pos_cmd_t *c;

    if (argc == 1) {    //表明不带任何选项命令
        pos_help();
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        p = argv[i];
        if (*p != '-') {//非选项
            //查找对应的命令
            for (n = 0; ;n++) {
                c = &cmd_table[n];
                if (c->name == NULL) {
                    break;
                }
                if (strcmp(p, c->name) == 0) {
                    break;
                }
            }
            if (c->name == NULL) {
                printf("undefined command %s \n", argv[i]);
                exit(1);
            }
            cmd = c;
        }
        else if (*p == '-') {//选项
            if (*(p + 1) == '-') {
                p = p + 2;  //长选项
                flag = 1;
            }
            else {
                p =  p + 1;//短选项
            }
            for (n = 0; ;n++) { //查找选项配置
                opt = &cmd_option[n];
                if (opt->cmd == NULL) {
                    break;
                }
                if (flag == 1) {
                    if (strcmp(p, opt->optnamel) == 0) {
                        //找到对应长选项
                        break;
                    }
                }
                else {
                    if (strcmp(p, opt->optname) == 0) {
                        //找到对应短选项
                        break;
                    }
                }
            }
            //没有找到都应的选项配置
            if (opt->cmd == NULL) {
                printf("undefined options: %s\n", argv[i]);
                exit(1);
            }
            if (strcmp(opt->optname, "h") == 0) {
                pos_help();
                exit(0);
            }
            //判断该选项是否需要参数
            if (opt->flag == 0) {   //该选项不带参数
                opt->value = "1";   //表明在命令行上设置过该选项
                break;
            }
            i = i + 1;
            if (i >=  argc || argv[i] == NULL) {
                printf("option %s must have arg\n", argv[i - 1]);
                exit(1);
            }
            opt->value = argv[i];
        }
    }
    if (cmd == NULL) {
        printf("have no command found\n");
        exit(1);
    }
    return 0;
}


int
main(int argc, char** argv)
{

    if (pos_parse_option(argc, argv) != 0) {
        return -1;
    }
    if (cmd != NULL) {
        cmd->proc();
    }
    return 0;
}

/***********./pos LOGIN ******************/
/*********./pos VOID_PAY --ref_code [data]*********/
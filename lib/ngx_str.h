#ifndef _NGX_STR_H_
#define _NGX_STR_H_

#include "ngx_core.h"

// ngx_str_t来表示字符串，切记不能把data当做字符串处理，data并没有规定以\0结尾
// data+len 才代表字符串，所以如果把data当做字符串处理，有可能导致内存越界。
// 不使用字符串能有效降低内存使用量。
typedef struct {
    size_t      len;  //字符串的有效长度
    u_char     *data; //字符串的内容，指向字符串的起始位置
} ngx_str_t;


#define ngx_memmove(dst, src, n)   (void) memmove(dst, src, n)

#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, NULL }
#define ngx_str_set(str, text)                                               \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = NULL

#define ngx_value_helper(n)   #n
#define ngx_value(n)          posp_value_helper(n)

#define ngx_strlen(s)       strlen((const char *) s)
#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))


void asc_to_bcd ( unsigned char *, unsigned  char *, int, unsigned char );
void bcd_to_asc ( unsigned char *, unsigned  char *, int, unsigned char );
int	get_str( char * p , char * str , char * out );
int skip_line( FILE * fp );
int get_line( FILE * fp , char * line );
int is_view_letter( int ch );
char* get_word( char * in , char * word );
char* get_first_view( char * in );
void trimspace( char * str );
void delspace( char * str );
char* my_strtok ( char *s, const char *delim);
int asc2hex(const u_char* asc, int len, u_char type, u_char* hex);
int hex2asc(const u_char* hex, int len, u_char type, u_char* asc);
int byte2bcd(int num, char* ch) ;
int bcd2byte(char ch, int* num);
int int2bcd(char* bcd, int num);
int bcd2int(const char* bcd, int *num);
int ngx_strcasecmp(u_char *s1, u_char *s2);
void ngx_separate_string(char* str, char sep, ngx_array_t* array);
u_char * cpystrn(u_char *dst, u_char *src, size_t n);
int ngx_c2_to_int(const u_char* hex);
int c4_to_int(u_char* hex);
void int_to_c4 (u_char* hex, const int n);
void ngx_trim_char(char* s, char c);
void ngx_trim_left_char(char* s, char c);
void ngx_trim_right_char(char* s, char c);
void ngx_trim(char *s);
u_char calc_lrc(u_char* data, size_t size);
ssize_t atosz(u_char *line, size_t n);
int ngx_atoi(const u_char* str, size_t n);
ngx_int_t ngx_hatoi(const u_char* str, size_t n);
void Hex2Str(char *sSrc,  char *sDest, int nSrcLen);
void Hex2Str(char *sSrc,  char *sDest, int nSrcLen);
void Str2Hex(char *sSrc, char *sDest, int nSrcLen);
void AddString(char *d, int dlen, char *s, int slen);
char MinString(char *d, int dlen, char *s, int slen);
int StrToLower(char   *pchStr);
int toupper_str(char* str, int len);
int istruealnum(char c);
#endif

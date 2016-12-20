#ifndef _NGX_CRYPT_H_
#define _NGX_CRYPT_H_

#define NGX_DES_CRYPT		0
#define NGX_DES_DECRYPT	1


void ngx_des(unsigned char *boutput, const unsigned char *binput, const unsigned char *bkey, int direction);
void ngx_3des(unsigned char *boutput, const unsigned char *binput, const unsigned char *bkey, int direction);
void ngx_calc_x99_mac(u_char* macblock, const u_char* msg, int len, const u_char* key, char flag);
void ngx_calc_ecb_mac(u_char* macblock, const u_char* msg, int len, const u_char* key, char flag);
void ngx_md5(const u_char* data, int len, u_char* md5);

#if defined(NGX_USE_OPENSSL)

#define NGX_SHA1_SIZE		20
ngx_int_t ngx_rsa_encrypt(char* rsakey, const unsigned char* in, ngx_int_t inlen, unsigned char* out, ngx_int_t *outlen);
ngx_int_t ngx_sha1(const unsigned char* in, ngx_int_t len, unsigned char* md);
ngx_int_t ngx_sha1_with_rsa(char* rsakey, const unsigned char* in, ngx_int_t inlen, unsigned char* out, ngx_int_t *outlen);
#endif

#endif
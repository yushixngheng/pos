#include "ngx_core.h"

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(NGX_USE_OPENSSL)

#undef boolean	//在openssl/ans1.h中boolean 为结构变量名称，这里需要取消定义

#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#endif

#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


static unsigned char T1[] =
{
	57,49,41,33,25,17, 9, 1,
	59,51,43,35,27,19,11, 3,
	61,53,45,37,29,21,13, 5,
	63,55,47,39,31,23,15, 7,
	56,48,40,32,24,16, 8, 0,
	58,50,42,34,26,18,10, 2,
	60,52,44,36,28,20,12, 4,
	62,54,46,38,30,22,14, 6
};

static unsigned char T2[] =
{
	39, 7,47,15,55,23,63,31,
	38, 6,46,14,54,22,62,30,
	37, 5,45,13,53,21,61,29,
	36, 4,44,12,52,20,60,28,
	35, 3,43,11,51,19,59,27,
	34, 2,42,10,50,18,58,26,
	33, 1,41, 9,49,17,57,25,
	32, 0,40, 8,48,16,56,24
};

static unsigned char T3[] =
{
	31, 0, 1, 2, 3, 4,
	 3, 4, 5, 6, 7, 8,
	 7, 8, 9,10,11,12,
	11,12,13,14,15,16,
	15,16,17,18,19,20,
	19,20,21,22,23,24,
	23,24,25,26,27,28,
	27,28,29,30,31, 0
};

static unsigned char T5[] =
{
	15, 6,19,20,
	28,11,27,16,
	 0,14,22,25,
	 4,17,30, 9,
	 1, 7,23,13,
	31,26, 2, 8,
	18,12,29, 5,
	21,10, 3,24
};

static unsigned char T7_1_2[] =
{
	56,48,40,32,24,16, 8,
	 0,57,49,41,33,25,17,
	 9, 1,58,50,42,34,26,
	18,10, 2,59,51,43,35,
	62,54,46,38,30,22,14,
	 6,61,53,45,37,29,21,
	13, 5,60,52,44,36,28,
	20,12, 4,27,19,11, 3
};

static unsigned char T8[] =
{
	 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0
};

static unsigned char T9[] =
{
	13,16,10,23, 0, 4,
	 2,27,14, 5,20, 9,
	22,18,11, 3,25, 7,
	15, 6,26,19,12, 1,
	40,51,30,36,46,54,
	29,39,50,44,32,47,
	43,48,38,55,33,52,
	45,41,49,35,28,31
};

static unsigned char T6[][64] =
{
    {/* S1 */
	14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7,
	 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8,
	 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,
	15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13
    },
    {/* S2 */
	15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10,
	 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5,
	 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,
	13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9
    },
    {/* S3 */
	10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,
	13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7,
	 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12
    },
    {/* S4 */
	 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,
	13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,
	10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4,
	 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14
    },
    {/* S5 */
	 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,
	14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6,
	 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,
	11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3
    },
    {/* S6 */
	12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,
	10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8,
	 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6,
	 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13
    },
    {/* S7 */
	 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,
	13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6,
	 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2,
	 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12
    },
    {/* S8 */
	13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7,
	 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2,
	 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8,
	 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11
    }
};

unsigned char TE[][4] =
{
	{0,0,0,0},
	{0,0,0,1},
	{0,0,1,0},
	{0,0,1,1},
	{0,1,0,0},
	{0,1,0,1},
	{0,1,1,0},
	{0,1,1,1},
	{1,0,0,0},
	{1,0,0,1},
	{1,0,1,0},
	{1,0,1,1},
	{1,1,0,0},
	{1,1,0,1},
	{1,1,1,0},
	{1,1,1,1}
};

void  Ks (unsigned char *Key, unsigned char Kn[16][48])
{
	unsigned char cd[56];
	unsigned char zt[60];
	int n, i;
	unsigned char tmp11, tmp12, tmp21, tmp22;
	unsigned char *Knn;

	/* choix 1 */
	for (i = 0; i < 56; i++)
	{
		cd[i] = Key[T7_1_2[i]];
	}

	for (n = 0; n < 16; n++)
	{
		if (T8[n] == 0)
		{
			tmp11 = cd[0];
			tmp21 = cd[28];
			memcpy( zt , &cd[1] , 55 );
			memcpy( cd , zt 	, 55 );
			cd[27] = tmp11;
			cd[55] = tmp21;
		}
		else
		{
			tmp11 = cd[0];
			tmp12 = cd[1];
			tmp21=  cd[28];
			tmp22 = cd[29];

			memcpy( zt , &cd[2] , 54 );
			memcpy( cd , zt 	, 54 );

			cd[26] = tmp11;
			cd[27] = tmp12;
			cd[54] = tmp21;
			cd[55] = tmp22;
		}
		Knn = Kn[n];
		for (i = 0; i < 48; i++)
		{
			Knn[i] = cd[T9[i]];
		}
	}
}

void fonction(unsigned char *Knn, unsigned char *r, unsigned char *s)
{

     unsigned char x[32];
     unsigned int *px;
     int i, l;
     unsigned char c;
     unsigned char t;
    for (i = 0, l = 0, px = (unsigned int *) x; i < 8;)
	{
		c = 32 * (r[T3[l]] ^ Knn[l]);
        l++;
		c += 8 * (r[T3[l]] ^ Knn[l]);
        l++;
		c += 4 * (r[T3[l]] ^ Knn[l]);
        l++;
		c += 2 * (r[T3[l]] ^ Knn[l]);
        l++;
		c += 1 * (r[T3[l]] ^ Knn[l]);
        l++;
		c += 16 * (r[T3[l]] ^ Knn[l]);
        l++;
        t = T6[i][c];
        i++;
		*px = *(int *)TE[t];
        px++;
    }
	for (i = 0; i < 32; i++)
	{
		s[i] = x[T5[i]];
	}
}

void permutation(unsigned char *org, unsigned char *tab)
{
     unsigned char tmp[64];
     int i;


	memcpy(tmp, org, 64);
	for (i = 0; i < 64; i++)
	{
		org[i] = tmp[tab[i]];
	}
}

void chiffrement(unsigned char *xi, unsigned char *xo, unsigned char Kn[16][48])
{
     unsigned char r[32], l[32];
     unsigned char rp[32], lp[32];

     int i;
	 int n;

	memcpy(l, &xi[0], 32);
	memcpy(r, &xi[32], 32);

	for (n = 0; n < 16; n++)
	{
		memcpy(lp, r, 32);

		fonction(Kn[n], r, rp);
		for (i = 0; i < 32; i++)
		{
			r[i] =( ( l[i]) ^ (rp[i] )  ) ;
		}
		memcpy(l, lp, 32);
	}
	memcpy(&xo[0], r, 32);
	memcpy(&xo[32], l, 32);

}

void dechiffrement(unsigned char *xi, unsigned char *xo, unsigned char Kn[16][48])
{
     unsigned char r[32], l[32], rp[32], lp[32];

     int i;
     int n;

	memcpy(l, &xi[0], 32);
	memcpy(r, &xi[32], 32);

	for (n = 0; n < 16; n++)
	{
		memcpy(lp, r, 32);
		fonction(Kn[15 - n], r, rp);
		for (i = 0; i < 32; i++)
		{
			r[i] =( ( l[i] ) ^ ( rp[i] )) ;
		}
		memcpy(l, lp, 32);
	}
	memcpy(&xo[0], r, 32);
	memcpy(&xo[32], l, 32);
}

void eclater(const unsigned char *buf_bit, unsigned char *byte)
{
     int i;
     unsigned char m;

	for (i = 0; i < 8; i++)
	{
		for (m = 0x80; m != 0;	)
		{
            if ((buf_bit[i] & m) != 0)
				*byte = 1;
			else
				*byte = 0;
            byte++;
            m=m/2 ;
		}
	}

}

void compacter(unsigned char *byte, unsigned char *buf_bit)
{
	 int i;
	 unsigned char m, n;

	for (i = 0; i < 8; i++)
	{
		n = 0;
		for (m = 0x80; m != 0; )
		{
			if (*byte++)
			n = n | m;
				   m=m/2 ;

		}
		buf_bit[i] = n;
	}
}


void ngx_des(unsigned char *boutput, const unsigned char *binput, const unsigned char *bkey, int direction) {
	unsigned char  input[64];	/* data input */
	unsigned char  output[64];	/* data output */
	unsigned char  Key[64];
	unsigned char  Kn[16][48];
	eclater(binput, input);
	eclater(bkey, Key);
	Ks(Key, Kn);
	permutation(input, T1);
	if (direction)  dechiffrement(input, output, Kn);
	else  chiffrement(input, output, Kn);
	permutation(output, T2);
	compacter(output, boutput);
}

//双倍长3des算法(binput是单倍长数据)
void ngx_3des(unsigned char *boutput, const unsigned char *binput, const unsigned char *bkey, int direction) {
	char tmp[17],tmp2[17];

	ngx_des(tmp, binput , bkey ,direction);
	ngx_des(tmp2, tmp , bkey+8 ,1-direction);
	ngx_des(boutput, tmp2 , bkey ,direction);

}

//双倍长3des算法(binput是双倍长数据)
void ngx_3des2(unsigned char *boutput, const unsigned char *binput, const unsigned char *bkey, int direction) {
	char tmp[33],tmp2[33];

	ngx_des(tmp, binput , bkey ,direction);
	ngx_des(tmp2, tmp, bkey,1-direction);
	ngx_des(boutput, tmp2 , bkey ,direction);

	ngx_des(tmp, binput + 8, bkey ,direction);
	ngx_des(tmp2, tmp, bkey+8 ,1-direction);
	ngx_des(boutput + 8, tmp2 , bkey ,direction);

}



//ANSI X9.9MAC算法(cbc mac)
void ngx_calc_x99_mac(u_char* macblock, const u_char* msg, int len, const u_char* key, char flag) {
    int   i, j;
    memset(macblock, 0, 8);
    ErrorLog_buf(INFO, msg, len);
	for(i=0; i<((len)/8); i++) {
		for(j=0;j<8;j++) {
			macblock[j] ^= msg[i*8+j];
		}
		if(flag == '0')
	    	ngx_des(macblock, macblock, key, NGX_DES_CRYPT);
		else
	    	ngx_3des(macblock, macblock, key, NGX_DES_CRYPT);
	}
	if((len%8)!=0) {
		for(j=0;j < (len % 8) ; j++)
			macblock[j] ^= msg[i*8+j];

		if(flag == '0')
	    	ngx_des(macblock, macblock, key, NGX_DES_CRYPT);
		else
	    	ngx_3des(macblock, macblock, key, NGX_DES_CRYPT);
	}
}



void ngx_calc_ecb_mac(u_char* macblock, const u_char* msg, int len, const u_char* key, char flag) {
	char data[8] = {0}, data1[8 + 1] = {0};
	char mac_hex[8 + 1] = {0};
	ngx_int_t i;
	char mb[16 + 1] = {0};

	for (i = 0; i < len; i++) {
		data[(i&0x07)] ^= msg[i];
	}
	hex2asc(data, 16, 0, mb);
	ErrorLog(INFO, "mb[%s]", mb);
	memcpy(data , mb, 8);
	ErrorLog_buf(INFO, data, 8);
	if (flag == '0') {
		ngx_des(data1, data, key, NGX_DES_CRYPT);
	} else {
		ngx_3des(data1, data, key, NGX_DES_CRYPT);
	}
	ErrorLog(INFO, "the frist crypt");
	ErrorLog_buf(INFO, data1, 8);
	for (i = 0; i < 8; i++) {
		data1[i] ^= mb[8 + i];
	}
	if (flag == '0') {
		ngx_des(mac_hex, data1, key, NGX_DES_CRYPT);
	} else {
		ngx_3des(mac_hex, data1, key, NGX_DES_CRYPT);
	}

	ErrorLog(INFO, "the second crypt");
	ErrorLog_buf(INFO, mac_hex, 8);
	hex2asc(mac_hex, 8, 0, macblock);
	return;
}



void ngx_md5(const u_char* data, int len, u_char* md5) {
	u_char md[16 + 1] = {0};
	MD5_CTX mdc;

	MD5_Init(&mdc);
	MD5_Update(&mdc, data, len);
	MD5_Final(md, &mdc);

	hex2asc(md, 32 , 0, md5);
}

#if defined(NGX_USE_OPENSSL)
ngx_int_t ngx_rsa_encrypt(char* rsakey, const unsigned char* in, ngx_int_t inlen, unsigned char* out, ngx_int_t *outlen) {
	char rk[140] = {0};
	RSA* k1;
	ngx_int_t num;
	const unsigned char* p;

	asc2hex(rsakey, strlen(rsakey), 0, rk);
	p = rk;
	k1 = d2i_RSAPublicKey(NULL, &p, strlen(rsakey) / 2);
	if (k1 == NULL) {
		return NGX_ERROR;
	}
	num = RSA_public_encrypt(inlen, in, out, k1, RSA_PKCS1_PADDING);
	RSA_free(k1);
	if (num < 0) {
		return NGX_ERROR;
	}
	*outlen = num;
	return NGX_OK;
}

ngx_int_t ngx_sha1(const unsigned char* in, ngx_int_t len, unsigned char* md) {
	if (SHA1(in, len, md) == NULL) {
		return NGX_ERROR;
	}
	return NGX_OK;
}

ngx_int_t ngx_sha1_with_rsa(char* rsakey, const unsigned char* in, ngx_int_t inlen, unsigned char* out, ngx_int_t *outlen) {
	unsigned char md[20] = {0};

	if (ngx_sha1(in, inlen, md) != NGX_OK) {
		return NGX_ERROR;
	}

	if (ngx_rsa_encrypt(rsakey, md, 20, out, outlen) !=  NGX_OK) {
		return NGX_ERROR;
	}
	return NGX_OK;
}

ngx_int_t ngx_rsa_decrypt(char* rsakey, char* in, ngx_int_t inlen, char* out, ngx_int_t *outlen) {

	return NGX_OK;
}

#endif
















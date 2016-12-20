#ifndef _DBDES_H
#define _DBDES_H


char hextoasc11(int);
char hexhightoasc11(int);
char hexlowtoasc11(int);
char asctohex11(char, char);
int bcdhex_to_aschex11(char bcdhex[], int len, char aschex[]);
int aschex_to_bcdhex11(char [], int, char []);
int IntToBcd11(int, char *);
int byte2int_to_bcd11(int, char []);
void AscBcd11(char *buf, int len, char *result);
void BcdAsc11(char *buf, int len, char *result);
void xor11(char *a,  char *b,  char *out, int bytes);


void Des(unsigned char *key, unsigned char *text, unsigned char *mtext);
void TDes(unsigned char *key, unsigned char *text, unsigned char *mtext);

void _Des(unsigned char *key, unsigned char *text, unsigned char *mtext);
void _TDes(unsigned char *key, unsigned char *text, unsigned char *mtext);

void encrypt0(unsigned char *text, unsigned char *mtext);
void discrypt0(unsigned char *mtext, unsigned char *text);
void expand0(unsigned char *in, unsigned char *out);
void compress0(unsigned char *out, unsigned char *in);
void compress016(unsigned char *out, unsigned char *in);
void setkeystar(unsigned char *bits);
void LS(unsigned char *bits, unsigned char *buffer, int count);
void son(unsigned char *cc, unsigned char *dd, unsigned char *kk);
void ip(unsigned char *text, unsigned char *ll, unsigned char *rr);
void _ip(unsigned char *text, unsigned char *ll, unsigned char *rr);
void F(int n, unsigned char *ll, unsigned char *rr, unsigned char *LL, unsigned char *RR);
void s_box(unsigned char *aa, unsigned char *bb);

int DB_DES(char *tt, int ss, char *dd, int *ee, int ff);

#endif

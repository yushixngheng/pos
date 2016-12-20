/*
 * Name:    str.c
 */
#include "ngx_str.h"


void
asc_to_bcd ( unsigned char *bcd_buf, unsigned char *ascii_buf, int conv_len, unsigned char type )
{

int    	cnt;
char   	ch, ch1;

	if ( conv_len & 0x01 && type )
		ch1 = 0;
	else
		ch1 = 0x55;
	for ( cnt = 0; cnt < conv_len; ascii_buf ++, cnt ++ )
	{
		if ( *ascii_buf >= 'a' )
			ch = *ascii_buf - 'a' + 10;
		else if ( *ascii_buf >= 'A' )
			ch = *ascii_buf - 'A' + 10;
		else if ( *ascii_buf >= '0' )
			ch = *ascii_buf - '0';
		else
			ch = 0;
		if ( 0x55 == ch1 )
			ch1 = ch;
		else
		{
			*bcd_buf ++ = ch1 << 4 | ch;
			ch1 = 0x55;
		}
	}
	if ( ch1 != 0x55 )
		*bcd_buf = ch1 << 4;

	return;

}


/*void
bcd_to_asc(int n,char * p){
      int i, k = 0;
      int len = master[n].length_in_byte;

      for (i = 0; i < len/2; i++) {
            p[k++] = (master[n].data[i * 2] & 0x0f) * 16 +(master[n].data[i*2+1] & 0x0f);
      }

      p[k++] = '\0';
      return ;
}
*/
void
bcd_to_asc ( unsigned char *ascii_buf, unsigned char* bcd_buf, int conv_len, unsigned char type )

{

int 	cnt;

    	if ( conv_len & 0x01 && type ) 
	{
		cnt = 1;
		conv_len ++;
	}
    	else
		cnt = 0;

	for ( ; cnt < conv_len; cnt ++, ascii_buf ++ ) 
	{
		*ascii_buf = ( ( cnt & 0x01 ) ? ( *bcd_buf ++ & 0x0f ) : ( *bcd_buf >> 4 ) );
		*ascii_buf += ( ( *ascii_buf > 9 ) ? ( 'A' - 10 ) : '0' );
    	}

	return;

}


int
get_str( char * p , char * str , char * out )
{
	char 	* pos1 , * pos2 ;

	pos1 = strstr( p , str ) ;
	if( pos1 == NULL)
		return -5 ;

	pos1 += ( strlen( str ) + 2 ) ;
	if( *( pos1 -1 ) != '[' )
		return -5 ;

	pos2 = strchr( pos1 , ']' ) ;
	if( pos2 == NULL)
		return -5 ;

	memcpy( out , pos1 , ( pos2 - pos1 ) ) ;
	out[ pos2 - pos1 ] = 0 ;

	return 0 ;
}


int
skip_line( FILE * fp )
{
	int ret ;

	while( 1 )
	{
		if( feof( fp ) )
		{
			return( -1 ) ;
		}

		ret = fgetc( fp ) ;
		if( ret < 0 )
		{
			return( -1 ) ;
		}

		if( ret == '\n' )
		{
			return 0 ;
		}
	}
}


int
get_line( FILE * fp , char * line )
{
	int ret ;

	while( 1 )
	{
		if( feof( fp ) )
		{
			return( -1 ) ;
		}
		ret = fgetc( fp ) ;
		if( ret < 0 )
		{
			return( -1 ) ;
		}

		if( ret == '\n' )
		{
			* line = 0 ;
			return 0 ;
		}

		* line = ret ;
		line ++ ;
	}
}


/* if letter is view */
int
is_view_letter( int ch )
{
	/* 0x20 -- space : ' ' */
	/* 0x09 -- tab	: '	' */

	if( ch == 0x20 || ch == 0x9 )
		return 0 ;

	if( isprint( ch ) )
		return 1 ;
	else
		return 0 ;
}


/* return the next word point */
char*
get_word( char * in , char * word )
{
	char	ch	;

	while( 1 )
	{

		ch = * in ;
		if( is_view_letter( ch ) )
			break ;
		else
			;

		if( ch == 0 || ch == '\n'  )
			break ;
		in ++ ;
	}

	while( 1 )
	{
		/*
		if( ( ch >= 'a' && ch <= 'z' ) ||
		    ( ch >= 'A' && ch <= 'Z' ) ||
		    ( ch >= '0' && ch <= '9' ) ||
		    ( ch == '_' || ch =='-' ) ||
		    ( ch == '+' || ch =='=' ) ||
		    ( ch == '[' || ch ==']' ) ||
		    ( ch == '|' || ch =='\\' ) ||
		    ( ch == ':' || ch ==';' ) ||
		    ( ch == '"' || ch =='\'' ) ||
		    ( ch == '<' || ch ==',' ) ||
		    ( ch == '>' || ch =='.' ) )
		*/

		if( is_view_letter( * in ) )
			;
		else
		{
			* word = 0 ;
			return ( in + 1 ) ;
		}

		* word = * in ;
		word ++ ;
		in ++ ;
	}
}


/* return the first view word point */
char*
get_first_view( char * in )
{
	char	ch	;

	while( 1 )
	{

		ch = * in ;
		if( is_view_letter( ch ) )
			break ;
		else
			;

		if( ch == 0 || ch == '\n'  )
			break ;
		in ++ ;
	}

	return in ;
}


void
trimspace( char * str )
{
	char * p ;
	if( * str == '\0' ) return ;
	p = str + strlen( str ) - 1 ;

	while( 1 )
	{
		if( * p == ' ' )
		{
			* p = '\0' ;
			p -- ;
		}
		else
			break ;
	}
	printf("str = %s\n", str);
}


void
delspace( char * str )
{
	char * p1 , * p2 ;
	if( * str == '\0' ) return ;
	p1 = p2 = str ;

	while( 1 )
	{
		if( * p2 == '\0' )
			break ;
		if( * p2 != ' ' )
		{
			*p1 = *p2 ;

		}
		else
		{
			p2 ++ ;
			continue ;
		}

		p1 ++ ;
		p2 ++ ;
	}

	* p1 = '\0' ;
}


/***ugly code by steve, bad example
char * my_strtok( char *s, int c  )
{
	static char * nextptr;
	char *tmp;
	if(s !=  NULL)
	{
		tmp = strchr(s, c);
		if( tmp != NULL )
		{
			*tmp = '\0';
			nextptr = tmp + 1;
			return s;
		}
		else
		{
			return s;
		}
	}
	else
	{
		tmp = strchr( nextptr, c );
		if( tmp != NULL )
		{
			*tmp = '\0';
}

****/


/** original code from glibc-2.9 under GPL license **/
extern void *rawmemchr (__const void *__s, int __c)
     __THROW __attribute_pure__ __nonnull ((1));
/*
 * 功能和标准strtok()相近，但是strtok()将相邻连续的分隔符都看做一个分隔符处理。
 * my_strtok()如此处理：解析完一个token后，如果余下的字符串首为一个分隔符，
 * 则将此分隔符置为'\0'，并返回其地址，也就是返回了一个空字符串。
 *
 */

char*
my_strtok ( char *s, const char *delim)
{
#undef strtok
	static char *olds;
	char *token;

	if (s == NULL)
	s = olds;

	/* Scan leading delimiters.  */
	/*s += strspn (s, delim); */ /* comment by steve */
	if (*s == '\0')
	{
		olds = s;
		return NULL;
	}

	/* Find the end of the token.  */
	token = s;
	s = strpbrk (token, delim);
	if (s == NULL)
		/* This token finishes the string.  */
		olds = rawmemchr (token, '\0'); // __rawmemchr ---> rawmemchr  Modified by steve
	else
	{
		/* Terminate the token and make OLDS point past it.  */
		*s = '\0';
		olds = s + 1;
	}
	return token;
}


int
asc2hex(const u_char* asc, int len, u_char type, u_char* hex)
{
    int i = 0;
    char tmp, tmp1;

    if (asc == NULL || hex == NULL || len <= 0) {
        return -1;
    }

    if (len & 0x01 && type)/*判别是否为奇数以及往那边对齐*/ {
        tmp1 = 0 ;
    }
    else {
        tmp1 = 0x55 ;
    }

    for (i = 0; i < len; asc++, i++) {
        if ( *asc >= 'a' ) {
            tmp = *asc - 'a' + 10 ;
        }
        else if ( *asc >= 'A' ) {
            tmp = *asc - 'A' + 10 ;
        }
        else if ( *asc >= '0' ) {
            tmp = *asc - '0' ;
        }
        else {
            tmp = *asc;
            tmp&=0x0f;
         }

        if ( tmp1 == 0x55 ) {
            tmp1 = tmp;
        }
        else {
            *hex ++ = tmp1 << 4 | tmp;
            tmp1 = 0x55;
        }
    }
    if (tmp1 != 0x55) {
        *hex = tmp1 << 4;
    }
    return 0;
}


int
hex2asc(const u_char* hex, int len, u_char type, u_char* asc)
{
    int i = 0;

    if (hex == NULL) {
        return -1;
    }
    if (len & 0x01 && type) /*判别是否为奇数以及往那边对齐,0:左，1:右*/ {
        i = 1;
        len++;
    }
    else {
        i = 0;
    }
    for (; i < len; i++, asc++) {
        if (i & 0x01) {
            *asc = *hex++ & 0x0f;
        }
        else {
            *asc = *hex >> 4;
        }
        if (*asc > 9) {
            *asc += 'A' - 10;
        }
        else {
            *asc += '0';
        }
    }
    *asc = 0;
    return 0;
}


int
byte2bcd(int num, char* ch)
{
    if (ch == NULL) {
        return -1;
    }
    if ((num < 0) || (num > 99)) {
        return -1;
    }
    *ch = ((num / 10) << 4) | (num % 10);
    return 0;
}


int
bcd2byte(char ch, int* num)
{
    if (num == NULL) {
        return -1;
    }
    if (((ch & 0x0F) > 9) || ((ch >> 4) > 9)) {
        return -1;
    }
    *num = (ch >> 4) * 10 + (ch & 0x0f);
    return 0;
}


int
int2bcd(char* bcd, int num)
{
    if (bcd == NULL) {
        return -1;
    }
    if ((num < 0) || (num > 9999)) {
        return -1;
    }
    byte2bcd(num / 100, bcd);
    byte2bcd(num %100, bcd + 1);
    return 0;
}


int
bcd2int(const char* bcd, int *num)
{
    int num1, num2;
    if (bcd == NULL || num == NULL) {
        return 0;
    }
    if (bcd2byte(bcd[0], &num1) != 0) {
        return -1;
    }
    if (bcd2byte(bcd[1], &num2) != 0) {
        return -1;
    }
    *num = num1 * 100 + num2;
    return 0;
}


void
ngx_separate_string(char* str, char sep, ngx_array_t* array) {
    int i, len;
    int begin, now;
    ngx_str_t* s;

    len = strlen(str);
    begin = now = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != sep) {
            continue;
        }
        s = ngx_array_push(array);
        s->data = str + begin;
        s->len = i - begin;
        begin = i + 1;
    }
    s = ngx_array_push(array);
    s->data = str + begin;
    s->len = i - begin;
}


int
ngx_strcasecmp(u_char *s1, u_char *s2)
{
    int  c1, c2;

    for ( ;; ) {
        c1 = (unsigned int) *s1++;
        c2 = (unsigned int) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if (c1 == c2) {

            if (c1) {
                continue;
            }

            return 0;
        }

        return c1 - c2;
    }
}


u_char*
cpystrn(u_char *dst, u_char *src, size_t n)
{
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;

        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}


int
ngx_c2_to_int(const u_char* hex)
{
    return ((*hex) << 8) + (*(hex + 1));
}


void
ngx_int_to_c2 (u_char* hex, int n)
{
    int tmp = n;
    if (tmp <= 65535) {
        *(hex + 1) = n % 256;
        *hex = tmp >>8;
    }
    return;
}


int
c4_to_int(u_char* hex)
{
    return ((*hex) << 24) + (*(hex+1) << 16) + (*(hex+2) << 8) + (*(hex + 3));
}


void
int_to_c4 (u_char* hex, const int n)
{

    int tmp = 0;

    tmp = n;
    if (n <= 4294967295ll) {
        *( hex ) = n >> 24;
        *( hex + 1 ) = (n >> 16) ;
        *( hex + 2 ) = (n >> 8) ;
        *( hex + 3 ) = n %256;
    }

}


void
ngx_trim_char(char* s, char c)
{
    int i, j, k;

    i = strlen(s);
    while ((i > 0) && (s[i-1] == c)) {
        i--;
        s[i] = '\0';
    }

    j = 0;
    while ((j < i) && (s[j] == c)) {
        j++;
    }
    for (k = j; k <= i; k++) {
        s[k-j] = s[k];
    }
}


void
ngx_trim_left_char(char* s, char c)
{
    int i, j, k;
    j = 0;
    i = strlen(s);
    while ((j < i) && (s[j] == c)) {
        j++;
    }
    for (k = j; k <= i; k++) {
        s[k-j] = s[k];
    }
}


void
ngx_trim_right_char(char* s, char c)
{
    int i, k;

    i = strlen(s);
    while ((i > 0) && (s[i-1] == c)) {
        i--;
        s[i] = '\0';
    }
}


void
ngx_trim(char *s)
{
    int i, j, k;

    i = strlen(s);
    while ((i > 0) && (s[i-1] == ' ')) {
        i--;
        s[i] = '\0';
    }

    j = 0;
    while ((j < i) && (s[j] == ' ')) {
        j++;
    }
    for (k = j; k <= i; k++) {
        s[k-j] = s[k];
    }

}


u_char
calc_lrc(u_char* data, size_t size)
{
    int i;
    u_char lrc;
    lrc = 0x00;
    for (i = 0; i < size; i++) {
        lrc = lrc ^ data[i];
    }
    return lrc;
}


ssize_t
atosz(u_char *line, size_t n)
{
    ssize_t  value;

    if (n == 0) {
        return -1;
    }

    for (value = 0; n--; line++) {
        if (*line < '0' || *line > '9') {
            return -1;
        }

        value = value * 10 + (*line - '0');
    }

    if (value < 0) {
        return -1;

    } else {
        return value;
    }
}


int
ngx_atoi(const u_char* str, size_t n)
{

 	int  value;

    if (n == 0) {
        return -1;
    }
    for (value = 0; n--; str++) {
        if (*str < '0' || *str > '9') {
            return -1;
        }
        value = value * 10 + (*str - '0');
    }
    if (value < 0) {
        return -1;
    } else {
        return value;
    }
}


ngx_int_t
ngx_hatoi(const u_char* str, size_t n)
{
    ngx_int_t value;
    if (n == 0) {
        return NGX_ERROR;
    }
    for (value = 0; n--; str++) {
        if (*str >= '0' && *str <= '9') {
            value = value * 16 + (*str - '0');
        }
        else if (*str >= 'a' && *str <= 'f') {
            value = value * 16 + (*str - 'a' + 10);
        }
        else if (*str >= 'A' && *str <= 'F') {
            value = value * 16 + (*str - 'A' + 10);
        }
        else {
            return NGX_ERROR;
        }
    }
    if (value < 0) {
        return NGX_ERROR;
    }
    return value;
}


void
Hex2Str(char *sSrc,  char *sDest, int nSrcLen)
{
    int  i;
    char szTmp[3];

    for(i = 0; i < nSrcLen; i++)
    {
        sprintf(szTmp, "%02X", (unsigned char) sSrc[i]);
        memcpy(&sDest[i * 2], szTmp, 2);
    }

    return ;
}


void
Str2Hex(char *sSrc, char *sDest, int nSrcLen)
{
    int i, nHighBits, nLowBits;

    for(i = 0; i < nSrcLen; i += 2)
    {
        nHighBits = sSrc[i];
        nLowBits  = sSrc[i + 1];

        if(nHighBits > 0x39)
        {
            nHighBits -= 0x37;
        }
        else
        {
            nHighBits -= 0x30;
        }

        if(i == nSrcLen - 1)
        {
            nLowBits = 0;
        }
        else if(nLowBits > 0x39)
        {
            nLowBits -= 0x37;
        }
        else
        {
            nLowBits -= 0x30;
        }

        sDest[i / 2] = (nHighBits << 4) | (nLowBits & 0x0f);
    }

    return ;
}


/********************************************************************
            Module name:    AddString
            Function   :    Add a string into another by value
            Input      :    d: to be added string
                                        dlen: length of dest string
                                        s: source string
                                        slen: length of source string
            Output     :    void
            Create Date:    Mar 1, 1996
            Last Modify:    Aug 6, 1996
********************************************************************/
void
AddString(char *d, int dlen, char *s, int slen)
{
    int i, j, vals;
    int carry = 0, res;

    if((dlen == 0) || (slen == 0))
    {
        return;
    }

    if(dlen < slen)
    {
        return;
    }

    for(i = dlen - 1, j = slen - 1; i >= 0; i--, j--)
    {
        if(j < 0)
        {
            vals = '0';
        }
        else
        {
            vals = s[j];
        }

        res = d[i] - '0' + vals - '0' + carry;
        d[i] = res % 10 + '0';
        carry = res / 10;
    }
}

/********************************************************************
            Module name:    MinString
            Function   :    Min a string into another by value
            Input      :    d: to be mined string
                                        dlen: length of dest string
                                        s: source string
                                        slen: length of source string
            Output     :    the sign of result: '+' or '-'
            Create Date:    Mar 1, 1996
            Last Modify:    Aug 6, 1996
********************************************************************/
char
MinString(char *d, int dlen, char *s, int slen)
{
    int i, j, carry = 0;
    signed char vals, res;

    if((dlen == 0) || (slen == 0))
    {
        return 0;
    }

    if(dlen < slen)
    {
        return 0;
    }

    for(i = dlen - 1, j = slen - 1; i >= 0; i--, j--)
    {
        if(j < 0)
        {
            vals = '0';
        }
        else
        {
            vals = s[j];
        }

        res = d[i] - carry - vals;

        if(res < 0)
        {
            carry = 1;
            res += 10;
        }
        else
        {
            carry = 0;
        }

        d[i] = res + '0';
    }

    return (carry == 1 ? '-' : '+');
}


int
StrToLower(char* pchStr)
{
    int  i;

    for(i = 0; i < strlen(pchStr); i++)
    {
        pchStr[i] = tolower(pchStr[i]);
    }

    return 0;
}


int
toupper_str(char* str, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = toupper(str[i]);
        }
    }
    return 0;
}

/*void toupper_str( char * str )
{
        char * p ;
        p = str ;

        while( 1 )
        {
                * p = toupper( * p ) ;
                p ++ ;
                if( * p == '\0' ) return ;
        }
}*/

int
istruealnum(char c)
{
    if('a' <= c && c <= 'z')
    {
        return 1;
    }

    if('A' <= c && c <= 'Z')
    {
        return 1;
    }

    if('0' <= c && c <= '9')
    {
        return 1;
    }

    return 0;
}

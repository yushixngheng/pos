#include "ngx_core.h"


static ngx_int_t
ngx_change_charset(const char* pFromCode, const char* pToCode, char* pInBuf, size_t* iInLen, char* pOutBuf, size_t* iOutLen )
{
        int iRet;
        //打开字符集转换
        iconv_t hIconv = iconv_open( pToCode, pFromCode );
        if ( (iconv_t)-1 == hIconv ) {
           ErrorLog(ERROR, "open iconv error");
           return -1;//打开失败，可能不支持的字符集 
        }
        //开始转换
        iRet = iconv( hIconv, &pInBuf, iInLen, (char**)(&pOutBuf), iOutLen);
        //关闭字符集转换
        iconv_close( hIconv );
        return iRet;
 }


ngx_int_t
ngx_uni2gbk(u_char* in, int ilen, u_char* out, int* olen)
{
 	int rv;
 	size_t len1, olen1;
 	len1 = ilen;
 	olen1 = *olen;

 	rv = ngx_change_charset("UTF-8", "GBK", in, &len1, out, &olen1);
 	if (rv == -1) {
 		ErrorLog(ERROR, "change charset error");
 		return NGX_ERROR;
 	}
 	*olen = *olen - olen1;
	return NGX_OK;
}


ngx_int_t
ngx_gbk2uni(u_char* in, int ilen, u_char* out, int* olen)
{
    int rv;
    size_t len1, olen1;
    len1 = ilen;
    olen1 = *olen;

    rv = ngx_change_charset("GBK", "UTF-8", in, &len1, out, &olen1);
    if (rv == -1) {
        ErrorLog(ERROR, "change charset error");
        return NGX_ERROR;
    }
    *olen = *olen - olen1;
    return NGX_OK;
}





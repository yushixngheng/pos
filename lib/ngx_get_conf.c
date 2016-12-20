#include "ngx_core.h"

int
ngx_get_conf(char* file, char* val_name, char* value)
{
	FILE *	fp;
	char 	buf1[100], buf2[100];
	char 	ret_str[80];

	//printf("%s\n", getenv("HOME"));
	sprintf(buf1, "%s", file);

	//printf("%s\n", buf1);
	if ((fp = fopen(buf1, "r")) == NULL) {
		ErrorLog( ERROR, "FILE open error" );
		return NGX_ERROR;
	}

	while (!feof(fp)) {
		fgets(buf2, 80, fp);
		if (buf2[0] == '#') continue;
		sscanf(buf2, "%[^=]=%s", buf1, ret_str );
		//ErrorLog(DEBUG, "buf1[%s],ret_str[%s]", buf1, ret_str);
		if (!strcmp(val_name, buf1)) {
			memcpy( value, ret_str, strlen(ret_str));
			fclose( fp );
			return NGX_OK;
		}
	}

	fclose( fp );
	//ErrorLog( ERROR, "get conf error" );
	return  NGX_ERROR;
}


int
read_conf( char *path, char * in , char * out )
{
    FILE    * fp    ;
    char    buf[ 4096 ] , buf1[ 1024 ] ;
    int ret         ;

    sprintf(buf, "%s/%s", getenv("HOME"), path);
    //sprintf( buf , getenv( "CONFIG_FILE" ) ) ;

    fp = fopen( buf , "r" ) ;
    if( fp == NULL )
    {
        return( -1 )    ;
    }

    memset( buf , 0 , sizeof( buf ) ) ;

    while( 1 )
    {
        if( feof( fp ) )
            break ;

        ret = fscanf( fp , "%s" , buf1 ) ;
        if( ret < 0 )
            break ;

        if( buf1[ 0 ] == '#' )
        {
            if( skip_line( fp ) < 0 )
                break ;
            continue ;
        }

        strcat( buf , buf1 ) ;
    }

    fclose( fp ) ;
    if( get_str( buf , in , out ) < 0 )
        return -1 ;

    return 0 ;
}

/*
 *  出除字符串左边的空格、制表符、及回车符
 *
 */
int strTrimL(char *s)
{
    int i;

    for(i = 0; i < strlen(s) && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r'); i++);

    if(i > 0)
    {
        memmove(s, s + i, strlen(s) - i + 1);
    }

    return 0;
}

/*
 * 出除字符串右边的空格、制表符、及回车符
 */
int strTrimR(char *s)
{
    int i;

    for(i = strlen(s) - 1; i >= 0 && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r'); i--);

    if(i >= 0)
    {
        s[i + 1] = 0;
    }

    return 0;
}


/*
 * 出除字符串的空格、制表符、及回车符
 */
void strTrim(char *s)
{
    char *pTmp = s;
    while (*s != '\0')
    {
        if (*s != ' ' && *s != '\t' && *s != '\n' && *s != '\r' )
        {
            *pTmp++ = *s;
        }
        ++s;
    }
    *pTmp = '\0';
}


int GetConf(char *file, char *sec, char *key, char *result)
{
    char    *p;
    FILE    *fp;
    char    cfgfile[200];
    char    buf[400];
    memset(cfgfile, 0x00, sizeof(cfgfile));
    memset(buf, 0x00, sizeof(buf));
    result[0] = 0;
    //sprintf(cfgfile, "%s/online/etc/kms.ini", getenv("HOME"));
    sprintf(cfgfile, "%s", file);
    //printf("%s\n", cfgfile);
    //  strcpy(cfgfile, file);

    if((fp = fopen(cfgfile, "r")) == NULL)
    {
        return -1;
    }

    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        strTrimL(buf);
        strTrimR(buf);

        if(strlen(buf) == 0 || buf[0] == '#')
        {
            continue;
        }

        if(buf[0] != '[' || buf[strlen(buf) - 1] != ']')
        {
            continue;
        }

        buf[strlen(buf) - 1] = 0;

        if(strcmp(buf + 1, sec))
        {
            continue;
        }

        while(fgets(buf, sizeof(buf), fp) != NULL)
        {
            strTrimL(buf);
            strTrimR(buf);

            if(strlen(buf) == 0 || buf[0] == '#')
            {
                continue;
            }

            if((p = strchr(buf, '#')) != NULL)
            {
                *p = 0;
            }

            if((p = strchr(buf, '=')) == NULL)
            {
                continue;
            }
            //printf("%c\n", *p);
            //printf("before *p = 0 the buf:%s\n", buf);
            *p = 0;
            //printf("after *p = 0 the buf:%s\n", buf);
            if(strcmp(buf, key))
            {
                continue;
            }

            strcpy(result, p + 1);
            break;
        }

        fclose(fp);
        return 0;
    }

    fclose(fp);
    return -1;
}

int
get_param( char *path, char * val_name, char * value)
{
	FILE *	fp;
	char 	buf1[100], buf2[100];
	char 	ret_str[80];

	//printf("%s\n", getenv("HOME"));
	sprintf(buf1, "%s%s", getenv("HOME"), path);

	//printf("%s\n", buf1);
	if ((fp = fopen(buf1, "r")) == NULL) {
		//ErrorLog( ERROR, " hsm.cf FILE open error" );
		return NGX_ERROR;
	}

	while (!feof(fp)) {
		fgets(buf2, 80, fp);
		if (buf2[0] == '#') continue;
		sscanf(buf2, "%[^=]=%s", buf1, ret_str );
		if (!strcmp(val_name, buf1)) {
			memcpy( value, ret_str, strlen(ret_str));
			fclose( fp );
			return NGX_OK;
		}
	}

	fclose( fp );
	//ErrorLog( ERROR, "get param error" );
	return  NGX_ERROR;
}


//将文件中str那一行等于后面的字符串换成key
int
file_line_change(char* mht_path, char *str1, char* str2)
{
     /*
    *linebuffer：读取文件中的一行存入的缓冲
    *buffer1：一行中第一个字段的存入缓冲
    *buffer2：一行中第二个字段的存入缓冲
    */
    char linebuffer[512] = {0};
    char buffer1[512] = {0};
    char buffer2[512] = {0};

    int line_len = 0;
    int len = 0;
    int res;

    char mpath[1024] = {0};
    sprintf(mpath, "%s", mht_path);
    /*
    * mht_path是文件名，r+代表可以读写文件
    */
    FILE *fp = fopen(mpath, "rw+");
    if (fp == NULL) {
        //ErrorLog(ERROR, "open error");
        return NGX_ERROR;
    }
    while(fgets(linebuffer, 512, fp)) {
        line_len = strlen(linebuffer);
        len += line_len;
        if (linebuffer[0] == '#') continue;
        sscanf(linebuffer, "%[^=]=%s", buffer1,buffer2);
        if(!strcmp(str1, buffer1)) {
            /*
            * 由于已经找到所需要写的位置，所以需要写位置的“头”
            */
            len -= strlen(linebuffer);
            /*
            * 实现文件位置的偏移，为写文件做准备
            */
            res = fseek(fp, len, SEEK_SET);
            if(res < 0) {
                ErrorLog(ERROR, "fseek");
                return NGX_ERROR;
            }
            strcpy(buffer2, str2);
            /*strcat(buffer1, "=");*/
            strcat(buffer1, "=");
            strcat(buffer1, buffer2);
            //printf("%d",strlen(buffer1));
            /*
            * 写文件，存入所需的内容
            */
            fprintf(fp, "%s", buffer1);
            fclose(fp);
            return NGX_OK;
        }
    }
    return NGX_ERROR;
}
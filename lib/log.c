#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "log.h"
#include "ngx_time.h"


int ngx_log_open(int* ret_fd) {
    u_char tmp[256];
    u_char date_time[14+1];
    struct stat  file_stat;
    int  fd;

    //printf("%s\n", path_log);
    fd = open(PATH_LOG, O_CREAT | O_WRONLY | O_APPEND , 0664);
    if (fd == -1) {
        fprintf(stderr, "1fopen log file fail [%s][%s]\n", PATH_LOG, strerror(errno));
        return -1;
    }
    if (fstat(fd, &file_stat) == -1) {
        return -1;
    }
    if (file_stat.st_size >= MAX_LOG) {
        //超出单个日志人间最大大小， 进行分割
        //备份文件需要同步操作,防止重命名到相同文件.
        //int sem_id = ngx_create_sem((key_t)POSP_SEMKEY_LOG, 1);
        //if (sem_id == -1) {
         //   return -1;
       // }
        //ngx_p_sem(sem_id);
        memset(tmp, 0, sizeof(tmp));
        memset(date_time, 0, sizeof(date_time));
        ngx_format_time(&file_stat.st_ctime, date_time, sizeof(date_time));
        sprintf(tmp, "%s_%s",PATH_LOG, date_time);
        if (access(tmp, F_OK) != 0) {
            rename(PATH_LOG, tmp);
        }
        close(fd);
        fd = open(PATH_LOG, O_CREAT | O_WRONLY | O_APPEND, 0664);
        if (fd == -1) {
            fprintf(stderr, "2open log file fail [%s][%s]\n", PATH_LOG, strerror(errno));
        //    ngx_v_sem(sem_id);
            return -1;
        }
        //ngx_v_sem(sem_id);
    }
    *ret_fd = fd;
    return 0;
}


int errlog (char *Info )
{

    int Len;
    int  fp;

    Len = strlen ( Info );
    //fp = fopen ( PATH_LOG, "a+" );
    //ngx_log_open(&fp);
    char path_log[80] = {0};
    get_param("PATH_LOG", path_log);

    if ( ngx_log_open(&fp) < 0 ) {

        fprintf ( stderr, "%s(%d): fopen(%s) error", __FILE__, __LINE__, path_log);
        return ( -1 );
    }

    write(fp, Info, Len);
    //fwrite  ( Info, 1, Len, fp );
    close ( fp );

    return  0 ;
}


void ErrorLog ( char * File, int Line, int level, char * fmt, ... )
{
    va_list args;
    int Len;
    u_char* type;
    char    MDate [ 11 ], MTime [ 13 ], Info [ 4096 ], T_File [ 512 ];

    memset(MDate, 0, sizeof(MDate));
    memset(MTime, 0, sizeof(MTime));
    memset(Info, 0, sizeof(Info));
    memset(T_File, 0, sizeof(T_File));

    switch (level) {
    case LOG_STDERR:
            type = "";
            break;
    case LOG_DEBUG:
            type = "[DEBUG]";
            break;
    case LOG_INFO:
            type = "[INFO]";
            break;
    case LOG_WARN:
            type = "[WARN]";
            break;
    case LOG_ERROR:
            type = "[ERROR]";
            break;
    case LOG_FATAL:
            type = "[FATAL]";
            break;
    }

    va_start ( args , fmt );
    sgetdatef ( MDate, "-" );
    milltime ( MTime, ":" );
    sprintf ( Info, "[%s][%s][%s %d]%s:", MDate, MTime, File, Line, type );
    Len = strlen ( Info );
    vsprintf ( Info + Len, fmt, args );
    strcat ( Info, "\n" );

    if (level == LOG_STDERR) { //直接输出到标准错误输出
        fprintf(stderr, Info);
    }
    else {
        if ( errlog (Info ) < 0 ) {
            Len = strlen ( Info );
            fprintf ( stderr, "%s %s(%d): errlog error", Info, __FILE__, __LINE__ );
        }
    }

    va_end ( args );

    return;

}


int ErrorLog_buf(char * File, int Line, int level, const u_char* dump_buf, size_t size) {
    int i,j,n;
    u_char* type;
    int Len;
    char    MDate [ 11 ], MTime [ 13 ], Info [ 4096 ], T_File [ 512 ];

    memset(MDate, 0, sizeof(MDate));
    memset(MTime, 0, sizeof(MTime));
    memset(Info, 0, sizeof(Info));
    memset(T_File, 0, sizeof(T_File));

    switch (level) {
    case LOG_STDERR:
            type = "";
            break;
    case LOG_DEBUG:
            type = "[DEBUG]";
            break;
    case LOG_INFO:
            type = "[INFO]";
            break;
    case LOG_WARN:
            type = "[WARN]";
            break;
    case LOG_ERROR:
            type = "[ERROR]";
            break;
    case LOG_FATAL:
            type = "[FATAL]";
            break;
    }

    sgetdatef ( MDate, "-" );
    milltime ( MTime, ":" );
    sprintf ( Info, "[%s][%s][%s %d]%s:\n", MDate, MTime, File, Line, type );
    //Len = strlen ( Info );
    for (i = 0; i < size; i++) {
        sprintf(Info + strlen(Info), "%02x ", dump_buf[i]);
        if ((i + 1) % 16 == 0 && (i + 1) != size) {
            Info[strlen(Info)] = '\n';
        }
    }

    Info[strlen(Info)] = '\n';
    if (level == LOG_STDERR) { //直接输出到标准错误输出
        fprintf(stderr, Info);
    }
    else {
        if ( errlog (Info ) < 0 ) {
            Len = strlen ( Info );
            fprintf ( stderr, "%s %s(%d): errlog error", Info, __FILE__, __LINE__ );
        }
    }


    return 0;
}
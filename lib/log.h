#ifndef _LOG_H
#define _LOG_H


#define PATH_LOG "../log/yjx.log"

void ErrorLog ( char * File, int Line, int level, char * fmt, ... );
int ErrorLog_buf(char * File, int Line, int level, const u_char* dump_buf, size_t size);

#define LOG_STDERR		0
#define	LOG_DEBUG		1
#define LOG_INFO		2
#define LOG_WARN		3
#define LOG_ERROR		4
#define LOG_FATAL		5


#define MAX_NUM		20
#define MAX_LOG		1024 * 1024 * MAX_NUM
#define STDERR     __FILE__, __LINE__, LOG_STDERR
#define DEBUG      __FILE__, __LINE__, LOG_DEBUG
#define INFO       __FILE__, __LINE__, LOG_INFO
#define WARN       __FILE__, __LINE__, LOG_WARN
#define ERROR      __FILE__, __LINE__, LOG_ERROR
#define FATAL      __FILE__, __LINE__, LOG_FATAL

#define     sgetdatef( a, b )   datetime ( a, 1, b, b, "" )

#endif
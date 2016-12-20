
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ifndef _NGX_TIME_H_INCLUDE_
#define _NGX_TIME_H_INCLUDE_


#define     DATE_ONLY   1
#define     TIME_ONLY   2

int ngx_format_time(time_t* time, u_char* fmtstr, size_t size);
int ngx_get_datetime(u_char* date_time);
int datetime ( char* str, int dort, char *fmts1, char *fmts2, char *fmts3 );
int milltime ( char *str, char *fmt1 );
void CommonGetCurrentDate(char *sCurrentDate);
void CommonGetCurrentTime(char *sCurrentTime);

#endif
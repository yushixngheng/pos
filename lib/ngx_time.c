/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "ngx_time.h"


int
ngx_format_time(time_t* time, u_char* fmtstr, size_t size) {
    struct tm* t;

    t = localtime(time);
    strftime(fmtstr, size, "%Y%m%d%H%M%S", t);
    return 0;
}


int
ngx_get_datetime(u_char* date_time) {
    time_t t;
    t = time(NULL);
    ngx_format_time(&t, date_time, 15);
    return 0;
}


int
datetime ( char* str, int dort, char *fmts1, char *fmts2, char *fmts3 )
{

    char        fmt [ 40 ];
    long        clock;
    struct  tm  * tm;
    int Year;

    time ( & clock );
    tm = localtime ( & clock  );
    Year = tm->tm_year;
    if (Year < 70)
        Year = Year + 2000;
    else
        Year = Year + 1900;

    switch (dort) {

        case   DATE_ONLY:

            sprintf ( fmt, "%%04d%s%%02d%s%%02d%s",
                    fmts1, fmts2, fmts3 );
            sprintf ( str, fmt,
                    Year,
                    (tm -> tm_mon  ) + 1,
                    (tm -> tm_mday ) );
            break;

        case   TIME_ONLY:

            sprintf ( fmt, "%%02d%s%%02d%s%%02d%s",
                    fmts1, fmts2, fmts3 );
            sprintf ( str, fmt,
                    (tm -> tm_hour ) ,
                    (tm -> tm_min  ) ,
                    (tm -> tm_sec  ) );
            break;


        default :

            return -1;
    }

    return 0;
}


int
milltime ( char *str, char *fmt1 )
{

    struct  timeb   tp;
    struct  tm  * tm;
    char    fmt [ 20 ];

    ftime ( &tp );
    tm = localtime (  & ( tp.time )  );

    sprintf ( fmt, "%%02d%s%%02d%s%%02d%s%%03d", fmt1, fmt1, fmt1 );
    sprintf ( str, fmt, ( tm -> tm_hour ) , ( tm -> tm_min  ) ,
        ( tm -> tm_sec  ),  ( tp.millitm ) );

    return 0;

}


void
CommonGetCurrentDate(char *sCurrentDate)
{
    time_t current;
    struct tm *tmCurrentTime;
    time(&current);
    tmCurrentTime = localtime(&current);
    sprintf(sCurrentDate, "%4d%02d%02d", tmCurrentTime->tm_year + 1900,
            tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday);
}

/*****************************************************************************/
/* FUNC:   void CommonGetCurrentTime (char *sCurrentTime);                   */
/* INPUT:  <none>                                                            */
/* OUTPUT: sCurrentTime   -- the string of current time                      */
/* RETURN: <none>                                                            */
/* DESC:   Get the system time with the format (YYYYMMDDhhmmss).             */
/*****************************************************************************/
void
CommonGetCurrentTime(char *sCurrentTime)
{
    time_t current;
    struct tm *tmCurrentTime;
    tzset();
    time(&current);
    tmCurrentTime = localtime(&current);
    sprintf(sCurrentTime, "%4d%02d%02d%02d%02d%02d",
            tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
            tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
            tmCurrentTime->tm_min, tmCurrentTime->tm_sec);
}


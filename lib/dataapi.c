#include "ngx_core.h"

/*----------------------------------------------------------------------------
  **  函数名称: isLeapYear
  **  功能描述: 判断是否为闰年(仅供其他函数使用)
  **  输入参数: int year    --->  年
  **  输出参数:
  **  返回结果：int
                 0  --->  不是闰年
                 1  --->  是闰年
  ---------------------------------------------------------------------------*/
int
isLeapYear(int year)
{
    if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    {
        return 1;
    }

    return 0;
}



/*----------------------------------------------------------------------------
  **  函数名称: daysFrom
  **  功能描述: 计算某日到该年第一天的间隔天数(仅在函数DaysInterval中使用)
  **  输入参数: int year    --->  年
                int month   --->  月
                int day     --->  日
  **  输出参数:
  **  返回结果：int --->  间隔天数
  ---------------------------------------------------------------------------*/
int daysFrom(int year, int month, int day)
{
    int monthDays[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int i = 0;
    int sum = 0;

    for(i = 1; i < month; i++)
    {
        sum += monthDays[ i ];
    }

    if(month > 2)
        if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        {
            sum += 1;
        }

    sum += day;
    return sum;
}


/*****************************************************************************
  **  函数名称: DaysInterval
  **  功能描述: 计算两个日期时间间隔
  **  输入参数: char * v1   --->  起始日期
                char * v2   --->  结束日期
  **  输出参数:
  **  返回结果：int --->  起始日期到结束日期的天数，算头不算尾
  **  备注：返回正值，结束日期在起始日期后面，返回负值，结束日期在起始日期前面
  ****************************************************************************/
int
DaysInterval(char* v1, char* v2)
{
    char tmpYear[5];
    char tmpMonth[3];
    char tmpDay[3];
    int dayLong = 0;
    int year1,
        month1,
        day1;
    int year2,
        month2,
        day2;
    int from1;
    int from2;
    int totalDay1;
    int totalDay2;
    int i = 0;
    int tmpDays = 365;
    strncpy(tmpYear, v1, 4);
    strncpy(tmpMonth, v1 + 4, 2);
    strncpy(tmpDay, v1 + 6, 2);
    tmpYear[ 4 ] = '\0';
    tmpMonth[ 2 ] = '\0';
    tmpDay[ 2 ] = '\0';
    year1 = atoi(tmpYear);
    month1 = atoi(tmpMonth);
    day1 = atoi(tmpDay);
    strncpy(tmpYear, v2, 4);
    strncpy(tmpMonth, v2 + 4, 2);
    strncpy(tmpDay, v2 + 6, 2);
    tmpYear[ 4 ] = '\0';
    tmpMonth[ 2 ] = '\0';
    tmpDay[ 2 ] = '\0';
    year2 = atoi(tmpYear);
    month2 = atoi(tmpMonth);
    day2 = atoi(tmpDay);

    if(year1 == year2)
    {
        from1 = daysFrom(year1, month1, day1);
        from2 = daysFrom(year2, month2, day2);
        dayLong = from2 - from1;
    }
    else if(year1 < year2)
    {
        from1 = daysFrom(year1, month1, day1);
        totalDay1 = 365 - from1;

        if(((year1 % 4 == 0) && (year1 % 100 != 0)) || (year1 % 400 == 0))
        {
            totalDay1 += 1;
        }

        dayLong += totalDay1;
        i = year1;

        while(++i < year2)
        {
            tmpDays = 365;

            if(((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
            {
                tmpDays += 1;
            }

            dayLong += tmpDays;
        }

        from2 = daysFrom(year2, month2, day2);
        dayLong += from2;
    }
    else
    {
        from2 = daysFrom(year2, month2, day2);
        totalDay2 = 365 - from2;

        if(((year2 % 4 == 0) && (year2 % 100 != 0)) || (year2 % 400 == 0))
        {
            totalDay2 += 1;
        }

        dayLong += totalDay2;
        i = year2;

        while(++i < year1)
        {
            tmpDays = 365;

            if(((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
            {
                tmpDays += 1;
            }

            dayLong += tmpDays;
        }

        from1 = daysFrom(year1, month1, day1);
        dayLong += from1;
        dayLong *= -1;
    }

    return dayLong;
}


/*****************************************************************************
  **  函数名称: CalNewDate
  **  功能描述: 给定一个日期和间隔天数，计算新日期
  **  输入参数: char *srcDate   --->  基准日期
                int days        --->  间隔天数(正负皆可)
                char *desDate   --->  新日期
  **  输出参数:
  **  返回结果：int
                 0  --->  成功
  ****************************************************************************/
int
CalNewDate(char* srcDate, int days, char* desDate)
{
    if(days == 0)
    {
        strcpy(desDate, srcDate);
    }
    else
    {
        char tmpYear[5];
        char tmpMonth[3];
        char tmpDay[3];
        int year = 0;
        int month = 0;
        int day = 0;
        int monthDays[ 2 ][ 13 ] = { 365, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
                                     366, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
                                   };
        strncpy(tmpYear, srcDate, 4);
        strncpy(tmpMonth, srcDate + 4, 2);
        strncpy(tmpDay, srcDate + 4 + 2, 2);
        tmpYear[ 4 ] = '\0';
        tmpMonth[ 2 ] = '\0';
        tmpDay[ 2 ] = '\0';
        year = atoi(tmpYear);
        month = atoi(tmpMonth);
        day = atoi(tmpDay);
        day += days;

        if(days > 0)
        {
            while(day > monthDays[ isLeapYear(year) ][ month ])
            {
                day -= monthDays[ isLeapYear(year) ][ month ];
                month += 1;

                if(month > 12)
                {
                    year += 1;
                    month = 1;
                }
            }
        }
        else
        {
            while(day <= 0)
            {
                month -= 1;

                if(month < 1)
                {
                    year -= 1;
                    month = 12;
                }

                day += monthDays[ isLeapYear(year) ][ month ];
            }
        }

        sprintf(tmpYear, "%d", year);
        month < 10 ? sprintf(tmpMonth, "0%d", month) : sprintf(tmpMonth, "%d", month);
        day < 10 ? sprintf(tmpDay, "0%d", day) : sprintf(tmpDay, "%d", day);
        strcpy(desDate, tmpYear);
        strcat(desDate, tmpMonth);
        strcat(desDate, tmpDay);
        desDate[ 8 ] = '\0';
    }

    return 0;
}


char*
GetDate()
{
    static char time_str[15];
    time_t current;
    struct tm *tmCurrentTime;
    time(&current);
    tmCurrentTime = localtime(&current);
    sprintf(time_str, "%4d%02d%02d%02d%02d%02d", tmCurrentTime->tm_year + 1900,
            tmCurrentTime->tm_mon + 1, tmCurrentTime->tm_mday,
            tmCurrentTime->tm_hour, tmCurrentTime->tm_min,
            tmCurrentTime->tm_sec);
    return (time_str) ;
}

/************************************************************************************/
/*   函数名：                      取日期偏移量                                     */
/************************************************************************************/
/*   声明  ： int GetDate( )                                                        */
/*   功能  ： InPOutPDate  输入输出日期                                             */
/*   参数  ： iPos  偏移量                                                          */
/*         ： 无-- 输出                                                             */
/*  返回值 ： 无                                                                    */
/*            无                                                                    */
/************************************************************************************/
void
GetDate_offset(char* InPOutPDate, int iPos)
{
    time_t timeInOut;
    struct tm *tmInOut;
    char DateTmp[8 + 1];
    char yearS[4 + 1];
    char mouS[2 + 1];
    char dateS[2 + 1];
    int year = 0;
    int mou = 0;
    int date = 0;
    memset(yearS , 0 , sizeof(yearS));
    memset(mouS , 0 , sizeof(mouS));
    memset(dateS , 0 , sizeof(dateS));
    memcpy(yearS, InPOutPDate , 4);
    memcpy(mouS, InPOutPDate + 4 , 2);
    memcpy(dateS, InPOutPDate + 6 , 2);
    year = atoi(yearS);
    mou  = atoi(mouS);
    date = atoi(dateS);
    time(&timeInOut);
    tmInOut = localtime(&timeInOut);
    tmInOut->tm_year = year - 1900;
    tmInOut->tm_mon = mou - 1;
    tmInOut->tm_mday = date;
    tmInOut->tm_hour = 0;
    tmInOut->tm_min = 0;
    tmInOut->tm_sec = 0;
    timeInOut = mktime(tmInOut);
    memset(DateTmp , 0 , sizeof(DateTmp));
    timeInOut += iPos * 24l * 3600l;
    tmInOut = localtime(&timeInOut);
    sprintf(DateTmp,
            "%04d%02d%02d",
            tmInOut->tm_year + 1900 , tmInOut->tm_mon + 1, tmInOut->tm_mday);
    memcpy(InPOutPDate , DateTmp , 8);
}


/*****************************************************
*函数名称: CountDate
*函数功能: 计算一个日期加上或者减去一个天数后所得的日期
*输入参数: lStartDate -- 起始日期(YYYYMMDD)
           iOffset    -- 偏移量(负数表示减去)
*输出参数: sResultDate -- 计算所得日期
*返回值:   0  -- 成功
           -1 -- 失败
*****************************************************/
long
CountDate(char* sStartDate, int iOffset, char* sResultDate)
{
    int  step;
    long lStartDate;
    int  iYear, iMonth, iDays;
    long i;
    lStartDate  =  atol(sStartDate);
    iYear  = lStartDate / 10000;
    iMonth = lStartDate % 10000 / 100;
    iDays  = lStartDate % 100;
    step = (iOffset >= 0 ? 1 : -1);

    for(i = 1; i <= abs(iOffset); i ++)
    {
        iDays += step;

        if(iDays <= 0 || iDays > RMONTHDAYS(iMonth, iYear))
        {
            iMonth += step;

            if(iMonth <= 0)
            {
                iYear += step;
                iMonth = 12;
            }
            else if(iMonth > 12)
            {
                iYear += step;
                iMonth = 1;
            }

            if(iDays <= 0)
            {
                iDays = RMONTHDAYS(iMonth, iYear);
            }
            else
            {
                iDays = 1;
            }
        }
    }

    lStartDate = iYear * 10000 + iMonth * 100 + iDays;
    sprintf(sResultDate, "%8ld", lStartDate);
    return 0;
}


/* 求两个日期之间的相差的天数 */
int
getDayBetween(char* s_date, char* b_date)
{
	long clock, s_clock, b_clock ;

	s_clock = date2long( s_date ) ;
	b_clock = date2long( b_date ) ;

	clock = b_clock - s_clock ;

	return ( clock / 86400l ) ;
}


long
date2long(char * sdate)
{
long	clock;
struct  tm 	*tm , tm0 ;
int	Year , Month , Day ;
char	buf[ 10 ] ;

	if( strlen( sdate ) != 8 )
		return -1 ;

	memcpy( buf , sdate , 4 ) ;
	buf[ 4 ] = '\0' ;
	Year = atoi( buf ) ;

	memcpy( buf , sdate + 4 , 2 ) ;
	buf[ 2 ] = '\0' ;
	Month = atoi( buf ) ;

	memcpy( buf , sdate + 6 , 2 ) ;
	buf[ 2 ] = '\0' ;
	Day = atoi( buf ) ;

	tm = &tm0 ;

	/*
        if(Year >= 2000 )
                Year = Year - 2000;
        else
                Year = Year - 1900;
	*/

	Year = Year - 1900;

        tm->tm_year = Year ;
        tm->tm_mon = Month - 1 ;
        tm->tm_mday = Day ;
        tm->tm_hour = 8 ;
        tm->tm_min = 30 ;
        tm->tm_sec = 30 ;

	clock = mktime( tm ) ;

	return clock ;
}


int
getodate(char* tdate, int day, char* odate)
{
long	clock;
struct  tm 	*tm , tm0 ;
int	Year , Month , Day ;
char	buf[ 10 ] ;

	if( strlen( tdate ) != 8 )
		return -1 ;

	memcpy( buf , tdate , 4 ) ;
	buf[ 4 ] = '\0' ;
	Year = atoi( buf ) ;

	memcpy( buf , tdate + 4 , 2 ) ;
	buf[ 2 ] = '\0' ;
	Month = atoi( buf ) ;

	memcpy( buf , tdate + 6 , 2 ) ;
	buf[ 2 ] = '\0' ;
	Day = atoi( buf ) ;

	tm = &tm0 ;

	/*
        if(Year >= 2000 )
                Year = Year - 2000;
        else
                Year = Year - 1900;
	*/

	Year = Year - 1900;

        tm->tm_year = Year ;
        tm->tm_mon = Month - 1 ;
        tm->tm_mday = Day ;
        tm->tm_hour = 8 ;
        tm->tm_min = 30 ;
        tm->tm_sec = 30 ;

	clock = mktime( tm ) ;

	clock += ( 86400l * day ) ;

	tm = localtime ( & clock  );
        Year = tm->tm_year;

	/*
        if(Year < 70 )
                Year = Year + 2000;
        else
                Year = Year + 1900;
	*/

	Year = Year + 1900;

        sprintf( odate, "%04d%02d%02d" , Year , tm->tm_mon + 1 , tm->tm_mday );
	return 0 ;
}


void
getbdate(char* bdate, int day)
{
long	clock;
struct  tm* tm;
int	Year;

 	time ( & clock );
	clock -= ( 86400 * day ) ;
	tm = localtime ( & clock  );

        Year = tm->tm_year;

	/*
        if(Year < 70 )
                Year = Year + 2000;
        else
                Year = Year + 1900;
	*/

	Year = Year + 1900;

        sprintf( bdate, "%04d%02d%02d" , Year , tm->tm_mon + 1 , tm->tm_mday );
	return ;
}


void
sysdate(char* sdate)
{

    long        l;
    struct  tm  * tm;
    int     year , month ;

    time ( & l );
    tm = localtime ( & l  );

    year = tm->tm_year;

    /*
    if(year < 70 )
        year = year + 2000;
    else
        year = year + 1900;
    */
    year = year + 1900;

    month = tm->tm_mon + 1;

    sprintf( sdate , "%04d%02d%02d" , year , month , tm->tm_mday ) ;

}


long
calc_delay_time(char* start_date , char* start_time)
{
    struct  tm      * tm ;
    long    l , delay_time ;
    char    buf[ 10 ] ;
    int hour , min , sec ;

    buf[ 2 ] = '\0' ;
    memcpy( buf , start_time  , 2 ) ;
    hour = atoi( buf ) ;
    memcpy( buf , start_time  + 2 , 2 ) ;
    min = atoi( buf ) ;
    memcpy( buf , start_time  + 4 , 2 ) ;
    sec = atoi( buf ) ;

    time ( & l );
    tm = localtime ( & l  );

    sysdate( buf ) ;
    if( memcmp( buf , start_date , 8 ) == 0 )
    {
        delay_time =    3600 * ( tm-> tm_hour - hour )  +
                60 * ( tm-> tm_min - min )  +
                ( tm-> tm_sec - sec ) ;
    }
    else
    {
        delay_time =    3600 * ( tm-> tm_hour )  +
                60 * ( tm-> tm_min )  + tm-> tm_sec ;
        delay_time +=   ( 3600 * ( 23 - hour )  +
                  60 * ( 59 - min )  + 
                  ( 60 - sec )
                ) ; 

    }
    return ( delay_time ) ;

}


int
nowdate_add_day(int off, char* date)
{        //增加对应的天数
        char * stime;
        time_t now_time;
        time_t new_time;
        struct tm *t;

        now_time=time(NULL);
        new_time=now_time+ off*DAYSEC;
        t = localtime(&new_time);
        strftime(date, 9, "%Y%m%d", t);
        return 0;
}

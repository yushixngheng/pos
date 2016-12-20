#ifndef _DATA_API_H_
#define _DATA_API_H_


#define ___IsLeapYear(N) (((N % 4 == 0) && (N % 100 != 0)) || (N % 400 == 0)) /*是否为闰年*/

#define YEARDAYS(a) ___IsLeapYear(a)?366:365      /*一年的天数*/
#define MON2DAYS(a) ___IsLeapYear(a)?29:28        /*二月的天数*/
#define DAYSEC    (3600*24)

/*一个月的天数*/
#define RMONTHDAYS(a, b) ((a) == 2?(MON2DAYS(b)):((a) < 8 && (a) % 2 == 0 || (a) >= 8 && (a) % 2 != 0)?30:31)

int isLeapYear(int year);
int daysFrom(int year, int month, int day);
int DaysInterval(char* v1, char* v2);
int CalNewDate(char *srcDate, int days, char* desDate);
char *GetDate();
void GetDate_offset(char* InPOutPDate, int iPos);
long CountDate(char *sStartDate, int iOffset, char* sResultDate);
long date2long(char* sdate);
int getDayBetween(char* s_date, char* b_date);/* 求两个日期之间的相差的天数 */
int getodate( char* tdate, int day, char* odate);
void getbdate(char* bdate, int day);
void sysdate(char* sdate);
long calc_delay_time(char* start_date , char* start_time);
int nowdate_add_day(int off, char* date);

#endif
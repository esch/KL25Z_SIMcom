#include "mbed.h"


#include <stdio.h>
#include <sstream>
#include <string>

#define IS_LEAP(year) (year%4 == 0)
#define SEC_IN_NON_LEAP (86400*365)
#define SEC_IN_LEAP (86400*366)
#define SEC_IN_YEAR(year) (IS_LEAP(year) ? SEC_IN_LEAP : SEC_IN_NON_LEAP)

char buf[30];

unsigned char calendar [] = {31, 28, 31, 30,31, 30, 31, 31,30, 31, 30, 31};
unsigned char calendar_leap [] = {31, 29, 31, 30,31, 30, 31, 31,30, 31, 30, 31};

unsigned char *get_calendar(int year) {
        return IS_LEAP(year) ? calendar_leap : calendar;
}

int  get_year(unsigned long *t) {
        int year=1970;
        while(*t>SEC_IN_YEAR(year)) {
                  *t-=SEC_IN_YEAR(year);
                  year++;
        }   
        return year;
}

int get_month(unsigned long *t, int year) {
        unsigned char *cal = get_calendar(year);
        int i=0;
        while(*t > cal[i]*86400) {
                *t-=cal[i]*86400;
                i++;
        }   
        return i+1;
}

char *epoch_to_str(unsigned long epoch) {
   int year=get_year(&epoch);
   unsigned char month=get_month(&epoch,year);
   unsigned char day=1+(epoch/86400);
   epoch=epoch%86400;
   unsigned char hour=epoch/3600;
   epoch%=3600;
   unsigned char min=epoch/60;
   unsigned char sec=epoch%60;
   

   sprintf(buf,"%02d.%02d.%02d %02d:%02d:%02d",day,month,year,hour,min,sec);
   return buf;
}

char *time_to_str(unsigned long epoch) {
   epoch=epoch%86400;
   unsigned char hour=epoch/3600;
   epoch%=3600;
   unsigned char min=epoch/60;
   unsigned char sec=epoch%60;
   
   sprintf(buf,"%02d:%02d:%02d",hour,min,sec);
   return buf;
}


int time_in_day(unsigned long epoch) {
   epoch=epoch%86400;
   unsigned char hour=epoch/3600;
   epoch%=3600;
   unsigned char min=epoch/60;

   return hour*60+min;
}


time_t time_when_compiled(char *d,char *t)
{
    char  datestr[16];
    char  timestr[16];

    char str_month[8];
    int day;
    int year;

    int hour, min, sec;

    sprintf(datestr,"%s",d);
    sprintf(timestr,"%s",t);

    sscanf(datestr,"%s %d %d",&str_month,&day,&year);
    sscanf(timestr,"%d:%d:%d",&hour,&min,&sec);

    printf("date %s  %d %d \r\n",str_month,day,year );
    printf("time %d %d %d \r\n",hour,min,sec );

    int month;
    if     ( !strcmp(str_month,"Jan") ) month = 1;
    else if( !strcmp(str_month,"Feb") ) month = 2;
    else if( !strcmp(str_month,"Mar") ) month = 3;
    else if( !strcmp(str_month,"Apr") ) month = 4;
    else if( !strcmp(str_month,"May") ) month = 5;
    else if( !strcmp(str_month,"Jun") ) month = 6;
    else if( !strcmp(str_month,"Jul") ) month = 7;
    else if( !strcmp(str_month,"Aug") ) month = 8;
    else if( !strcmp(str_month,"Sep") ) month = 9;
    else if( !strcmp(str_month,"Oct") ) month = 10;
    else if( !strcmp(str_month,"Nov") ) month = 11;
    else if( !strcmp(str_month,"Dec") ) month = 12;
    else 
        return 0;

    tm _t = {0};

    _t.tm_mon = month - 1;
    _t.tm_mday = day;
    _t.tm_year = year - 1900;
    _t.tm_hour = hour;
    _t.tm_min = min;
    _t.tm_sec = sec;

  // printf("time-- %d \r\n",mktime(&t) );


    return mktime(&_t);
}

/*
 Copyright (C) 2017 E. Schaefer <esch.dev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "mbed.h" 
#include "sys.h"
#include "time_utils.h"

time_t secc;

Sys::Sys() {

}

char *Sys::getTime() 
{
     return epoch_to_str(secc);
}

void Sys::setTime(char *str) 
{

int hour,min,sec;
int year,day,month;
int len;

  printf("in:%s\r\n",str );
  sscanf(str, "%d.%d.%d %d:%d:%d\%n",&day,&month,&year, &hour, &min,&sec,&len);

  printf("%d.%d.%d %d:%d:%d\r\n",day,month,year,hour,min,sec);

    tm t = {0};

    t.tm_mon = month-1;
    t.tm_mday = day;
    t.tm_year = year - 1900;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;

    secc = mktime(&t);

}

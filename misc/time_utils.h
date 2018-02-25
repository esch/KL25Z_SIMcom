#ifndef __TIME_UTILS_H
#define __TIME_UTILS_H

char *epoch_to_str(unsigned long epoch);
char *time_to_str(unsigned long epoch);
int  get_month(unsigned long *t, int year);
int  get_year(unsigned long *t) ;
int  time_in_day(unsigned long );

time_t time_when_compiled(char *d,char *t);

#endif
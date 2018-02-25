#ifndef __SYS_H__
#define __SYS_H__
#include "mbed.h"
#include <string>

class Sys 
{
public:
    Sys();

    char *getTime();
    void setTime(char *);

};


#endif
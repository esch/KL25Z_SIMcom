#ifndef __SIMGPS_H__
#define __SIMGPS_H__


int scanGPGSA(char *);
int scanGPRMC(char *);
int scanGPGGA(char *);
int outputScan(char *);
int scanGPGSV(char *);


m_item gpsList[] = { 
        {(char *)"GON", (char *)"AT+CGPSPWR=1\r\n", (char *)"GPS power on",10000,0,false,false},
        {(char *)"GOF", (char *)"AT+CGPSPWR=0\r\n", (char *)"GPS power off",10000,0,false,false},

        {(char *)"GON", (char *)"AT+CGPSRST=1\r\n", (char *)"reset (hot start)",10000,0,false,false},
        {(char *)"GOF", (char *)"AT+CGPSRST=0\r\n", (char *)"reset (cold start)",10000,0,false,false},


        {(char *)"GIN",(char *)"AT+CGPSINF=0\r",(char *)"read GPS",10,0,false,true,outputScan},
        {(char *)"GET0",(char *)"AT+CGPSOUT=0\r" , (char *)"disable GPS data",100,2 ,false,false},
        {(char *)"GET1",(char *)"AT+CGPSOUT=1\r" , (char *)"get GPS ",100,5 ,false,false},
        {(char *)"GET2",(char *)"AT+CGPSOUT=2\r" , (char *)"get GPGGA",100,5 ,false,true,scanGPGGA},
        {(char *)"GET3",(char *)"AT+CGPSOUT=4\r" , (char *)"get GPS",100,5 ,false,false},
        {(char *)"GET4",(char *)"AT+CGPSOUT=8\r" , (char *)"get GPGSA",100,5 ,false,true,scanGPGSA},
        {(char *)"GET5",(char *)"AT+CGPSOUT=16\r" , (char *)"get GPGSV",100,5 ,false,true,scanGPGSV},
        {(char *)"GET6",(char *)"AT+CGPSOUT=32\r" , (char *)"get GPRMC",100,5 ,false,true,scanGPRMC},
        {(char *)"GET7",(char *)"AT+CGPSOUT=64\r" , (char *)"get GPS",100,5 ,false,false},
        {(char *)"GST",(char *)"AT+CGPSSTATUS?\r" , (char *)"GPS status",10,0 ,false,false}
  };


int gpsItems = (sizeof(gpsList)/sizeof(gpsList[0]));






#endif

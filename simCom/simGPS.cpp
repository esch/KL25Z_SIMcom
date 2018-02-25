/*
 Copyright (C) 2017 E. Schaefer <esch.dev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "mbed.h" 
#include "simCom.h" 






int outputScan(char *ms)
{
int len = 0;

char sp[2][16];
float dp[9];

printf("%s\n",ms );

sscanf(ms, "%s %s %f,%f,%f,%f,%f,%f,%f,%f,%f%n", &sp[0], &sp[1],&dp[0],&dp[1],&dp[2],&dp[3],&dp[4],&dp[5],&dp[6],&dp[7],&dp[8],&len);
  printf(">>:%s %s mode:%f long:%f lat:%f alt:%f UTC:%f TTFF:%f num:%f speed:%f course:%f %d\n",sp[0],sp[1],dp[0],dp[1],dp[2],dp[3],dp[4],dp[5],dp[6],dp[7],dp[8] ,len);

return(0);
}



// $GPGGA,141805.000,4612.7534,N,00556.4933,E,1,10,0.80,583.8,M,48.2,M,,*6B
// $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

// Where:
//      GGA          Global Positioning System Fix Data
//      123519       Fix taken at 12:35:19 UTC
//      4807.038,N   Latitude 48 deg 07.038' N
//      01131.000,E  Longitude 11 deg 31.000' E
//      1            Fix quality: 0 = invalid
//                                1 = GPS fix (SPS)
//                                2 = DGPS fix
//                                3 = PPS fix
//                                4 = Real Time Kinematic
//                                5 = Float RTK
//                                6 = estimated (dead reckoning) (2.3 feature)
//                                7 = Manual input mode
//                                8 = Simulation mode
//      08           Number of satellites being tracked
//      0.9          Horizontal dilution of position
//      545.4,M      Altitude, Meters, above mean sea level
//      46.9,M       Height of geoid (mean sea level) above WGS84
//                       ellipsoid
//      (empty field) time in seconds since last DGPS update
//      (empty field) DGPS station ID number
//      *47          the checksum data, always begins with *



int scanGPGGA(char *ms) {
int len = 0;

char sp[2][16];
float dp[9];

printf("%s\n",ms );


// sscanf(ms, "%s %s %f,%f,%f,%f,%f,%f,%f,%f,%f%n", &sp[0], &sp[1],&dp[0],&dp[1],&dp[2],&dp[3],&dp[4],&dp[5],&dp[6],&dp[7],&dp[8],&len);
  // printf(">>:%s %s mode:%f long:%f lat:%f alt:%f UTC:%f TTFF:%f num:%f speed:%f course:%f %d\n",sp[0],sp[1],dp[0],dp[1],dp[2],dp[3],dp[4],dp[5],dp[6],dp[7],dp[8] ,len);

  return 0;
}



// $GPRMC,140424.000,A,4612.7545,N,00556.4932,E,0.01,92.34,060417,,,A*58

// Where:
//      RMC          Recommended Minimum sentence C
//      123519       Fix taken at 12:35:19 UTC
//      A            Status A=active or V=Void.
//      4807.038,N   Latitude 48 deg 07.038' N
//      01131.000,E  Longitude 11 deg 31.000' E
//      022.4        Speed over the ground in knots
//      084.4        Track angle in degrees True
//      230394       Date - 23rd of March 1994
//      003.1,W      Magnetic Variation
//      *6A          The checksum data, always begins with *

int scanGPRMC(char *ms) {
int len = 0;

char sp[2][16];
float dp[9];

printf("%s\n",ms );


// sscanf(ms, "%s %s %f,%f,%f,%f,%f,%f,%f,%f,%f%n", &sp[0], &sp[1],&dp[0],&dp[1],&dp[2],&dp[3],&dp[4],&dp[5],&dp[6],&dp[7],&dp[8],&len);
  // printf(">>:%s %s mode:%f long:%f lat:%f alt:%f UTC:%f TTFF:%f num:%f speed:%f course:%f %d\n",sp[0],sp[1],dp[0],dp[1],dp[2],dp[3],dp[4],dp[5],dp[6],dp[7],dp[8] ,len);

  return 0;
}


// $GPGSA,A,3,12,19,24,14,06,25,32,02,15,29,,,1.48,0.80,1.25*04

// GSA      Satellite status
// A        Auto selection of 2D or 3D fix (M = manual) 
// 3        3D fix - values include: 1 = no fix
//                                   2 = 2D fix
//                                   3 = 3D fix
// 04,05... PRNs of satellites used for fix (space for 12) 
// 2.5      PDOP (dilution of precision) 
// 1.3      Horizontal dilution of precision (HDOP) 
// 2.1      Vertical dilution of precision (VDOP)
// *39      the checksum data, always begins with *

// char  _gsa[] = "$GPGSA,A,3,15,13,24,17,19,28,18,12,20,,,,1.98,1.01,1.70*08";

int scanGPGSA(char *ms) {
int len = 0;

char sp[2][16];
float dp[9];

printf("%s\n",ms );


// sscanf(ms, "%s %s %f,%f,%f,%f,%f,%f,%f,%f,%f%n", &sp[0], &sp[1],&dp[0],&dp[1],&dp[2],&dp[3],&dp[4],&dp[5],&dp[6],&dp[7],&dp[8],&len);
  // printf(">>:%s %s mode:%f long:%f lat:%f alt:%f UTC:%f TTFF:%f num:%f speed:%f course:%f %d\n",sp[0],sp[1],dp[0],dp[1],dp[2],dp[3],dp[4],dp[5],dp[6],dp[7],dp[8] ,len);

  return 0;
}

//   $GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75

// Where:
//       GSV          Satellites in view
//       2            Number of sentences for full data
//       1            sentence 1 of 2
//       08           Number of satellites in view

//       01           Satellite PRN number
//       40           Elevation, degrees
//       083          Azimuth, degrees
//       46           SNR - higher is better
//            for up to 4 satellites per sentence
//       *75          the checksum data, always begins with *

int scanGPGSV(char *ms) {
int len = 0;

char sp[2][16];
float dp[9];

printf("%s\n",ms );


// sscanf(ms, "%s %s %f,%f,%f,%f,%f,%f,%f,%f,%f%n", &sp[0], &sp[1],&dp[0],&dp[1],&dp[2],&dp[3],&dp[4],&dp[5],&dp[6],&dp[7],&dp[8],&len);
  // printf(">>:%s %s mode:%f long:%f lat:%f alt:%f UTC:%f TTFF:%f num:%f speed:%f course:%f %d\n",sp[0],sp[1],dp[0],dp[1],dp[2],dp[3],dp[4],dp[5],dp[6],dp[7],dp[8] ,len);

  return 0;
}




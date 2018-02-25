/*
 Copyright (C) 2017 E. Schaefer <esch.dev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "mbed.h" 
#include "simCom.h"
#include "simGPS.h"
#include "time_utils.h"
#include "tinysh.h"

Serial *gdbg;

extern PwmOut rled;
extern PwmOut bled;
DigitalOut led(PTC7);
DigitalOut ldb(PTC0);


extern time_t secc;
extern SIMcom *sim;


// http://www.developershome.com/sms/readSmsByAtCommands.asp
// +CMTI: "SM",9

// returns index into message string 



      // cmd id,  AT cmd,  decription, wait for first return msg [ms], wait for second return msg [sec] , input needed, pointer function present ,pointer function to be executed  }
        // AT+XXXX=? test command;  AT+XXXX? read command
m_item mList[] = { 
        {(char *)"AT", (char *)"AT\r\n", (char *)"simple test",10000,0,false,false},
        {(char *)"CME",(char *)"AT+CMEE=2;&W\r",(char *)"set debug outputs",10,0,false,false},
        {(char *)"CLK",(char *)"AT+CCLK?\r" , (char *)"get clock time",10,0 ,false,false},
        {(char *)"PIN",(char *)"AT+CPIN=8236\r", (char *)"set PIN 8236",500,10,false,false},
        // {(char *)"PIN",(char *)"AT+CPIN=7331\r", (char *)"set PIN 7331",500,10,false,false},
        {(char *)"OPS",(char *)"AT+COPS=?\r\n", (char *)"get operators",20000,0,false,false},
        {(char *)"GCF",(char *)"AT&V\r", (char *)"get current configuration",10,0,false,false},
        {(char *)"GSN",(char *)"AT+GSN\r", (char *)"get IMEI",10,0,false,false},
        {(char *)"GSP",(char *)"AT+CSPN\r", (char *)"get service provider from sim",10,0,false,false},
        {(char *)"SDT",(char *)"AT+CSDT=?\r", (char *)"detect sim",10,0,false,false},
        {(char *)"CSQ",(char *)"AT+CSQ\r", (char *)"signal quality",100,0,false,false},
        {(char *)"SCA",(char *)"AT+CSCA?\r", (char *)"service centre address",100,0,false,false},
        {(char *)"PBE",(char *)"AT+CPBS?\r", (char *)"phone book entry",100,3,false,false},
        {(char *)"REG",(char *)"AT+CREG?\r", (char *)"registration",100,3,false,false},

// AT+CMSS Send SMS message from storage

// AT+CNMI New SMS message indications
// AT+CPMS Preferred SMS message storage
// AT+CRES Restore SMS settings
// AT+CSAS Save SMS settings
// AT+CSCA SMS service center address
// AT+CSCB Select cell broadcast SMS messages
// AT+CSDH Show SMS text mode parameters
// AT+CSMP Set SMS text mode parameters
// AT+CSMS Select message service    
        // AT+CSMS=?
        {(char *)"SMS",(char *)"AT+CSMS=?\r", (char *)"select message service",10,0,false,false},
        {(char *)"MGF",(char *)"AT+CMGF=1\r", (char *)"set SMS mode",10,0,false,false},
        {(char *)"LSS",(char *)"AT+CSMS?\r", (char *)"list of supported services",100,0,false,false},

        {(char *)"MSG",(char *)"AT+CMGS=?\r", (char *)"send message",10,0,false,false},
        {(char *)"MSW",(char *)"AT+CMGW=?\r", (char *)"store message",10,0,false,false},
        {(char *)"MSS",(char *)"AT+CMSS=%%MID%%\r", (char *)"send message from storage",100,0,true,false},

        {(char *)"GMS",(char *)"AT+CPMS?\r", (char *)"get prefered message storage",20,0,false,false},
        {(char *)"TMS",(char *)"AT+CPMS=?\r", (char *)"supported message storage",20,0,false,false},
        {(char *)"NMS",(char *)"AT+CNMI=?\r", (char *)"new message indications",20,0,false,false},
        // {(char *)"LMS",(char *)"AT+CMGL?\r", (char *)"list messages",20,0,false},
        {(char *)"LMS",(char *)"AT+CMGL=?\r", (char *)"list messages",20,0,false,false},
        {(char *)"LMI",(char *)"AT+CNMI=?\r", (char *)"list messages index (GPIO?)",50,0,false,false},
        {(char *)"DMS",(char *)"AT+CMGD=%%MID%%\r", (char *)"del message",50,0,true,false},
        {(char *)"RMS",(char *)"AT+CMGR=%%MID%%\r", (char *)"read messages",500,0,true,true,scanCMGR},
        {(char *)"RUM",(char *)"AT+CMGL=\"REC UNREAD\"\r", (char *)"read unread messages",500,0,false,false},
        {(char *)"RRM",(char *)"AT+CMGL=\"REC READ\"\r", (char *)"read rec messages",500,0,false,false},
        {(char *)"RSU",(char *)"AT+CMGL=\"STO UNSENT\"\r", (char *)"read storage unsent",500,0,false,false},
        {(char *)"RAM",(char *)"AT+CMGL=\"ALL\"\r", (char *)"read all messages",500,0,false,false},
        {(char *)"RAT",(char *)"AT%%ATCMD%%\r", (char *)"raw cmd (experimental)",500,0,true,false},
        {(char *)"R\r",(char *)"--", (char *)"read simCom input buffer (internal)",0,0,false,false}


};

// for test
char str1[] = "AT+CMGR=6+CMGL: (\"REC UNREAD\",\"REC READ\",\"STO UNSENT\",\"STO SENT\",\"ALL\")";


m_item btList[] = { 
        {(char *)"BON", (char *)"AT+BTPOWER=1\r\n", (char *)"BT on",10000,0,false,false},
        {(char *)"BOF", (char *)"AT+BTPOWER=0\r\n", (char *)"BT off",10000,0,false,false},
        {(char *)"BST", (char *)"AT+BTSTATUS?\r\n", (char *)"BT status",10000,0,false,false},

        {(char *)"BHO", (char *)"AT+BTHOST?\r\n", (char *)"BT Host",10000,0,false,false}


};


int nItems = (sizeof(mList)/sizeof(mList[0]));
int btItems = (sizeof(btList)/sizeof(btList[0]));


char *upperCase(char *c,int size) {

    for(int x=0;x<size;x++) {
        if(c[x] >= 'a' && c[x] <= 'z' ) 
         c[x] -= 0x20;
    }

return c;
}



SIMcom::SIMcom() {
     dbg = new Serial(PTA2, PTA1, 115200);
     simIn = new Serial(PTC4, PTC3, 115200);

     dbg->attach(this,  &SIMcom::user_callback );
     gdbg = dbg;
     simIn->attach(this,  &SIMcom::sim_callback );     

     msgPos = 0;
     usrIPos = 0;
     
     userCmd = -1;
     memset(result,0,MSG_LEN);
     // default echo is on. Todo, read SIM
     echoOn = 1;

     // these values should be taken from storage
     setPinNumber((char *)"7331");
     setPhoneNumber((char *)"+491636643260");
     setMsg((char *)"default MSG ..");
     setNode(1);
     resetTimeWindow();
     setReplyOff();

     waiting4SimInput = false;
     simInputScanner = false;
     checkCMTI = false;
     cmgrReady = false;
     cmdResult = false;
     msgHandler = false;

     userInput = false;
     userMsgP = 0;

     led = 1;
     ldb = 1;
     // dbg->printf("Starting ...(vers: %s-%s)\r\n",__DATE__,__TIME__);
    }

void SIMcom::simulate()
{
 // simulate incoming message

    msgPos = sprintf(result,"+CMTI: \"SM\",12\r\n");

    checkCMTI = true;

    ldb = !ldb;

}
// static int cmdsend = (sizeof(cmds)/sizeof(cmds[0]));

void SIMcom::scheduler() {
      // on = 0;
  // printf("scheduler ..\r\n");
      if(!checkCMTI && !cmgrReady && !cmdResult && !msgHandler && !userInput && !simInputScanner && !getPir() ) {
        wait_ms(20);
        return;
    }

// ********************** serves for the shell  **********************************************

      // wait_ms(20);  // estimated time to get message 
      if(userInput) {
        tinysh_char_in(userInputMsg);
        userInput = false;
      }

// **************************  ******************************************
// get the result, filled in by sim_callback 

      // after sendCMD;
      if(simInputScanner) {


           _wait_ms(mList[_cmdID].wait);  // let the sim_callback finish it's work
          if(mList[_cmdID].execPF) {      // is there a funtion to process the information ?
            dbg->printf("---sendCMD ------\r\n" ); 
            getResult(&mList[_cmdID]);    
            } 
          else
            getResult();                  // just get plain information

          if( mList[_cmdID].waitEx) {           // for some request we get additional junk of data, e.g. reading messages
              wait( mList[_cmdID].waitEx);       // wait 10 second for additional message
              getResult_1(); 
              }
          waiting4SimInput = false; 
           simInputScanner = false;  // the input in sim_callback should be done and we can reset 
      }


// ****************  check CMTI (also for SIM incoming msgs) ****************************

      if(checkCMTI) {
                   // now we check the message 
          char *s = 0;
          int cmtiID = -1;

          s = strstr(result,"+CMTI");  // check for incoming msg
          if(s) {
            cmtiID = scanCMTI(s);

            // dbg->printf("CMTI found \r\n" );
            }

          if(msgPos && cmtiID != -1) {
            dbg->printf("attimout:%s\r\n",result);
            // trigger send CMGR
            cmgrReady = true;
            }
          else
            dbg->printf("attimout: msgPos 0  or no valid cmtiID\r\n");

          ldb = !ldb;
          checkCMTI = false;  // the input in sim_callback should be done and we can reset 
      }

// ****************  read the message ****************************
      if(cmgrReady) {
          ldb = !ldb;
          // cmdResult = true;
          cmgrReady = false;

          // needs to be reviewed
          char _tStr[8];
          sprintf(_tStr,"%d",lastMsgId);
          sendCMD(getCmdId("RMS"),_tStr);
          return;
      }


      if(msgHandler) {
      msgHandler = false;

        simMSGHandler();
      }

      if( getPir() ) {
        setPir(false);
        pirMsg();

        // sendMsg ...
      }

      // memset(result,0,msgPos);
      // msgPos = 0;
  }


// bool first = true;

void SIMcom::sim_callback()
{
    // we get the character and check for end of msg

 led = !led;
 // ldb = !ldb;

          // if(first) {
            // wait_us(300);

          // simIn->gets(&result[msgPos],3);
        // }
        // else
      result[msgPos] = simIn->getc();

        if( msgPos < MSG_LEN-1) {
  // if(first)
          // msgPos += 3;
// else
          msgPos++;

          // result[msgPos] = 0;   // terminate the string
        if(!(msgPos%12))
          if(waiting4SimInput)
          // dbg->printf("sc:%d\n",msgPos );
            simInputScanner = true;
          else
            checkCMTI = true;

        }
  led = !led;
// first = false;
 // ldb = !ldb;       
}



void SIMcom::user_callback()
{

          userInputMsg = dbg->getc();
          userInput = true;

}

int SIMcom::getArg(char *_arg,char *retStr) {

      dbg->printf("%s>",_arg);  
      arg = true;

      int cc=0;

      userInput = false;
      do{
                  // dbg->putc('.');
                  // retStr[cc] = dbg->getc(); // blocking input
                  if( userInput ) {                  
                     retStr[cc] = userInputMsg;
                     userInput = false;
                   
                    dbg->putc(userInputMsg);
                    // dbg->putc('+');

                     wait_ms(10);

                    if( retStr[cc] == '\r' )
                        arg = false;

                    cc++;  
                    }
                  } while( arg );   
  return 0;
};


// skip echo with offset
void SIMcom::getResult() {

    for(int x=0;x<msgPos;x++)
        dbg->putc(result[x]);
    
    msgPos = 0;
    char *s = 0;
    s = strstr(result,"\r\nOK\r\n");

    // dbg->printf("%x\r\n",s);
    dbg->putc( s?'0':'1'); 
    dbg->putc( '\r'); 
    dbg->putc( '\n'); 

    if(!s)  // for debug
        dbg->printf("%s\r\n",result);

    // return rok;
    }

void SIMcom::getResult(m_item *item) {

    dbg->putc('\r');
    dbg->putc('\n');
    dbg->printf("Pos:%d\n", msgPos);

    memset(cmdMsg,0,MSG_SIZE);

    for(int x=0;x<msgPos;x++)
        dbg->putc(result[x]);
    
    // scan the output
    int msgIdx = item->pf(result);
    dbg->printf("cmdMSG: idx %d %d\r\n",msgIdx,msgPos);
    if( !msgIdx ) {
      dbg->printf("getResult: no valid message (%d)\r\n",msgIdx);
    }


    if( msgIdx ) {  // there are still 2 characters "\r\n" after msgIdx
      int _size = msgPos-msgIdx-9;  // the message still contains a "\r\nOK\r\n" in the end
      if(_size > MSG_SIZE ) _size = MSG_SIZE;
      if( _size < 0 ) {
        dbg->printf(" invalid  msg \r\n");
        return;
      }

          dbg->printf("cmdMSG size:%d\r\n",_size);

        strncpy(cmdMsg,&result[msgIdx+1],_size); // take out the message part 
      }

    msgPos = 0;
    dbg->printf("cmdMSG:%s\r\n",cmdMsg);
    dbg->printf("cmdMSG  done .. -> msgHandler:%s\r\n",cmdMsg);

    msgHandler = true;
    }


void SIMcom::getResult_1(int offset) {
        for(int x=offset;x<msgPos;x++)
         dbg->putc(result[x]);
      
}
    

void SIMcom::sendCMD(int id,char *cmdArg) {
    char _cStr1[32];
    char _cStr2[32];
    char argStr[32]; // return string for getArg
    int  iArg = strlen(cmdArg);

  dbg->printf("sendCMD:-->%d--%s\r\n<--",id,cmdArg);
  dbg->printf("pin %s\n",getPinNumber() );


    memset(result,0,MSG_LEN);
    msgPos = 0;

    int retTime =  mList[id].wait;
    int retTimeEx = mList[id].waitEx;

    int len = strlen(mList[id].cmd);   

    if(mList[id].inputRequired) {
          // dbg->printf("arg->%d\n",argV);
          char *p1 = strstr(mList[id].cmd,"%%");
          char *p2 = strstr(p1+1,"%%");

        if( !iArg ) {
            strncpy(_cStr1,p1+2,p2-p1-2);

            _cStr1[p2-p1-1] = 0;

            getArg((char *)_cStr1,argStr);
          // else
          //     dbg->printf("dd:%s\r\n",cmdArg);
            if( strlen(argStr) < 2 ) {
              dbg->printf("sendCMD: no input... \r\n");
              return;
            }
          }

          // argRet = 2;    
          int sSize  = p1-mList[id].cmd;
          strncpy(_cStr2,mList[id].cmd,sSize);
          sprintf(_cStr2+sSize,"%s\r\n",iArg?cmdArg:argStr);
          // sprintf(_cStr2+sSize,"%s\r\n",iArg?cmdArg:argStr);

          }
    else      
          strncpy(_cStr2,mList[id].cmd,len);

    len = strlen(_cStr2);

    // dbg->printf("-->%d\r\n<--",len);
    dbg->printf("sendCMD:-->%s\r\n<--",_cStr2);

    waiting4SimInput = true;

    _cmdID = id;

    // write(_cStr2,len);
    for(int w=0;w<len;w++)
        simIn->putc(_cStr2[w]);

    }



void SIMcom::sendGPSCMD(int id) {
    // char _cStr1[32];
    char _cStr2[32];

    memset(result,0,MSG_LEN);

    int retTime =  gpsList[id].wait;
    int retTimeEx = gpsList[id].waitEx;

    int len = strlen(gpsList[id].cmd);   

    strncpy(_cStr2,gpsList[id].cmd,len);

    len = strlen(_cStr2);

    dbg->printf("%s\n",_cStr2);

    waiting4SimInput = true;

    // simIn->write(_cStr2,len);
    for(int w=0;w<len;w++)
      simIn->putc(_cStr2[w]);

    }


void SIMcom::sendBTCMD(int id) {
    // char _cStr1[32];
    char _cStr2[32];

    memset(result,0,MSG_LEN);

    int retTime =  btList[id].wait;
    int retTimeEx = btList[id].waitEx;

    int len = strlen(btList[id].cmd);   

    
    strncpy(_cStr2,btList[id].cmd,len);

    len = strlen(_cStr2);

    dbg->printf("%s\n",_cStr2);

    waiting4SimInput = true;

    // simIn->write(_cStr2,len);
    for(int w=0;w<len;w++)
      simIn->putc(_cStr2[w]);

    }



// set the echo off, 
void SIMcom::setEchoOff(void) {
   char tStr[32];
   int len = sprintf(tStr,"ATE0\r");   
   dbg->printf("%s",tStr);
   // simIn->write(tStr,len);
   for(int w=0;w<len;w++)
      simIn->putc(tStr[w]);

   wait_ms(100);
// if we get OK (return 0), the echo is off
   // echoOn = getResult();
    getResult();
}



void SIMcom::sendMsg(char *num,char *msg) {
   char tStr[64];
   int len = sprintf(tStr,"AT+CMGS=\"%s\"\r\n",num);   
   // int len = sprintf(tStr,"AT+CMGS=\"015770443612\"\r\n");   
    // waiting4SimInput = true;

    // simIn->write(tStr,len); 
   for(int w=0;w<len;w++)
      simIn->putc(tStr[w]);

    wait_ms(10);
        len = sprintf(tStr,"%s\r\n",msg);   
     // simIn->write(tStr,len);   
    for(int w=0;w<len;w++)
      simIn->putc(tStr[w]);

    waiting4SimInput = true;
 
    simIn->putc(0x1A);  // ctrl+z

     // _wait_ms(500);
     // getResult(); 

     // wait(5);      // wait 5 second for additional message
     // getResult_1();
     // waiting4SimInput = false;
 
  
}


// AT+CMGW="+85291234567",145,"STO UNSENT"<CR>This is an example for illustrating the syntax of the +CMGW AT command in SMS text mode.<Ctrl+z>

void SIMcom::writeMsg(char *num,char *msg) {
   char tStr[64];
   // int len = sprintf(tStr,"AT+CMGW=\"%s\",145,\"STO UNSENT\"\r",num);   
   int len = sprintf(tStr,"AT+CMGW=\"%s\"\r\n",num);   
    waiting4SimInput = true;

    // simIn->write(tStr,len);
    for(int w=0;w<len;w++)
      simIn->putc(tStr[w]);

    wait_ms(10);
        len = sprintf(tStr,"\"%s\"\r\n",msg);   
        // len = sprintf(tStr,"\"test message ...\"");   
    // simIn->write(tStr,len);    
    for(int w=0;w<len;w++)
      simIn->putc(tStr[w]);

     simIn->putc(0x1A);  // ctrl+z

     _wait_ms(500);
     getResult(); 

     wait(5);      // wait 10 second for additional message
     getResult_1(); 
     waiting4SimInput = false;
 
}

// interruptable wait
void SIMcom::_wait_ms(int msMax) {

    rRead = false;
    int timeout = msMax/10;
    char *s = 0;

    do {
        wait_ms(10);

        s = strstr(result,"\r\nOK\r\n");
        if(! (timeout%10) ) dbg->putc('.');

    } while( timeout-- && !s );


}

// static int vv = 0;

void SIMcom::simMSGHandler() {  
    int n = getNode();

    bool sentM = false;

    char nodeON[4];
    char nodeOFF[4];
    char node[4];

    char addPhoneNumber[8];
    char delPhoneNumber[8];


    sprintf(nodeON,"%d#1",n);
    sprintf(nodeOFF,"%d#0",n);
    sprintf(node,"%d#",n);
    
    sprintf(addPhoneNumber,"%d#+#",n);
    sprintf(delPhoneNumber,"%d#-#",n);

    char msgRet[64];

    // find the first occurence of node#
    char *cmdStart = strstr(cmdMsg,node);

    if(!cmdStart) {
      // since we call this routine every 5 sec, don't print too much
      // dbg->printf("simMSGHandler: invalid cmd %s \r\n",cmdMsg );
      return;
    }
    // dbg->printf("simMSGHandler  start: (%s) \r\n",cmdMsg );
    // dbg->printf("simMSGHandler  start: \r\n" );
  // if(!(vv++%10)) inComingMsg = true;

    // dbg->printf("dbg: %d\r\n", strstr(cmdMsg,nodeON));

    if( strlen(cmdStart) ) {
      dbg->printf("simMSGHandler: strlen  %d\n",strlen(cmdStart) );

        if( strlen(cmdStart) < 8 && strstr(cmdStart,nodeON) ) {
            // dbg->printf("aktivieren  \r\n");
            dbg->printf("aktivieren  \r\n (%s) (%s) (%d)\r\n",cmdStart,nodeON,strstr(nodeON,cmdStart));
            sprintf(msgRet,"Mobile-AlarmBox #%d / aktiviert\n %s \n",n,epoch_to_str(secc));
            sentM = true;            
        }
        else if( strlen(cmdStart) < 8 && strstr(cmdStart,nodeOFF) ) {
            dbg->printf("deaktivieren \r\n");
            sprintf(msgRet,"Mobile-AlarmBox #%d / deaktiviert\n %s \n",n,epoch_to_str(secc)); 
            sentM = true;           
            
        }
        else if( strstr(cmdStart,node) ) { // e.g. 1#0830*1830
            int h_Start,m_Start,h_Stop,m_Stop;
            int nn;
            dbg->printf("Zeitfenster aktiviert (%s)\r\n",cmdStart);

            sscanf(cmdStart,"%d#%02d%02d*%02d%02d",&nn,&h_Start,&m_Start,&h_Stop,&m_Stop);
            dbg->printf("node %d start %d %d stop %d %d \r\n",nn,h_Start,m_Start,h_Stop,m_Stop );

            setTimerStart(h_Start*60+m_Start); 
            setTimerStop(h_Stop*60+m_Stop);

            sprintf(msgRet,"Mobile-AlarmBox #%d / Zeitfenster aktiviert\n %s \n",n,epoch_to_str(secc));            
            // for tests
            // simulate();
            sentM = true;

        } 
        else if( strstr(cmdStart,addPhoneNumber) ) {
            char num[32];
            int nn;
            sscanf(cmdStart,"%d#+#%n",&nn,&num);
            sprintf(msgRet,"Mobile-AlarmBox #%d / Nummer %s aufgenommen \n %s \n",n,num,epoch_to_str(secc));
        }
        else if( strstr(cmdStart,delPhoneNumber) ) {
            char num[32];
            int nn;
            sscanf(cmdStart,"%d#+#%n",&nn,&num);
            sprintf(msgRet,"Mobile-AlarmBox #%d / Nummer %s geloescht \n %s \n",n,num,epoch_to_str(secc));
          
        }
        else{
           dbg->printf("simMSGHandler: no valid cmd found in:(%s) ->%s<-\r\n",cmdStart,nodeON);
        }

        setMsg(msgRet);

        dbg->printf("simMSGHandler:%s -- %s\n",getSMS_PN(),getMsg() );

        if(sentM&& smsReply() ) {
            sendMsg(getSMS_PN() ,(getMsg)() );
          sentM = false;
        }
    }

   // bled = !bled;
    // printf("simMSG ...\r\n");
// clear msg
    memset(cmdMsg,0,MSG_SIZE);

       // dbg->printf("\r\nsimMSGHandler  end: \r\n" );


}


int SIMcom::getCmdId(char *cmd) {
    int id = 0;

 for(int x=0;x<nItems;x++)

    if( !strncmp(cmd,mList[x].name,strlen(mList[x].name)) ) 
        id = x;

return id;
}

int scanCMGR(char *ms)
{

int len = 0;
char s1[32];
char s2[5][32];
int idx = 0;
int count = 4;

 // printf("scanCMGR:%s (%d)\r\n", ms, strlen(ms));

if( strlen(ms) < 20 ) {
  gdbg->printf("scanCMGR: msg too short (%d)\r\n",strlen(ms));
  return 0;
}
// we also could get a str pointer to +CMGR: by strstr(ms,"+CMGR:") + strlen("+CMGR:")

sscanf(ms, "%s %n", &s1,&len);
  gdbg->printf(">>:%s %d\r\n",s1 ,len);
  idx += len;

sscanf(ms+idx, "%s  %n", &s1,&len);
  gdbg->printf(">>:%s %d\r\n",s1,len);
  idx += len;

do {
    int x = sscanf(ms+idx, "\"%[^\"]\"%n", &s2[count],&len);
    if(x) 
      gdbg->printf(">>:%s %d %d %d %d\r\n",s2[count],len,strlen(s2[count]),x,count );
    else
      gdbg->printf(">>:\"\"\r\n" );
    idx += x?len+1:3;
  } while( --count);



    sim->setSMS_PN(s2[3]);
    gdbg->printf("#:%s \r\n",s2[3] );
    gdbg->printf("time:%s \r\n",s2[1] );

int hour,min,sec;
int year,day,month;
// int len;

  sscanf(s2[1], "%02d/%02d/%02d,%02d:%02d:%02d\%n",&year,&month,&day, &hour, &min,&sec,&len);

  gdbg->printf("rms time:%02d.%02d.%d %02d:%02d:%02d\r\n",day,month,year,hour,min,sec);


    gdbg->printf("msg:%s \r\n",ms+idx );


    // evalCmd(ms+idx);

return(idx);
}


int SIMcom::scanCMTI(char *msgP )
{
int len = 0;
char sp[2][16];
int msgID;
char b[8];
// char str1[] = "+CMTI: \"SM\",9";


  // char *mp = strstr(result,"CMTI");
  dbg->printf("CMTI msg>:%s \r\n",msgP);



  sscanf(msgP, "%s \"%[^\"]\",%d\%n", &sp[0], &sp[1],&msgID,&len);
  dbg->printf("CMTI>:%s msgType:%s msgID:%d len(%d)\r\n",sp[0],sp[1],msgID ,len);

  if( msgID < 80)  // what happens after 80 ?
      lastMsgId = msgID;

  if( !(msgID >= 0 && msgID < 81) ) {
    dbg->printf("\r\nscanCMTI: invalid msgID  ... \r\n" );
    return -1;
  }

  
  sprintf(b,"%d",msgID);

  dbg->printf("CMTI> id: %d\n",msgID );
  dbg->printf("scanCMTI done .... %d\n" );

  return msgID;
}


void SIMcom::pirMsg() {
  int n = getNode();
  char msgRet[64];

  int actTime =  time_in_day(secc);
  int startTime =  getTimerStart();
  int stopTime =  getTimerStop();

  printf("time window active? %s \r\n",getTimeWindowActive() ? "true" : "false" );
  printf("actual  time [min] %d \r\n",time_in_day(secc) );
  printf("start of time window [min] %d \r\n",getTimerStart() );
  printf("stop of time window [min] %d \r\n",getTimerStop() );

  if( actTime > startTime && actTime < stopTime )
    sprintf(msgRet,"Mobile-AlarmBox #%d / Bewegung detektiert \n %s \r\n",n,epoch_to_str(secc));
  else
    sprintf(msgRet,"Mobile-AlarmBox #%d / Bewegung detektiert outside of timewindow\n %s \r\n",n,epoch_to_str(secc));

  dbg->printf("-->%s ",msgRet);

}


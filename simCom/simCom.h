#ifndef __SIMCOM_H__
#define __SIMCOM_H__

#include "mbed.h"
#include <string>

#define PIN "7331"
#define DEST "0628221013"  // destination number

// #define MSG_LEN 512
#define MSG_LEN 2048
#define MSG_SIZE 256
typedef int (*PFI_CH)(char *);

// +491636643260

typedef struct menuItem {
  // char id;
  char *name;
  char *cmd;
  char *desc;
  int  wait;
  int  waitEx;
  bool inputRequired;
  bool execPF;
  PFI_CH pf;
} m_item;

char *upperCase(char *,int);
int scanCMGR(char *);


// class SIMcom : public Serial
class SIMcom
{
    
public:
    // SIMcom(PinName tx, PinName rx, int baud = 115200); 
    SIMcom(); 
    Serial *dbg;
    Serial *simIn;
    int  _getc() { return dbg->getc(); };
    void  _putc(unsigned char c) { dbg->putc(c); };

    void sendMsg(char *,char *);
    void writeMsg(char *,char *);
    void sim_callback();
    void user_callback();
    void setEchoOff(void);

    // void msgHandler(char *);
    void getResult(char *,int *len);
    void getResult();
    void getResult(m_item *);
//    int getResult(int offset = 0);
    void getResult_1(int offset = 0);
    // void sendCMD(char *str,int retTime = 10, int retTimeEx = 0 ); // default return time 10 ms, extended return time in sec
    void sendCMD(int ,char *cmdArg = (char *)""); // 
    void sendGPSCMD(int ); // 
    void sendBTCMD(int ); // 

    // void mTime(char *str = (char *)COMPILETIME) { cTime = str; } ;
    char *getCTime(void) { return cTime; };

    void setUserCmd(int cmd) {userCmd = cmd;};
    int getUserCmd(void) {  int t = userCmd;
                            userCmd = -1;
                            return t; };

    int getArg(char *,char *);
    void _wait_ms(int );

    void test(int argc, char **argv) { printf("james ...\r\n"); };

    char *getPhoneNumber(void) { return phoneNumber; };
    void setPhoneNumber( char *str) { strcpy(phoneNumber,str); };

    char *getPinNumber(void) { return pinNumber; };
    void setPinNumber(char *str) { strcpy(pinNumber,str); };

    char *getMsg(void) { return msg; };
    void setMsg(char *str) { strcpy(msg,str); };

    int getCmdId(char *);

    void simMSGHandler(void);
    int  scanCMTI(char *);
    void scheduler(void);

    // void cmdCMGR();

    void simulate(void);

    void setNode(int s) { nodeID = s; };
    int  getNode(void) { return nodeID; };

    void pirMsg(void);
    void setSMS_PN(char *str) { strncpy(smsPNum,str,strlen(str) ); };
    char *getSMS_PN(void) {return smsPNum; };

    void setPir(bool v) { pirTrig = v; };
    bool getPir(void) { return pirTrig; };

    void setTimeWindowActive(bool arg ) { timeWindow = arg; };
    bool getTimeWindowActive(void) { return timeWindow; };
    void setTimerStart(int minInDay) { timerStart = minInDay; };
    int  getTimerStart(void) { return timerStart; };
    void setTimerStop(int minInDay) { timerEnd = minInDay; };
    int  getTimerStop(void) { return timerEnd; };

    // we open the window for the whole day
    void resetTimeWindow(void ) { timerStart = 0; timerEnd = 24*60; };


    void setReplyOn() {  reply = true; };
    void setReplyOff() {  reply = false; };

    bool smsReply() { return reply; }

private:
//    string result;    
    char result[MSG_LEN]; // support message length   
    int msgPos;

    char userInputMsg;
    bool userInput;
    int userMsgP;
    // char userInput[MSG_LEN]; // support message length   
    int usrIPos;

    int echoOn;
    bool arg;
    int argV;

    // bool arkOK;
    bool reply;

    bool rRead;
    int userCmd;
    char *cTime;

    char phoneNumber[64];
    char pinNumber[16];
    char msg[MSG_SIZE];

    char cmdMsg[MSG_SIZE];

    bool timeWindow;
    int timerStart;     // start time in minute
    int timerEnd;       // end time in minute

    bool pirTrig;
   
    bool waiting4SimInput;
    bool simInputScanner;
    bool checkCMTI;
    bool cmgrReady;
    bool cmdResult;
    bool msgHandler;

    int lastMsgId;

    int nodeID;

    int _cmdID;

    char  smsPNum[32];  // 


};





#endif
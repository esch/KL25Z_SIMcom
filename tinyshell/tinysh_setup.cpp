/*
 * tinysh.c
 *
 * Minimal portable shell
 *
 * E. Schaefer
 */

#include "tinysh.h"
#include "simCom.h"
#include "time_utils.h"
#include "sys.h"

#include "FreescaleIAP.h"


extern SIMcom *sim;
extern Sys     *sys;

extern time_t secc;

extern m_item mList[];
extern int nItems;

extern m_item gpsList[];
extern int gpsItems;

extern m_item btList[];
extern int btItems;


void storage2flash();


static  char *cmds [] = {
                        (char *)"Syntax: help \n\r",
                        (char *)"Function: \n\r",
                        (char *)"  'help'  print this help info\n\r",
                        };

static  char *cmdsGPS [] = {
                        (char *)"Syntax: help \n\r",
                        (char *)"Function: \n\r",
                        (char *)"  'help'  print this help info\n\r",
                        };

// extern int printuse(char *);
void printuse(void)
{
    int cc = 0;
            printf(cmds[0], strlen( cmds[0]) );            
            printf(cmds[1], strlen( cmds[1]) );            

    while (cc < nItems) {
            printf("\'%s\'\t- %s \r\n", mList[cc].name, mList[cc].desc );            

         cc++;
    }
     printf("(ver: %s- %s )\r\n", __DATE__, __TIME__ );            

}
// 


void printGPSuse(void)
{
    int cc = 0;
            printf(cmdsGPS[0], strlen( cmdsGPS[0]) );            
            printf(cmdsGPS[1], strlen( cmdsGPS[1]) );            

    while (cc < gpsItems) {
            printf("\'%s\'\t- %s \r\n", gpsList[cc].name, gpsList[cc].desc );            

         cc++;
    }


}



void printBTuse(void)
{
    int cc = 0;
            printf(cmds[0], strlen( cmds[0]) );            
            printf(cmds[1], strlen( cmds[1]) );            

    while (cc < btItems) {
            printf("\'%s\'\t- %s \r\n", btList[cc].name, btList[cc].desc );            

         cc++;
    }


}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/////// SIM SHELL EXAMPLE AS THREAD ///////////////////////////


void kl25z_sim(int argc, char **argv)
{
    printf("sim command called  %s\r\n",epoch_to_str(secc));
    for(int i=0; i<argc; i++) {
        printf("argv[%d]=\"%s\"\r\n",i,argv[i]);
    }

    // printf("item %s command called\r\n", tinysh_get_arg());
    printf("argc %d \r\n", argc);

    if(argc<2) {
        printuse();
        return;
    }


    upperCase(argv[1],strlen(argv[1]) );
    printf("-->%s\n",argv[1] );

    int cmdI = -1;


    if( !strcmp(argv[1],"MSG") ) {
        printf("msg sent to %s \n",sim->getPhoneNumber());
        sim->sendMsg(sim->getPhoneNumber(),sim->getMsg() );
    }
    else if(!strcmp(argv[1],"MSW")) {
         printf("msg localy stored for %s \n",sim->getPhoneNumber());
        sim->writeMsg(sim->getPhoneNumber(),sim->getMsg() );
       
    }

    else {
        for(int x=0;x<nItems;x++)
            if( !strncmp(argv[1],mList[x].name,strlen(mList[x].name)) ) {
                cmdI = x;
                // sim->sendCMD(x);     
                break;
            }

        if(cmdI != -1)
           sim->sendCMD(cmdI);
        else
            printf("cmd %s not found \r\n",argv[1]);
    }
                  // setUserCmd(x);  

}



tinysh_cmd_t cmd_sim= {0,(char *)"sa",(char *)"sim AT cmd",(char *)"[args]",kl25z_sim,0,0,0};


void kl25z_simGPS(int argc, char **argv)
{
    // char _str[1024];

    printf(" gps command called\r\n");
    printf(" Time: %s (%d)\r\n", epoch_to_str(secc),secc );

    for(int i=0; i<argc; i++) {
        printf("argv[%d]=\"%s\"\r\n",i,argv[i]);
    }

    // printf("item %s command called\r\n", tinysh_get_arg());

    if(argc<2) {
        printGPSuse();
        return;
    }

    upperCase(argv[1],strlen(argv[1]) );
    printf("-->%s\n",argv[1] );

    int cmdI = -1;

    for(int x=0;x<gpsItems;x++)
            if( !strncmp(argv[1],gpsList[x].name,strlen(gpsList[x].name)) ) {
                cmdI = x;
                // sim->sendCMD(x);     
                break;
            }

        if(cmdI != -1){
            sim->sendGPSCMD(cmdI);            
           // sim->sendCMD(cmdI);
        }
        else
            printf("cmd %s not found \r\n",argv[1]);

}

tinysh_cmd_t cmd_simGPS= {0,(char *)"gps",(char *)"simGPS AT cmd",(char *)"[args]",kl25z_simGPS,0,0,0};

void kl25z_simBT(int argc, char **argv)
{
    // char _str[1024];

    printf(" bt command called\r\n");
    printf(" Time: %s (%d)\r\n", epoch_to_str(secc),secc );

    for(int i=0; i<argc; i++) {
        printf("argv[%d]=\"%s\"\r\n",i,argv[i]);
    }

    // printf("item %s command called\r\n", tinysh_get_arg());

    if(argc<2) {
        printBTuse();
        return;
    }

    upperCase(argv[1],strlen(argv[1]) );
    printf("-->%s\n",argv[1] );

    int cmdI = -1;

    for(int x=0;x<btItems;x++)
        if( !strncmp(argv[1],btList[x].name,strlen(btList[x].name)) ) {
            cmdI = x;
            // sim->sendCMD(x);     
            break;
        }

    if(cmdI != -1){
        sim->sendBTCMD(cmdI);            
       // sim->sendCMD(cmdI);
    }
    else
        printf("cmd %s not found \r\n",argv[1]);
    

}

tinysh_cmd_t cmd_simBT= {0,(char *)"bt",(char *)"simBT AT cmd",(char *)"[args]",kl25z_simBT,0,0,0};


void kl25z_pin(int argc, char **argv)
{
    // char _str[1024];

    if(argc<2) {
        printf("pin set 7331 \r\n");
        printf("pin get \r\n");
        return;
    }


    upperCase(argv[1],strlen(argv[1]) );
    printf("\r\n-->%s\n",argv[1] );

    if( !strcmp(argv[1],"GET") ) { 
        printf("PIN %s \r\n",sim->getPinNumber() );
        }
    else if( !strcmp(argv[1],"SET") ) {
        if(argc==3) {
            sim->setPinNumber(argv[2]);
        }
        else
            printf("no number \r\n");
        
    }
    else {
        printf("unknown command \r\n");   
    }    
   // printf("%s\n",_str );

}

tinysh_cmd_t cmd_pin= {0,(char *)"pin",(char *)"pin number [get/set]",(char *)"[args]",kl25z_pin,0,0,0};

void kl25z_snum(int argc, char **argv)
{

    if(argc<2) {
        printf("pn set (e.g. 0XXXXX or +41XXXXXXXX) \r\n");
        printf("pn get \r\n");
        return;
    }


    upperCase(argv[1],strlen(argv[1]) );
    printf("\r\n-->%s\n",argv[1] );

    if( !strcmp(argv[1],"GET") ) {
        printf("phone number %s \r\n",sim->getPhoneNumber() ); 
        }
    else if( !strcmp(argv[1],"SET") ) {
        if(argc==3) {
            sim->setPhoneNumber(argv[2]);
        }
        else
            printf("no number \r\n");
    }
    else {
            printf("unknown command \r\n");        
    }    

}


tinysh_cmd_t cmd_snum= {0,(char *)"pn",(char *)"sms phone number [get/set]",(char *)"[args]",kl25z_snum,0,0,0};


void kl25z_msg(int argc, char **argv)
{

    if(argc<2) {
        printf("msg set \"this is a message\" \r\n");
        printf("msg get \r\n");
        return;
    }

    time_t seconds = time(NULL);
    upperCase(argv[1],strlen(argv[1]) );
    printf("\r\n-->%s\r\n",argv[1] );

int len=0;
char ts[128];

    for(int i=2;i<argc;i++) 
        len += sprintf(ts+len,"%s ",argv[i]);

    if( !strcmp(argv[1],"GET") ) {
        printf("msg: %s \r\n",sim->getMsg() ); 
        }
    else if( !strcmp(argv[1],"SET") ) {
        if(strlen(ts)) {
            sim->setMsg(ts);
        }
        else
            printf("no msg \r\n");
    }
    else {
            printf("unknown command \r\n");        
    }    

}


tinysh_cmd_t cmd_msg= {0,(char *)"msg",(char *)"write message [get/set]",(char *)"[args]",kl25z_msg,0,0,0};

////////////////////////////////////////////////////////////////

void kl25z_time(int argc, char **argv)
{
    // char _str[1024];

    if(argc<2) {
        printf("time set 20/03/2017 23:10:02 [dd.mm.yyyy hh:mm:ss]\r\n");
        printf("time get \r\n");
        return;
    }


    upperCase(argv[1],strlen(argv[1]) );
    printf("\r\n-->%s\n",argv[1] );

    if( !strcmp(argv[1],"GET") ) { 
        printf("TIME %s \r\n",sys->getTime() );
        }
    else if( !strcmp(argv[1],"SET") ) {
        if(argc>2) {
            char tBuf[32];
            sprintf(tBuf,"%s %s\r\n",argv[2],argv[3]);
            sys->setTime(tBuf);
        }
        else
            printf("no number \r\n");
        
    }
    else {
        printf("unknown command \r\n");   
    }    
   // printf("%s\n",_str );

}

tinysh_cmd_t cmd_time= {0,(char *)"time",(char *)"system time [get/set] [dd.mm.yyyy hh:mm:ss]",(char *)"[args]",kl25z_time,0,0,0};

////////////////////////////////////////////////////////////////////////////////////////////

void kl25z_smsReply(int argc, char **argv)
{
    // char _str[1024];

    if(argc<2) {
        printf("sms Reply is %s \r\n", sim->smsReply() ? "On":"Off");
        return;
    }


    upperCase(argv[1],strlen(argv[1]) );
    printf("\r\n-->%s\n",argv[1] );

    if( !strcmp(argv[1],"ON") ) { 
        sim->setReplyOn();
        }
    else if( !strcmp(argv[1],"OFF") ) {
        sim->setReplyOff();
        }

}

tinysh_cmd_t cmd_reply= {0,(char *)"rp",(char *)"sms reply [on/off]",(char *)"[args]",kl25z_smsReply,0,0,0};



void kl25z_simu(int argc, char **argv)
{
    printf("\r\nsimulate called\r\n");
    // for(int i=0; i<argc; i++) {
    //     printf("argv[%d]=\"%s\"\r\n",i,argv[i]);
    // }
    sim->simulate();
    // printf("item %s command called\r\n", tinysh_get_arg());
}

tinysh_cmd_t cmd_simu= {0,(char *)"is",(char *)"simulate input msg",(char *)"[args]",kl25z_simu,0,0,0};

void kl25z_flash(int argc, char **argv)
{
    printf("simulate called\r\n");
    // for(int i=0; i<argc; i++) {
    //     printf("argv[%d]=\"%s\"\r\n",i,argv[i]);
    // }
    storage2flash();
    // printf("item %s command called\r\n", tinysh_get_arg());
}

tinysh_cmd_t cmd_flash= {0,(char *)"fl",(char *)"flash access (just a test case)",(char *)"[args]",kl25z_flash,0,0,0};


void tinysh_char_out(unsigned char c)
{
    sim->_putc(c);
}

//void tinyshell_thread(void const *args)
void tinyshell_init(void)
{
    tinysh_set_prompt((char *)"$ ");
    tinysh_add_command(&cmd_sim);
    tinysh_add_command(&cmd_pin);
    tinysh_add_command(&cmd_snum);
    tinysh_add_command(&cmd_msg);
    tinysh_add_command(&cmd_simGPS);
    tinysh_add_command(&cmd_simBT);
    tinysh_add_command(&cmd_time);
    tinysh_add_command(&cmd_reply);

    tinysh_add_command(&cmd_simu);
    tinysh_add_command(&cmd_flash);
}


/// for test case


void storage2flash()
{
    int address = flash_size() - SECTOR_SIZE;           //Write in last sector
    int *fdata = (int*)address;  // pointer to flash data
    
    printf("flash size %d sector size %d\r\n",flash_size(),SECTOR_SIZE );
    printf("fdata: %d\r\n",fdata[0] );
    
   //By default flash is initialized at 0xFF, this is signed -1, so now we know
    //the program runs for the first time. You of course need to make sure your program
    //never writes -1 to this variable if you use this method
    
    //Alternatively you could also do the same, but with a seperate "initial run" variable added,
    //so your other variables can take any value
    if (fdata[0] == -1) {
        printf("Initial run\r\n");
        printf("Writing 42 and 42\r\n");
        erase_sector(address);
        int newvalues[2] = {42, 42};
        program_flash(address,(char*) newvalues, 8);     //Two integers of 4 bytes = 8 bytes
        printf("Initial run done\n");
        return;
    }
    printf("Current = %d and %d, new is %d and %d\r\n", fdata[0], fdata[1], fdata[0]+1, fdata[1]-1);
    int newvalues[2] = {fdata[0]+1, fdata[1]-1};
    erase_sector(address);
    program_flash(address, (char*) newvalues, 8);
     
    
    }
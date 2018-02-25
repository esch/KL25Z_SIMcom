#include "mbed.h"
#include "tinysh.h"
#include "simCom.h"
#include "time_utils.h"
#include "sys.h"


#include <sstream>
#include <string>

extern time_t secc;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


PwmOut rled(LED1);
PwmOut gled(LED2);
PwmOut bled(LED3);
SIMcom  *sim;
Sys     *sys;

// only PTA or PTD !! 
InterruptIn  pirInterrupt(PTD7);

////////////////////////////////////////////////////////////////
Ticker softTime;

void timeStep() {
    rled = !rled;
    secc += 2;
}


void pirTrigger() {
    sim->setPir(true);
    // printf("pirTrig  ..\r\n");
}



////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

int main()
{
    sim = new SIMcom();  // tx ,rx
    sys = new Sys();
    //kl25z led power is inverted 1=off 0=on.
    rled=0.99;
    gled=0.99;
    bled=0.99;


    secc = time_when_compiled(__DATE__,__TIME__);

    // pc.attach( &tinyshell_thread, Serial::RxIrq ); 
    tinyshell_init();

    softTime.attach(&timeStep, 2.0);
    pirInterrupt.rise(&pirTrigger);

    while(true) {
        
        
        sim->scheduler();
        wait_ms(20);
    }

}

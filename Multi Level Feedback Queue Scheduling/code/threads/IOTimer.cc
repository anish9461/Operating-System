#include "copyright.h"
#include "timer.h"
#include "main.h"
#include "sysdep.h"
#include "kernel.h"


//IOTimer for the IO operations
IOTimer::IOTimer(bool doRandom, CallBackObj *toCall)
{
    randomize = doRandom;
    callPeriodically = toCall;
    disable = FALSE;
}

//IO Timer callback for the IO operations
void 
IOTimer::CallBack() 
{
    callPeriodically->CallBack();
}

//Set interrupts for the IO operations
void
IOTimer::SetInterrupt(int d) 
{
       kernel->interrupt->Schedule(this, d, IOTimerInt);   
}

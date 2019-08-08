
#ifndef IOTIMER_H
#define IOTIMER_H

#include "copyright.h"
#include "utility.h"
#include "callback.h"

// The following class defines a hardware timer. 
class IOTimer : public CallBackObj {
  public:
    IOTimer(bool doRandom, CallBackObj *toCall);
				// Initialize the timer, and callback to "toCall"
				// every time slice.
    virtual ~IOTimer() {}
    
    void Disable() { disable = TRUE; }
    				// Turn timer device off, so it doesn't
				// generate any more interrupts.
    void SetInterrupt(int d);  	// cause an interrupt to occur in the
    				// the future after a fixed or random
				// delay

  private:
    bool randomize;		// set if we need to use a random timeout delay
    CallBackObj *callPeriodically; // call this every TimerTicks time units 
    bool disable;		// turn off the timer device after next
    				// interrupt.
    
    void CallBack();		// called internally when the hardware
				// timer generates an interrupt

    
};

#endif // TIMER_H

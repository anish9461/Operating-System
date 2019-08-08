// alarm.h 
//	Data structures for a software alarm clock.
//
//	We make use of a hardware timer device, that generates
//	an interrupt every X time ticks (on real systems, X is
//	usually between 0.25 - 10 milliseconds).
//
//	From this, we provide the ability for a thread to be
//	woken up after a delay; we also provide time-slicing.
//
//	NOTE: this abstraction is not completely implemented.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef IOALARM_H
#define IOALARM_H

#include "copyright.h"
#include "utility.h"
#include "callback.h"
#include "timer.h"
#include "request.h"
#include "IOTimer.h"


// The following class defines a software alarm clock. 
class IOAlarm : public CallBackObj {
  public:
    IOAlarm(bool doRandomYield);	// Initialize the timer, and callback 
				// to "toCall" every time slice.
    ~IOAlarm() { delete IOtimer; }
    
    void WaitUntil(int x);	// suspend execution until time > now + x
                                // this method is not yet implemented
    IOTimer *IOtimer;		// the hardware timer device

  private:
    

    void CallBack();		// called when the hardware
				// timer generates an interrupt
};

#endif // IOALARM_H

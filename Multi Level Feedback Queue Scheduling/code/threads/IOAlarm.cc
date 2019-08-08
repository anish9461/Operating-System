

#include "copyright.h"
#include "IOAlarm.h"
#include "main.h"
#include "kernel.h"
#include "IOHandler.h"
#include "list.h"
#include "stats.h"
#include "IOTimer.h"

//constructor for IO Alarm
IOAlarm::IOAlarm(bool doRandom)
{
  //create object of IOtimer for IO operations
    IOtimer = new IOTimer(doRandom, this);
}


//IOAlarm callback when the interrupt occurs
void 
IOAlarm::CallBack() 
{
    //list iterator for the event queue
    ListIterator<request*> *eQueuelist = new ListIterator<request*>(kernel->eventQueue);
    //IOHandler instance
    IOHandler *IHandler;
    while(!eQueuelist->IsDone())
    {
        request *r = eQueuelist->Item();
        //check if handler is write
        if(r->getType() == "Write" && (kernel->stats->totalTicks >= r->getCompletionTime()))
        {
            IHandler->write(r);     
        }
        //check if handler is read
        else if(r->getType() == "Read" && (kernel->stats->totalTicks >= r->getCompletionTime()))
        {
            IHandler->read(r);
        }
        eQueuelist->Next();
    }   
}

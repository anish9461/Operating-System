#include "thread.h"
#include "interrupt.h"
#include "IOHandler.h"
#include "main.h"
#include "stats.h"

//constructor for IO Handler
IOHandler::IOHandler()
{
}
        
//Write IO handler
void IOHandler::write(request *r)
{
    //remove the served IO from the event queue
    kernel->eventQueue->Remove(r);
    Thread *t = r->getThreadState();
    int priority = t->getThreadPriority();
    cout << endl << endl;
    cout << ">------------< Serving IO Write request >--------------<" << endl;
    cout << "Write IOHandler for thread t" << t->getThreadID() << " with priority : " <<  priority << endl;
    cout << "Wrote : " << r->getWriteOutput() << endl;
    //increment the priority of the thread by 1. Do not increment if already in highest priority
    if(priority < 4)
    {
        priority++;
        t->setThreadPriority(priority);
    }
    cout << "Dynamically changing Priority of the thread t" << t->getThreadID()<< " from "<< (priority - 1) << " to " << priority << endl;
    cout << "Write request served at time : " << kernel->stats->totalTicks << endl; 
    cout << ">---------------------------------------------------------<"<< endl << endl;
    //put the thread to ready to run
    kernel->scheduler->ReadyToRun(t); 
}

void IOHandler::read(request *r)
{
    //remove the served IO from the event queue
    kernel->eventQueue->Remove(r);
    cout << endl << endl;
    cout << ">------------< Serving IO Read request >--------------< " << endl;
    Thread *t = r->getThreadState();
    int priority = t->getThreadPriority();
    cout << "Read IOHandler for thread t" << t->getThreadID() << " with priority : " <<  priority << endl;
    cout << "Information Read : " << r->getReadOutput() << endl;
    //increment the priority of the thread by 1. Do not increment if already in highest priority
    if(priority < 4)
    {
        priority++;
        t->setThreadPriority(priority);
    }
    cout << "Dynamically changing Priority of the thread t" << t->getThreadID()<< " from "<< (priority - 1) << " to " << priority << endl;
    cout << "Read request served at time : " << kernel->stats->totalTicks << endl;
    cout << ">---------------------------------------------------------<"<< endl << endl;
    //put the thread to ready to run
    kernel->scheduler->ReadyToRun(t); 
    
}
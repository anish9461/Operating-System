#include "string.h"
#include "IOInterrupt.h"
#include "thread.h"
#include "IOAlarm.h"
#include "stats.h"
#include "main.h"
#include "interrupt.h"

//constructor for IOInterrupt
IOInterrupt::IOInterrupt()
{

}

//IO Write interrupt
void IOInterrupt::IOWrite(string s)
{
    //create new request to store following details
    request *r = new request();
    r->setWriteOutput(s);
    r->setThreadState(kernel->currentThread);
    r->setDelay(200 + (rand() % (101)));
    r->setType("Write");
    //set the completion time of the IO operation
    r->setCompletionTime(kernel->stats->totalTicks + r->getDelay());
    cout << endl << endl;
    cout << ">--------------------------< Register Write >-----------------------<" << endl;     
    cout << "Register Write with Thread ID: " << kernel->currentThread->getThreadID() << endl;
    cout << "The Write request will be served after the time : " << r->getCompletionTime() << endl;
    cout << "The priority of the thread is initially set to : " << kernel->currentThread->getThreadPriority() << endl;
    cout << ">-------------------------------------------------------------------<" << endl << endl; 
    //insert the request into the event queue
    kernel->eventQueue->Insert(r);
    //set the interrupt for the random delay
    kernel->IOalarm->IOtimer->SetInterrupt(r->getDelay());
    //put the thread to sleep
    kernel->interrupt->SetLevel(IntOff);
    kernel->currentThread->Sleep(FALSE);
}

//IO interrupt for Read
string IOInterrupt::IORead(string s)
{
    //create new request to store following details
    request *r = new request();
    s = "A Read string";
    r->setReadOutput(s);
    r->setThreadState(kernel->currentThread);
    r->setDelay(400 + (rand() % (101)));
    r->setType("Read");
    //set the completion time of the IO operation
    r->setCompletionTime(kernel->stats->totalTicks + r->getDelay());
    cout << endl << endl;
    cout << ">--------------------------< Register Read >-----------------------<" << endl;
    cout << "Register Read with Thread ID : " << kernel->currentThread->getThreadID() << endl;
    cout << "The Read request will be served after the time : " << r->getCompletionTime() << endl << endl; 
    cout << ">-------------------------------------------------------------------<" << endl << endl;
    //insert the request into the event queue
    kernel->eventQueue->Insert(r);
    //set the interrupt for the random delay
 	kernel->IOalarm->IOtimer->SetInterrupt(r->getDelay());
     //put the thread to sleep
    kernel->interrupt->SetLevel(IntOff);
    kernel->currentThread->Sleep(FALSE);
    return s;
}
// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"
#include "thread.h"
#include "timer.h"
#include "alarm.h"
#include "stats.h"





//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------
//creating four level of lists for MLFQ
Scheduler::Scheduler()
{ 
    toBeDestroyed = NULL;
    Level1 = new List<Thread *>;
    Level2 = new List<Thread *>;
    Level3 = new List<Thread *>;
    Level4 = new List<Thread *>;
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    delete Level1;
    delete Level2;
    delete Level3;
    delete Level4; 
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());
 
    thread->setStatus(READY);

//Putting the threads to respective levels based on their priority and storing the time when the list was put into the ready list

    if(thread->getThreadPriority() >= 4)
        Level1->Append(thread);
    else if(thread->getThreadPriority() == 3)
    {
        thread->setReadyListTime(kernel->stats->totalTicks);
        Level2->Append(thread);
    }
    else if(thread->getThreadPriority() == 2)
    {
        thread->setReadyListTime(kernel->stats->totalTicks);
        Level3->Append(thread);
    }
    else if(thread->getThreadPriority() == 1)
    {
        thread->setReadyListTime(kernel->stats->totalTicks);
        Level4->Append(thread);
    }
 

}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    
    Alarm *alarmobj = kernel->alarm;
    Timer *timerobj = alarmobj->timer;
 
 //Performing aging to prevent starvation
    int age;
    //check if level is empty
     if(!Level2->IsEmpty())
     {
         //iterate through the list
         ListIterator<Thread*> *Level2list = new ListIterator<Thread*>(Level2);
         while(!Level2list->IsDone())
         {
             Thread *t = Level2list->Item();
             //calculate the age of the corresponding thread
             age = kernel->stats->totalTicks - t->getReadyListTime();
             //if the age is greater than the threshold i.e. 400, put the thread to higher order ready queue
             if(age > 800)
             {
                 cout << endl << endl;
                 cout << ">-----------------------------------< Aging >---------------------------------<" << endl;
                 cout << "Moving the " << t->getName() << " from Level 2 to Level 1 because of Starvation " << endl;
                 cout << "Dynamically changing priority "<< t->getName() << " from 3 to 4" << endl;
                 cout << ">-----------------------------------------------------------------------------<" << endl << endl;
                 //set the priority of the thread
                 t->setThreadPriority(4);
                 Level1->Append(t);
                 Level2->Remove(t);
                 ListIterator<Thread*> *Level2list = new ListIterator<Thread*>(Level2);
             }
             Level2list->Next();
         }
     }

     //check if level is empty
     if(!Level3->IsEmpty())
     {
         //iterate through the list
         ListIterator<Thread*> *Level3list = new ListIterator<Thread*>(Level3);
         while(!Level3list->IsDone())
         {
             Thread *t = Level3list->Item();
             //calculate the age of the corresponding thread
             age = kernel->stats->totalTicks - t->getReadyListTime();
             //if the age is greater than the threshold i.e. 400, put the thread to higher order ready queue
             if(age > 800)
             {
                 cout << endl << endl;
                 cout << ">-------------------------------< Aging >-------------------------------<" << endl;
                 cout << "Moving the " << t->getName() << " from Level 3 to Level 2 because of Starvation " << endl;
                 cout << "Dynamically changing priority "<< t->getName() << " from 2 to 3" << endl;
                 cout << ">-----------------------------------------------------------------------<" << endl << endl;
                 //set the priority of the thread
                 t->setThreadPriority(3);
                 Level2->Append(t);
                 Level3->Remove(t);
                 ListIterator<Thread*> *Level3list = new ListIterator<Thread*>(Level3);
             }
             Level3list->Next();
         }
     }

     //check if level is empty
     if(!Level4->IsEmpty())
     {
         //iterate through the list
         ListIterator<Thread*> *Level4list = new ListIterator<Thread*>(Level4);
         while(!Level4list->IsDone())
         {
             Thread *t = Level4list->Item();
             //calculate the age of the corresponding thread
             age = kernel->stats->totalTicks - t->getReadyListTime();
             //if the age is greater than the threshold i.e. 400, put the thread to higher order ready queue
             if(age > 1800)
             {
                 cout << endl << endl;
                 cout << ">-------------------------------------< Aging >-----------------------------------<" << endl;
                 cout << "Moving the " << t->getName() << " from Level 4 to Level 3 because of Starvation " << endl;
                 cout << "Dynamically changing priority of "<< t->getName() << " from 1 to 2" << endl;
                 cout << ">---------------------------------------------------------------------------------<" << endl << endl;
                 //set the priority of the thread
                 t->setThreadPriority(2);
                 Level3->Append(t);
                 Level4->Remove(t);
                 ListIterator<Thread*> *Level4list = new ListIterator<Thread*>(Level4);
             }
             Level4list->Next();
         }
     }

    //Removing the next thread to be run and removing pending interrupts if any for sleeping or finished threads 
    if(!Level1->IsEmpty())
    {
        //iterate through pending interrupts list
               ListIterator<PendingInterrupt *> *pendinglist = new ListIterator<PendingInterrupt *>(kernel->interrupt->pending);
               while(!pendinglist->IsDone())
               {
                   //remove TimerInt thread
                   if(pendinglist->Item()->type == 0)
                   {
                       kernel->interrupt->pending->Remove(pendinglist->Item());
                       break;
                   }
                   pendinglist->Next();
               }
               //set the interrupt for quantum value for level 1        
                timerobj->SetInterrupt(kernel->q1);
                return Level1->RemoveFront();      
    }
    else if(!Level2->IsEmpty())
    {
        //iterate through pending interrupts list
        ListIterator<PendingInterrupt *> *pendinglist = new ListIterator<PendingInterrupt *>(kernel->interrupt->pending);
               while(!pendinglist->IsDone())
               {
                   //remove TimerInt thread
                   if(pendinglist->Item()->type == 0)
                   {
                       kernel->interrupt->pending->Remove(pendinglist->Item());
                       break;
                   }
                   pendinglist->Next();
               } 
               //set the interrupt for quantum value for level 2  
                timerobj->SetInterrupt(kernel->q2);
                return Level2->RemoveFront();
    }
    else if(!Level3->IsEmpty())
    {
        //iterate through pending interrupts list
        ListIterator<PendingInterrupt *> *pendinglist = new ListIterator<PendingInterrupt *>(kernel->interrupt->pending);
               while(!pendinglist->IsDone())
               {
                    //remove TimerInt thread
                   if(pendinglist->Item()->type == 0)
                   {
                       kernel->interrupt->pending->Remove(pendinglist->Item());
                       break;
                   }
                   pendinglist->Next();
               } 
               //set the interrupt for quantum value for level 3 
                timerobj->SetInterrupt(kernel->q3);
                return Level3->RemoveFront();       
    }
    else if(!Level4->IsEmpty())
    {
        //iterate through pending interrupts list
        ListIterator<PendingInterrupt *> *pendinglist = new ListIterator<PendingInterrupt *>(kernel->interrupt->pending);
               while(!pendinglist->IsDone())
               {
                   //remove TimerInt thread
                   if(pendinglist->Item()->type == 0)
                   {
                       kernel->interrupt->pending->Remove(pendinglist->Item());
                       break;
                   }
                   pendinglist->Next();
               } 
                //set the interrupt for quantum value for level 4
                timerobj->SetInterrupt(kernel->q4);
                return Level4->RemoveFront();
    }
    else
        return NULL;

}
//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void
Scheduler::Run (Thread *nextThread, bool finishing)
{
   // cout << "in run running " << kernel->currentThread->getName() << endl;
    Thread *oldThread = kernel->currentThread;
    
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
         ASSERT(toBeDestroyed == NULL);
	 toBeDestroyed = oldThread;
    }
    
    if (oldThread->space != NULL) {	// if this thread is a user program,
        oldThread->SaveUserState(); 	// save the user's CPU registers
	oldThread->space->SaveState();
    }
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    
    
//changing the priority of the thread after the context switch
 int priority = kernel->currentThread->getThreadPriority();
     switch(priority)
     {
         case 4 : {
             //setting thread priority to 3
             kernel->currentThread->setThreadPriority(3);
             cout << endl << endl;
             cout << ">-----------------------< Context Switch >-----------------<" << endl;
             cout << "Switching from " << kernel->currentThread->getName() << " to " << nextThread->getName()<< " : Time quantum q1 = "<< kernel->q1 << " expired" <<endl;  
             cout << "setting priority of " << kernel->currentThread->getName() << " to 3" << endl;
             cout << ">-----------------------------------------------------------<" << endl << endl;
             break;
         }
         case 3 : {
             //setting thread priority to 2
             kernel->currentThread->setThreadPriority(2);
             cout << endl << endl;
             cout << ">-----------------------< Context Switch >-----------------<" << endl;
             cout << "Switching from " << kernel->currentThread->getName() << " to " << nextThread->getName() << " : Time quantum q2 = "<< kernel->q2 << " expired" << endl;  
             cout << "setting priority of " << kernel->currentThread->getName() << " to 2" << endl;
             cout << ">-----------------------------------------------------------<" << endl << endl;
             break;
         }
         case 2 : {
             //setting thread priority to 1
             kernel->currentThread->setThreadPriority(1);
             cout << endl << endl;
             cout << ">-----------------------< Context Switch >-----------------<" << endl;
             cout << "Switching from " << kernel->currentThread->getName() << " to " << nextThread->getName() << " : Time quantum q3 = "<< kernel->q3 << " expired" << endl;  
             cout << "setting priority of " << kernel->currentThread->getName() << " to 1" << endl;
             cout << ">-----------------------------------------------------------<" << endl << endl;
             break;
         }
         case 1 : {
             //keeping the thread priority 1 for lowest level
             kernel->currentThread->setThreadPriority(1);
             cout << endl << endl;
             cout << ">-----------------------< Context Switch >-----------------<" << endl;
             cout << "Switching from " << kernel->currentThread->getName() << " to " << nextThread->getName() << " : Time quantum q4 = "<< kernel->q4 << " expired" << endl;  
             cout << "Keeping priority of " << kernel->currentThread->getName() << " to 1 because it is lowest level" << endl;
             cout << ">-----------------------------------------------------------<" << endl << endl;
             break;
         }
      }


    
    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    
    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".


    
   


    

    
   
    SWITCH(oldThread, nextThread);

    // we're back, running oldThread
      
    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();		// check if thread we were running
					// before this one has finished
					// and needs to be cleaned up
    
    if (oldThread->space != NULL) {	    // if there is an address space
        oldThread->RestoreUserState();     // to restore, do it.
	oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
}
 
//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    cout << "Ready list contents:\n";
    Level1->Apply(ThreadPrint);
    Level2->Apply(ThreadPrint);
    Level3->Apply(ThreadPrint);
    Level4->Apply(ThreadPrint);
}

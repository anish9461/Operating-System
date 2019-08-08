#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "IOInterrupt.h"
#include "string.h"
#include "stats.h"
#include <fstream>
#include <cstdlib>
#include "switch.h"
#include "synch.h"
#include "sysdep.h"

//creating instance of IOInterrupt to call IOWrite and IORead
IOInterrupt *IOInt;
string s;

//Function to carry out IO Operations
void IOFunction(int i)
{
    cout << endl;
    cout << ">----------------------< IO-bound function >----------------------<"<< endl;
    cout << "This is IO function for Thread t"<< kernel->currentThread->getThreadID() << endl;
    //IOWrite operation that calls IOInterrupt
    IOInt->IOWrite("Operating System ");
    //IORead operation that calls Interrupt
    string text = IOInt->IORead(s);
    IOInt->IOWrite("Assignment 1");
    cout << endl;
    cout << ">---------------------------------------------------------------------------<" << endl;
    cout << "Finishing IO-bound thread with thread ID: " << kernel->currentThread->getThreadID() << endl;
    cout << ">---------------------------------------------------------------------------<" << endl << endl;
    kernel->currentThread->Finish();
}

//Function to carry out CPU computing operation
void ComputeFunction(int i)
{
    cout << endl;
    cout << ">----------------------< CPU-bound function >----------------------<"<< endl;
    cout << "This is compute function for thread t"<< kernel->currentThread->getThreadID() << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    kernel->interrupt->OneTick();
    cout << "Multiplication : 134 * 243 = " << 134 * 243 << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    for(int j = 1; j < 101 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }

    cout << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;

    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    cout << endl;
    cout << ">---------------------------------------------------------------------------<" << endl;
    cout << "Finishing CPU-bound thread with thread ID: " << kernel->currentThread->getThreadID() << endl;
    cout << ">---------------------------------------------------------------------------<" << endl << endl;
    kernel->currentThread->Finish();
}

//function to carry out combination of computing operation and IO operation
void Hybrid(int i)
{
    cout << ">----------------------< Hybrid function >----------------------<"<< endl;
    cout << "This is Hybrid function pointer for thread t" << kernel->currentThread->getThreadID() << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    //IOWrite operation that calls IOInterrupt
    IOInt->IOWrite("Hybrid function ");
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
cout << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
cout << endl;
for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
cout << endl;
    for(int j = 1; j < 50 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
cout << endl;
    for(int j = 1; j < 21 ; j++)
    {
        kernel->interrupt->OneTick();
        cout << j << " ";
    }
    cout << endl;
    //IORead operation that calls IOInterrupt for Read
    string text1 = IOInt->IORead(s);
    cout << endl;
    cout << ">---------------------------------------------------------------------------<" << endl;
    cout << "Finishing Mixed thread with thread ID: " << kernel->currentThread->getThreadID() << endl;
    cout << ">---------------------------------------------------------------------------<" << endl << endl;
    //Finishing the Hybrid Thread
    kernel->currentThread->Finish();
}

//------------< Defined function that creates child threads which are CPU-bound, IO bound or Hybrid for executing program >--------------
void myFunction()
{
cout << endl;
cout << "-----------------------< Simulating MLFQ in Nachos >-------------------------" << endl; 
cout << endl;
cout << "Number of levels is MLFQ : 4" << endl;
cout << "Level 1 has priority 4 with Quantum q1 = " << kernel->q1 << endl;
cout << "Level 2 has priority 3 with Quantum q2 = " << kernel->q2 << endl;
cout << "Level 3 has priority 2 with Quantum q3 = " << kernel->q3 << endl;
cout << "Level 4 has priority 1 with Quantum q4 = " << kernel->q4 << endl;
cout << endl;
cout << "The number threads forked for the simulation : 12" << endl << endl;

cout << "Number of IO-bound threads : 5" << endl;
cout << "Number of CPU-bound threads : 5" << endl;
cout << "Number of Mixed threads : 2" << endl;

//Enabling the interrupts 
kernel->interrupt->Enable();

//Forking 5 IO-bound threads

Thread *t1 = new Thread("IO bound thread t1");
t1->setThreadID(1);
t1->setThreadPriority(4);
t1 -> Fork((VoidFunctionPtr) IOFunction, (void *) 1);

Thread *t2 = new Thread("IO bound thread t2");
t2->setThreadID(2);
t2->setThreadPriority(4);
t2 -> Fork((VoidFunctionPtr) IOFunction, (void *) 2);

Thread *t3 = new Thread("IO bound thread t3");
t3->setThreadID(3);
t3->setThreadPriority(4);
t3->Fork((VoidFunctionPtr) IOFunction, (void *) 3);

Thread *t4 = new Thread("IO bound thread t4");
t4->setThreadID(4);
t4->setThreadPriority(4);
t4->Fork((VoidFunctionPtr) IOFunction, (void *) 4);

Thread *t5 = new Thread("IO bound thread t5");
t5->setThreadID(5);
t5->setThreadPriority(4);
t5->Fork((VoidFunctionPtr) IOFunction, (void *) 5);

//forking 5 CPU bound threads

Thread *t6 = new Thread("CPU bound thread t6");
t6->setThreadID(6);
t6->setThreadPriority(4);
t6->Fork((VoidFunctionPtr) ComputeFunction, (void *) 6);


Thread *t7 = new Thread("CPU bound thread t7");
t7->setThreadID(7);
t7->setThreadPriority(4);
t7 -> Fork((VoidFunctionPtr) ComputeFunction, (void *) 7);


Thread *t8 = new Thread("CPU bound thread t8");
t8->setThreadID(8);
t8->setThreadPriority(4);
t8 -> Fork((VoidFunctionPtr) ComputeFunction, (void *) 8);

Thread *t9 = new Thread("CPU bound thread t9");
t9->setThreadID(9);
t9->setThreadPriority(4);
t9 -> Fork((VoidFunctionPtr) ComputeFunction, (void *) 9);

Thread *t10 = new Thread("CPU bound thread t10");
t10->setThreadID(10);
t10->setThreadPriority(4);
t10->Fork((VoidFunctionPtr) ComputeFunction, (void *) 10);

//forking 2 Hybrid threads

Thread *t11 = new Thread("Hybrid thread t11");
t11->setThreadID(11);
t11->setThreadPriority(4);
t11->Fork((VoidFunctionPtr) Hybrid, (void *) 11);

Thread *t12 = new Thread("Hybrid thread t12");
t12->setThreadID(12);
t12->setThreadPriority(4);
t12-> Fork((VoidFunctionPtr) Hybrid, (void *) 12);

//finishing the main thread
kernel->currentThread->Finish();

}

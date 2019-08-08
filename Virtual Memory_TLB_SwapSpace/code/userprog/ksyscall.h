/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "string.h"
#include "machine.h"
#include "main.h"
#include "list.h"


//halt System Call
void SysHalt()
{
  cout << "Number of TLB Hits : " << kernel->stats->tlbHits << endl;
      cout << "Number of Page Hits : " << kernel->stats->numPageHits << endl;
      cout << "Number of Page Faults : " << kernel->stats->numPageFaults << endl << endl;
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

//Exit System call
void SysExit(int s)
{
 
  if(s == 0)
    {
    cout << ">==================================< Exit System Call >=====================================<" << endl << endl;
    for (int i = 0; i < TLBSize; i++)
    {
      if(kernel->machine->tlb[i].id == kernel->currentThread->getID())
		{
   
			kernel->physicalPagesBitmap->Clear(i);
            kernel->machine->tlb[i].valid = FALSE;
	        kernel->machine->tlb[i].use = FALSE;
	        kernel->machine->tlb[i].dirty = FALSE;
	        kernel->machine->tlb[i].readOnly = FALSE;
            kernel->machine->tlb[i].id = -1;
            kernel->machine->tlb[i].virtualPage = -1;
            kernel->listOfPagesUsed->Remove(i);
            //clearing out the occupied main memory
            bzero(&(kernel->machine->mainMemory[i * PageSize]),PageSize);		
		}
    }
    //clearing the page table entry associated with that process
    for(int i = 0; i < kernel->machine->pageTableSize; i++)
	{
		if(kernel->machine->pageTable[i].id == kernel->currentThread->getID())
		{
   
			kernel->physicalPagesBitmap->Clear(i);
            kernel->machine->pageTable[i].valid = FALSE;
	        kernel->machine->pageTable[i].use = FALSE;
	        kernel->machine->pageTable[i].dirty = FALSE;
	        kernel->machine->pageTable[i].readOnly = FALSE;
            kernel->machine->pageTable[i].id = -1;
            kernel->machine->pageTable[i].virtualPage = -1;
            if(kernel->listOfPagesUsed->IsInList(i))
                {
                  kernel->listOfPagesUsed->Remove(i);
                }
            //clearing out the occupied main memory
            bzero(&(kernel->machine->mainMemory[i * PageSize]),PageSize);		
		}
	}
  //displaying performance statistics
      cout << endl;
      cout << ">==================================< User Program : " << kernel->currentThread->getName() << " : Exiting >=============================<" << endl << endl;
      cout << ">------------------< Displaying statistics of the User program >---------------------<" << endl << endl;
      cout << "Number of TLB Hits : " << kernel->stats->tlbHits << endl;
      cout << "Number of Page Hits : " << kernel->stats->numPageHits << endl;
      cout << "Number of Page Faults : " << kernel->stats->numPageFaults << endl << endl;
      cout << ">------------------------------------------------------------------------------------<" << endl << endl;
      cout << ">============================================================================================================<" << endl << endl;
      cout << ">===========================================================================================<" << endl << endl;
      kernel->currentThread->Finish();
      
    }
   
}

//Yield system call to yield user thread
void SysUserYield()
{
  cout << endl;
  cout << ">---------< Yield test >---------< " << endl << endl;
  cout << "User Thread Yielding!!" << endl << endl;
  cout << ">--------------------------------<" << endl << endl;
  kernel->currentThread->Yield();
}

//Write system call to write onto console
int SysWrite(int b, int s,int r)
{
  int buffer = b;
   int size = s;
  int outputBuffer;
  int count = 0;
  string writeoutput;
  while(count != size)
  {
    kernel->machine->ReadMem(buffer, 1, &outputBuffer);
    writeoutput = writeoutput + (char)outputBuffer;
    count++;
    buffer++;
  } 
  cout << ">" << writeoutput << "<" << endl;
  return 0; 
}

//Read System Call to read from the console
void SysRead(int b, int s,int r)
{
  char buf[s];
  cout << "Enter string to read from Console: " << endl;
  cin >> buf;
  cout << "The ouput read from Console is : " << buf << endl;  
}

#endif /* ! __USERPROG_KSYSCALL_H__ */

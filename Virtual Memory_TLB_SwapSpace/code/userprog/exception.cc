// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "addrspace.h"
#include "machine.h"
#include "translate.h"
#include "noff.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void
TestThread(int which)
{
    int num;
    
				cout << ">-----------< Forked thread output >-----------<" << endl << endl;
        printf("*** thread %d looped %d times\n", which, num);
				cout << endl;
				cout << ">----------------------------------------------<" << endl << endl;
  
}


void
ExceptionHandler(ExceptionType which)
{
	int len;
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");
    switch (which) {
    case SyscallException:
      switch(type) {
      case SC_Halt:
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
	cout << endl;
	cout << ">==================================< Halt System Call >=====================================<" << endl << endl;
	SysHalt();
	cout << ">===========================================================================================<" << endl << endl;
	ASSERTNOTREACHED();
	break;

      case SC_Add:
			
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


case SC_Exit:

		SysExit((int)kernel->machine->ReadRegister(4));
	
{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}
	
	return;


case SC_Read:

cout << endl;
cout << ">==================================< Read System Call >=====================================<" << endl << endl;
		cout << ">-------------< Reading from Console >-----------------< " << endl << endl;
		
		SysRead((int)kernel->machine->ReadRegister(4),(int)kernel->machine->ReadRegister(5),(int)kernel->machine->ReadRegister(6));
		cout << "The above string is read from Console!!" << endl << endl;
		cout << ">-------------< Done Reading >-----------------< " << endl << endl;
{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}
cout << ">===========================================================================================<" << endl << endl;
	return;

case SC_Write:
{
		cout << endl;
		cout << ">==================================< Write System Call >=====================================<" << endl << endl;
		cout << ">-----------------< Writing the following on Console: >------------------< " << endl << endl;

  
			len = SysWrite((int)kernel->machine->ReadRegister(4),(int)kernel->machine->ReadRegister(5),(int)kernel->machine->ReadRegister(6));
			kernel->machine->WriteRegister(2, (int)kernel->machine->ReadRegister(5));
			cout << "Done Writing onto console!!" << endl << endl;
			cout << ">-------------------< Done Writing >----------------< " << endl << endl;

	
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}
	cout << ">===========================================================================================<" << endl << endl;
	return;

case SC_UserYield:
{
	cout << endl;
	cout << ">==================================< Yield System Call >=====================================<" << endl << endl;
		SysUserYield();
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	
}
		//ASSERTNOTREACHED();
	return;

case SC_UserFork:
{
cout << endl;
	cout << ">==================================< Fork System Call >=====================================<" << endl << endl;
		Thread *t = new Thread("forked thread");
		t->Fork((VoidFunctionPtr) TestThread, (void *) 1);

	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	

cout << ">===========================================================================================<" << endl << endl;
}
	return;

      default:
	cerr << "Unexpected system call " << type << "\n";

	break;
      }
      break;

	//Page fault exception		
	 case PageFaultException:
	 {
		 cout << endl;
		 kernel->stats->numPageFaults++;
	 	cout << ">==================================< Page Fault >=====================================<" << endl << endl;
		unsigned int vpn, offset;

			//bad virtual address will give vpn not there in main memory
			int badVaddress = kernel->machine->ReadRegister(39);
			vpn = (unsigned) badVaddress / PageSize;
			cout << "Virtual Page Number : " << vpn << endl;
		
			int getSwapCount = kernel->vpnToSwapMap[vpn];
			cout << "Getting Page from Swap Space.." << endl;
			//copy from swap space to main memory
			
			int ppn = kernel->physicalPagesBitmap->FindAndSet();
			
    char *buffer = new char[PageSize];
		kernel->swapExecutable->ReadAt(buffer, PageSize, getSwapCount * PageSize);
		
		//if main memory is empty copy directly from swap space to main memory and in TLB
    if(ppn != -1)
    {	
				cout << "Space available in Main Memory!!!"<< endl;
				cout << "Available Physical Page : " << ppn << endl;
				cout << "Adding Entry in TLB.." << endl;
				for(int i = 0; i < TLBSize ; i++)
			{
				if(kernel->machine->tlb[i].physicalPage = ppn)
				{
				kernel->machine->tlb[i].valid = TRUE;
        kernel->machine->tlb[i].virtualPage = vpn;
        kernel->machine->tlb[i].physicalPage = ppn;
        kernel->machine->tlb[i].id = kernel->currentThread->getID();
				break;
				}
			}

				cout << "Adding Entry in IPT.." << endl;
        kernel->machine->pageTable[ppn].valid = TRUE;
        kernel->machine->pageTable[ppn].virtualPage = vpn;
        kernel->machine->pageTable[ppn].physicalPage = ppn;
        kernel->machine->pageTable[ppn].id = kernel->currentThread->getID();

        //clear the existing space
        bzero(&(kernel->machine->mainMemory[ppn * PageSize]), PageSize);

        //copy from buffer to main memory
        bcopy(buffer, &(kernel->machine->mainMemory[ppn * PageSize]), PageSize);
				cout << "Loading to Main Memory" << endl;
        //add page table to list of pages used ( for FIFO to remove later )
        kernel->listOfPagesUsed->Append(ppn);
		
    }
		
		//if all pages in main memory are occupied
		//remove the first page according to FIFO.(also remove from the list)
		//get swap space count of the removed page and store the page back to the swap executable
		//then clear the main memory and copy new page from swap executable to main memory
		else
		{
			ppn = kernel->listOfPagesUsed->RemoveFront();
			 cout << ">----< Main Memory Full >---< Page Replacement using FIFO >---<" << endl;
			 //get the evicted swap count
			 int swapCountEvicted = kernel->vpnToSwapMap[kernel->machine->pageTable[ppn].virtualPage];
			// cout << "evicted swap count " << swapCountEvicted << endl;
				char *tempbuffer = new char[PageSize];
			 //copy from memory to buffer
			 bcopy(&(kernel->machine->mainMemory[ppn * PageSize]), tempbuffer, PageSize);
				//copy from buffer to swap file
			 kernel->swapExecutable->WriteAt(tempbuffer,PageSize,swapCountEvicted * PageSize);
			cout << "Evicted Physical Page : " << ppn << endl;

/////////////////////////////////////////

		//load page in tlb
			
			for(int i = 0; i < TLBSize ; i++)
			{
			
				if(kernel->machine->tlb[i].physicalPage == ppn)
				{
					cout << "Adding Entry in TLB.." << endl;
				kernel->machine->tlb[i].valid = TRUE;
        kernel->machine->tlb[i].virtualPage = vpn;
        kernel->machine->tlb[i].physicalPage = ppn;
        kernel->machine->tlb[i].id = kernel->currentThread->getID();
				}
			}

      cout << "Adding Entry in IPT.." << endl;

		//change entry of new page - vpn and process ID
			kernel->machine->pageTable[ppn].valid = TRUE;
			kernel->machine->pageTable[ppn].virtualPage = vpn;
			kernel->machine->pageTable[ppn].physicalPage = ppn;
    	kernel->machine->pageTable[ppn].id = kernel->currentThread->getID();
			//clear main memory
			bzero(&(kernel->machine->mainMemory[ppn * PageSize]), PageSize);

			//copy from buffer to main memory
        bcopy(buffer, &(kernel->machine->mainMemory[ppn * PageSize]), PageSize);
			cout << "Loading Page to Main Memory from swap space..." << endl;
			cout << ">------< Page Replacement Successful >-------< " << endl;

        //add page table to list of pages used ( for FIFO to remove later )
        kernel->listOfPagesUsed->Append(ppn);
				delete [] tempbuffer;
	 	}
		 delete [] buffer;
		 

		 cout << endl;
		 cout << ">===============================< Page Fault Handled >==================================<" << endl << endl;
	
	 }
	return;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();
	  
}



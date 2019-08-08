#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "filehdr.h"
#include "openfile.h"
#include "filesys.h"

//creating threads for testing
Thread *t2 = new Thread("forked thread");
Thread *t3 = new Thread("forked thread");
Thread *t4 = new Thread("forked thread");

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        kernel->currentThread->Yield();
    }
}

//fileoperation 1 and 2 for reading multiple files
void FileOperation1(int i)
{
   OpenFile *file1;
   //open the file 1
    file1 = kernel->fileSystem->Open("file1");
    //buffer to store the content to tfile 1
    char buffer[27]("Additional write in file 1");
    if(file1 != NULL)
    {
        //seek the location of the file to be written to
    file1->Seek(25);
    //write to the file
    file1->Write(buffer, 27);
    cout << "Next Wrote in file 1 : " << buffer << endl;
//close the file after writing
kernel->fileSystem->CloseFile("file1");
    }
    //clear the buffer
    memset(buffer,0,27);
    OpenFile *file;
    //open the file for reading
    file = kernel->fileSystem->Open("file1");
    if(file != NULL)
    {
     file->Read(buffer,10);
     cout << "Thread 1 Read from file 1 : " << buffer << endl;
    //close the file
     kernel->fileSystem->CloseFile("file1");
    }
    kernel->scheduler->ReadyToRun(t2);

 }

void FileOperation2(int i)
{
    kernel->currentThread->Sleep(FALSE);
    char buffer2[50]; //buffer to read into
    memset(buffer2,0,50);
    OpenFile *file;
    file = kernel->fileSystem->Open("file1"); //open the file
    if(file != NULL)
    {
     file->Read(buffer2,15); //read from file 1 concurrently
     cout << "Thread 2 Read from file 1 :" << buffer2 << endl;
     kernel->fileSystem->CloseFile("file1");
    cout << endl;
    cout << "Thread 2 deleting file 1" << endl;
    kernel->fileSystem->Remove("file1"); //delete the file
    cout << endl;
    }
    cout << ">------------------< Test case done for Multiple Read/Write and file extendable >-----------------<" << endl << endl;
    kernel->scheduler->ReadyToRun(t3); //run the next test case
    kernel->scheduler->ReadyToRun(t4);

}

void FileOperation3(int i)
{
    kernel->currentThread->Sleep(FALSE);
   
    cout << endl;
    cout << ">-------------------< Test for Deleting file even when other thread is using the file >--------------< " << endl << endl;
    
    OpenFile *file1;
    
    file1 = kernel->fileSystem->Open("file2"); //open file 2
    
    char buffer[27]("Data in file 2"); //store the data from buffer to file 2
    
    if(file1 != NULL)
    {
    
    
    file1->Write(buffer, 27); //write data to file 2
    cout << "Wrote in file 2 : " << buffer << endl;

kernel->fileSystem->CloseFile("file2"); //close the file

    }
    memset(buffer,0,27); //reset the buffer
    OpenFile *file;
    file = kernel->fileSystem->Open("file2"); //open the file 2 for reading
    if(file != NULL)
    {
     file->Read(buffer,8); // read from the file into buffer
     cout << "Thread 3 Read from file 2 : " << buffer << endl;

     kernel->fileSystem->CloseFile("file2");
     cout << "Thread 3 deleting file 2..." << endl;
     kernel->fileSystem->Remove("file2"); //delete the file
    }
    
}


void FileOperation4(int i)
{
    kernel->currentThread->Sleep(FALSE);
    
    char buffer2[50];
    memset(buffer2,0,50);
    OpenFile *file;
    file = kernel->fileSystem->Open("file2"); //open file 2
    if(file != NULL)
    {
     file->Read(buffer2,15); //read from the buffer
     cout << "Thread 4 Read from file 2 :" << buffer2 << endl;
     kernel->fileSystem->CloseFile("file2"); //close the file
    cout << endl;
    cout << "Thread 4 deleting file 2..." << endl;
    kernel->fileSystem->Remove("file2"); //delete the file
    }
    cout << ">------------------< Test case done for Delete if the file is not closed by other threads >-----------------<" << endl << endl;


}


void
ThreadTest()
{
    cout << ">===============================< Assignment 3 >============================<" << endl << endl;
    
OpenFile *file1;
    file1 = kernel->fileSystem->Open("file1");
    char buffer[25]("This is a Data of File 1");
    file1->Write(buffer, 22);
    cout << "Wrote in file 1 : " << buffer << endl;
    kernel->fileSystem->CloseFile("file1");
    Thread *t1 = new Thread("forked thread");
    t1->Fork((VoidFunctionPtr) FileOperation1, (void *) 1);

    t2->Fork((VoidFunctionPtr) FileOperation2, (void *) 1);
   
     t3->Fork((VoidFunctionPtr) FileOperation3, (void *) 1);

     t4->Fork((VoidFunctionPtr) FileOperation4, (void *) 1);

}

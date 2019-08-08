#include "request.h"
#include "thread.h"

//request constructor
request::request()
{

}

//set IOtype for the request
void request::setType(string IOtype)
{
	type = IOtype;
}
//return IO type for the request
string request::getType()
{
	return type;
}

//set thread state for the IO thread
void request::setThreadState(Thread *currentthread)
{
	t = currentthread;
}
//return thread state for the IO thread
Thread* request::getThreadState()
{
	return t;
}


//set delay for the IO thread
void request::setDelay(int d)
{
	delay = d;
}
//return delay for the IO thread
int request::getDelay()
{
	return delay;
}


//store write output for the IO thread
void request::setWriteOutput(string write)
{
	writeOutput = write;
}

//return write output for the IO thread
string request::getWriteOutput()
{
	return writeOutput;
}

//store read output for the IO thread
void request::setReadOutput(string read)
{
	readOutput = read;
}
//return read output for the IO thread
string request::getReadOutput()
{
	return readOutput;
}


//store completion time for the IO thread
void request::setCompletionTime(int c)
{
	ctime = c;
}
//return completion time for IO thread
int request::getCompletionTime()
{
	return ctime;
}

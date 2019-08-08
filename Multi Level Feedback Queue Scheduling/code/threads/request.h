#ifndef REQUEST_H
#define REQUEST_H


#include "string.h"
#include "thread.h"


class request{
	private:
		string type; //IO type
		string writeOutput; //write output
		Thread *t; //IO thread
		int delay; //random delay
		string readOutput; //read output
		int ctime; //completion time
	public:
		request(); //constructor
		~request(){}; //destructor

		void setType(string type); //set IO type
		string getType(); 		   //get IO type

		void setThreadState(Thread *currentthread); //set thread stste
		Thread* getThreadState();  					//get thread state

		void setDelay(int d);			//set random delay
		int getDelay();					//get random delay

		void setWriteOutput(string write); //set write output
		string getWriteOutput();		   //get write output

		void setReadOutput(string c);		//set read output
		string getReadOutput();             //get read output

		
		void setCompletionTime(int c);		//set completion time
		int getCompletionTime();			//get completion time

};

#endif
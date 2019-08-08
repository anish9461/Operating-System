#include "syscall.h"

int main()
{
	char *s = "This is Write Test";
	int size = 18;
	OpenFileId id = ConsoleOutput; 
	int len = Write(s,size,id);
	
	Exit(0);
}

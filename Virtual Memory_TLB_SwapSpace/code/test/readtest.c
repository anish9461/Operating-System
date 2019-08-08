#include "syscall.h"

int main()
{
	char *s;
	int size = 18;
	OpenFileId id = ConsoleInput; 
	Read(s,size,id);
	return 0;
}

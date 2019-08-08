#include "string.h"
#include "kernel.h"
#include "thread.h"
#include "stats.h"
#include "request.h"
#include "main.h"


using namespace std;
class IOInterrupt
{
private:
    string s;
public:
    IOInterrupt();
    ~IOInterrupt(){}
    void IOWrite(string s);
    string IORead(string s);

};
#include "request.h"

class IOHandler
{
    public:
	IOHandler();
        ~IOHandler(){};
        void write(request *r);
        void read(request *r);
};